#include "rt.h"


void	move_ellipsoid(t_obj *ellipsoid, t_3vecf dir, double fact)
{
	t_ellipsoid	*param;
	t_cut		*cuts;

	param = (t_ellipsoid *)ellipsoid->obj_param;
	param->origin.val[0] += dir.val[0] * fact;
	param->origin.val[1] += dir.val[1] * fact;
	param->origin.val[2] += dir.val[2] * fact;
	cuts = ellipsoid->cuts;
	while (cuts)
	{
		if (cuts->move && cuts->cut_type != CUT_STATIC)
			cuts->move(cuts, dir, fact);
		cuts = cuts->next;
	}
}

int		check_inside_ellipsoid(t_3vecf point, t_obj *ellipsoid)
{
	t_ellipsoid	*param;

	param = (t_ellipsoid *)ellipsoid->obj_param;
	point = sub_3vecf(point, param->origin);
	point.val[0] = (point.val[0] * point.val[0])
		/ (param->x_fact * param->x_fact);
	point.val[1] = (point.val[1] * point.val[1])
		/ (param->y_fact * param->y_fact);
	point.val[2] = (point.val[2] * point.val[2])
		/ (param->z_fact * param->z_fact);
	if (point.val[0] + point.val[1] + point.val[2] > 1)
		return (0);
	return (1);
}

void	assign_ellipsoid_function(t_obj *ellipsoid)
{
	ellipsoid->obj_type = OBJ_ELLIPSOID;
	ellipsoid->check_inside = &check_inside_ellipsoid;
	ellipsoid->ray_intersect = &ray_intersect_ellipsoid;
	ellipsoid->get_normal_inter = &get_normal_intersect_ellipsoid;
	ellipsoid->get_origin = &get_origin_ellipsoid;
	ellipsoid->move = &move_ellipsoid;
	ellipsoid->rotate = NULL;
	ellipsoid->get_text_coordinate = &get_text_coordinate_ellipsoid;
}

void	generate_new_ellipsoid(t_data *data)
{
	t_obj		*ellipsoid;
	t_ellipsoid	*param;
	t_3vecf		dir;

	dir = mult_3vecf_33matf(mult_3vecf_33matf(window_to_view(0, 0,
		data->size.val[0], data->size.val[1]), data->rot_mat[1]),
			data->rot_mat[0]);
	normalize_3vecf(&dir);
	if (!(ellipsoid = ft_memalloc(sizeof(t_obj))))
		return ;
	if (!(param = ft_memalloc(sizeof(t_ellipsoid))))
		return ;
	param->origin = add_3vecf(data->camera->origin, product_c3vecf(dir, 2));
	param->x_fact = get_random_number((time(NULL) * 0xcacacaca) << 16) * 2.5;
	param->y_fact = get_random_number((time(NULL) * 0xabcdef99) << 4) * 2.5;
	param->z_fact = get_random_number((time(NULL) * 0xff3672ff) << 3) * 2.5;
	ellipsoid->obj_param = param;
	assign_ellipsoid_function(ellipsoid);
	ellipsoid->text = generate_random_texture(ellipsoid);
	set_bump_own(ellipsoid);
	add_object(ellipsoid, data);
	data->new_obj = 1;
}
