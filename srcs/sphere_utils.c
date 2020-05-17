#include "rt.h"

int		check_inside_sphere(t_3vecf point, t_obj *sphere)
{
	t_sphere	*param;

	param = (t_sphere *)sphere->obj_param;
	if (get_length_3vecf(sub_3vecf(point, param->origin)) > param->radius)
		return (0);
	return (1);
}

void	move_sphere(t_obj *sphere, t_3vecf dir, double fact)
{
	t_sphere	*param;
	t_cut		*cuts;

	param = (t_sphere *)sphere->obj_param;
	param->origin.val[0] += dir.val[0] * fact;
	param->origin.val[1] += dir.val[1] * fact;
	param->origin.val[2] += dir.val[2] * fact;
	cuts = sphere->cuts;
	while (cuts)
	{
		if (cuts->move && cuts->cut_type != CUT_STATIC)
			cuts->move(cuts, dir, fact);
		cuts = cuts->next;
	}
}

void	rotate_sphere(t_obj *sphere, t_3vecf orig, t_33matf rot_mat[2])
{
	t_sphere	*param;
	t_cut		*cuts;

	param = (t_sphere *)sphere->obj_param;
	param->origin = sub_3vecf(param->origin, orig);
	param->origin = mult_3vecf_33matf(param->origin, rot_mat[1]);
	param->origin = mult_3vecf_33matf(param->origin, rot_mat[0]);
	param->origin = add_3vecf(param->origin, orig);
	cuts = sphere->cuts;
	while (cuts)
	{
		if (cuts->rotate && cuts->cut_type != CUT_STATIC)
			cuts->rotate(cuts, orig, rot_mat);
		cuts = cuts->next;
	}
}

void	assign_sphere_function(t_obj *sphere)
{
	sphere->obj_type = OBJ_SPHERE;
	sphere->check_inside = &check_inside_sphere;
	sphere->ray_intersect = &ray_intersect_sphere;
	sphere->get_normal_inter = &get_normal_intersect_sphere;
	sphere->get_origin = &get_origin_sphere;
	sphere->move = &move_sphere;
	sphere->rotate = &rotate_sphere;
	sphere->get_text_coordinate = &get_text_coordinate_sphere;
}

void	generate_new_sphere(t_data *data)
{
	t_obj		*sphere;
	t_sphere	*param;
	t_3vecf		dir;

	dir = mult_3vecf_33matf(mult_3vecf_33matf(window_to_view(0, 0,
		data->size.val[0], data->size.val[1]), data->rot_mat[1]),
			data->rot_mat[0]);
	normalize_3vecf(&dir);
	if (!(sphere = ft_memalloc(sizeof(t_obj))))
		return ;
	if (!(param = ft_memalloc(sizeof(t_sphere))))
		return ;
	param->radius = get_random_number(time(NULL)) * 2.5;
	param->origin = add_3vecf(data->camera->origin,
		product_c3vecf(dir, param->radius * 2));
	sphere->obj_param = param;
	assign_sphere_function(sphere);
	sphere->text = generate_random_texture(sphere);
	set_bump_own(sphere);
	add_object(sphere, data);
	data->new_obj = 1;
}
