#include "rt.h"

void	move_cylinder(t_obj *cylinder, t_3vecf dir, double fact)
{
	t_cylinder	*param;
	t_cut		*cuts;

	param = (t_cylinder *)cylinder->obj_param;
	param->center.val[0] += dir.val[0] * fact;
	param->center.val[1] += dir.val[1] * fact;
	param->center.val[2] += dir.val[2] * fact;
	param->tip.val[0] += dir.val[0] * fact;
	param->tip.val[1] += dir.val[1] * fact;
	param->tip.val[2] += dir.val[2] * fact;
	cuts = cylinder->cuts;
	while (cuts)
	{
		if (cuts->move && cuts->cut_type != CUT_STATIC)
			cuts->move(cuts, dir, fact);
		cuts = cuts->next;
	}
}

void	rotate_cylinder(t_obj *cylinder, t_3vecf orig, t_33matf rot_mat[2])
{
	t_cylinder	*param;
	t_cut		*cuts;

	param = (t_cylinder *)cylinder->obj_param;
	param->center = sub_3vecf(param->center, orig);
	param->tip = sub_3vecf(param->tip, orig);
	param->center = mult_3vecf_33matf(param->center, rot_mat[1]);
	param->center = mult_3vecf_33matf(param->center, rot_mat[0]);
	param->tip = mult_3vecf_33matf(param->tip, rot_mat[1]);
	param->tip = mult_3vecf_33matf(param->tip, rot_mat[0]);
	param->center = add_3vecf(param->center, orig);
	param->tip = add_3vecf(param->tip, orig);
	cuts = cylinder->cuts;
	while (cuts)
	{
		if (cuts->rotate && cuts->cut_type != CUT_STATIC)
			cuts->rotate(cuts, orig, rot_mat);
		cuts = cuts->next;
	}
}

int		check_inside_cylinder(t_3vecf inter_point, t_obj *cylinder)
{
	t_cylinder	*param;
	t_3vecf		tp;
	t_3vecf		cp;

	param = (t_cylinder *)cylinder->obj_param;
	tp = sub_3vecf(inter_point, param->tip);
	cp = sub_3vecf(inter_point, param->center);
	if (get_length_3vecf(product_3vecf(tp, cp))
		/ get_length_3vecf(sub_3vecf(param->tip, param->center))
			> param->radius)
		return (0);
	return (1);
}

void	assign_cylinder_function(t_obj *cylinder)
{
	cylinder->obj_type = OBJ_CYLINDER;
	cylinder->check_inside = &check_inside_cylinder;
	cylinder->ray_intersect = &ray_intersect_cylinder;
	cylinder->get_normal_inter = &get_normal_intersect_cylinder;
	cylinder->get_origin = &get_origin_cylinder;
	cylinder->move = &move_cylinder;
	cylinder->rotate = &rotate_cylinder;
	cylinder->get_text_coordinate = &get_text_coordinate_cylinder;
	// cylinder->get_text_color = &get_uni_color;
}

void	generate_new_cylinder(t_data *data)
{
	t_obj		*cylinder;
	t_cylinder	*param;
	t_3vecf		dir;

	dir = mult_3vecf_33matf(mult_3vecf_33matf(window_to_view(0, 0,
		data->size.val[0], data->size.val[1]), data->rot_mat[1]),
			data->rot_mat[0]);
	normalize_3vecf(&dir);
	if (!(cylinder = ft_memalloc(sizeof(t_obj))))
		return ;
	if (!(param = ft_memalloc(sizeof(t_cylinder))))
		return ;
	param->radius = get_random_number(time(NULL)) * 1.5;
	param->tip = add_3vecf(data->camera->origin, product_c3vecf(dir, 2));
	param->center = add_3vecf(assign_3vecf(0, 1, 0), param->tip);
	cylinder->obj_param = param;
	assign_cylinder_function(cylinder);
	cylinder->text = generate_random_texture(cylinder);
	set_bump_own(cylinder);
	add_object(cylinder, data);
	data->new_obj = 1;
}
