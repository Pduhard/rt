#include "rt.h"

int		check_inside_moebius(t_3vecf point, t_obj *moebius)
{
	t_moebius	*param;
	double		v;
	double		u;
	double		sin_v_2;

	param = (t_moebius *)moebius->obj_param;
	v = atan2(point.val[1], point.val[0]);
	if (!is_null((sin_v_2 = sin(v / 2))))
		u = point.val[2] / sin_v_2;
	else if (!is_null(v))
		u = (point.val[0] / cos(v) - param->radius) / cos(v / 2);
	else
		u = (point.val[0] - param->radius);
	point.val[0] -= (param->radius + u * cos(v / 2)) * cos(v);
	point.val[1] -= (param->radius + u * cos(v / 2)) * sin(v);
	point.val[2] -= u * sin_v_2;
	if (is_null(point.val[0] * point.val[0] + point.val[1]
			* point.val[1] + point.val[2] * point.val[2])
			&& u < param->half_width && u > -param->half_width)
		return (1);
	return (0);
}

void	move_moebius(t_obj *moebius, t_3vecf dir, double fact)
{
	t_moebius	*param;
	t_cut		*cuts;

	param = (t_moebius *)moebius->obj_param;
	param->origin.val[0] += dir.val[0] * fact;
	param->origin.val[1] += dir.val[1] * fact;
	param->origin.val[2] += dir.val[2] * fact;
	cuts = moebius->cuts;
	while (cuts)
	{
		if (cuts->move && cuts->cut_type != CUT_STATIC)
			cuts->move(cuts, dir, fact);
		cuts = cuts->next;
	}
}


void	assign_moebius_function(t_obj *moebius)
{
	moebius->obj_type = OBJ_MOEBIUS;
	moebius->check_inside = &check_inside_moebius;
	moebius->ray_intersect = &ray_intersect_moebius;
	moebius->get_normal_inter = &get_normal_intersect_moebius;
	moebius->get_origin = &get_origin_moebius;
	moebius->move = &move_moebius;
	moebius->rotate = NULL;
	moebius->get_text_coordinate = &get_text_coordinate_moebius;
}

void	generate_new_moebius(t_data *data)
{
	t_obj		*moebius;
	t_moebius	*param;
	t_3vecf		dir;

	dir = mult_3vecf_33matf(mult_3vecf_33matf(window_to_view(0, 0,
		data->size.val[0], data->size.val[1]), data->rot_mat[1]),
			data->rot_mat[0]);
	normalize_3vecf(&dir);
	if (!(moebius = ft_memalloc(sizeof(t_obj))))
		return ;
	if (!(param = ft_memalloc(sizeof(t_moebius))))
		return ;
	param->radius = get_random_number((time(NULL) * 0xcacacaca) << 16) * 3;
	param->origin = add_3vecf(data->camera->origin,
		product_c3vecf(dir, param->radius + 2));
	param->half_width = get_random_number((time(NULL) * 0xabcdef99) << 4)
		* param->radius + 0.1;
	moebius->obj_param = param;
	assign_moebius_function(moebius);
	moebius->text = generate_random_texture(moebius);
	set_bump_own(moebius);
	add_object(moebius, data);
	data->new_obj = 1;
}
