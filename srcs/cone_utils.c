/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cone_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aplat <aplat@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/18 01:09:07 by aplat             #+#    #+#             */
/*   Updated: 2020/05/18 01:09:08 by aplat            ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

void	move_cone(t_obj *cone, t_3vecf dir, double fact)
{
	t_cone	*param;
	t_cut	*cuts;

	param = (t_cone *)cone->obj_param;
	param->center.val[0] += dir.val[0] * fact;
	param->center.val[1] += dir.val[1] * fact;
	param->center.val[2] += dir.val[2] * fact;
	param->tip.val[0] += dir.val[0] * fact;
	param->tip.val[1] += dir.val[1] * fact;
	param->tip.val[2] += dir.val[2] * fact;
	cuts = cone->cuts;
	while (cuts)
	{
		if (cuts->move && cuts->cut_type != CUT_STATIC)
			cuts->move(cuts, dir, fact);
		cuts = cuts->next;
	}
}

void	rotate_cone(t_obj *cone, t_3vecf orig, t_33matf rot_mat[2])
{
	t_cone	*param;
	t_cut	*cuts;

	param = (t_cone *)cone->obj_param;
	param->center = sub_3vecf(param->center, orig);
	param->tip = sub_3vecf(param->tip, orig);
	param->center = mult_3vecf_33matf(param->center, rot_mat[1]);
	param->center = mult_3vecf_33matf(param->center, rot_mat[0]);
	param->tip = mult_3vecf_33matf(param->tip, rot_mat[1]);
	param->tip = mult_3vecf_33matf(param->tip, rot_mat[0]);
	param->center = add_3vecf(param->center, orig);
	param->tip = add_3vecf(param->tip, orig);
	cuts = cone->cuts;
	while (cuts)
	{
		if (cuts->rotate && cuts->cut_type != CUT_STATIC)
			cuts->rotate(cuts, orig, rot_mat);
		cuts = cuts->next;
	}
}

int		check_inside_cone(t_3vecf inter_point, t_obj *obj)
{
	t_cone	*param;
	double	c_dist;
	double	height;
	t_3vecf v[3];
	int		check_rev;

	check_rev = 0;
	param = (t_cone *)obj->obj_param;
	v[0] = sub_3vecf(param->center, param->tip);
	height = get_length_3vecf(v[0]);
	normalize_3vecf(&(v[0]));
	v[1] = sub_3vecf(inter_point, param->tip);
	c_dist = dot_product_3vecf(v[1], v[0]);
	if (c_dist < 0 && (check_rev = 1))
		c_dist *= -1;
	v[2].val[0] =
		v[1].val[0] - c_dist * (check_rev ? -v[0].val[0] : v[0].val[0]);
	v[2].val[1] =
		v[1].val[1] - c_dist * (check_rev ? -v[0].val[1] : v[0].val[1]);
	v[2].val[2] =
		v[1].val[2] - c_dist * (check_rev ? -v[0].val[2] : v[0].val[2]);
	if (get_length_3vecf(v[2]) > (c_dist / height) * param->radius)
		return (0);
	return (1);
}

void	assign_cone_function(t_obj *cone)
{
	cone->obj_type = OBJ_CONE;
	cone->check_inside = &check_inside_cone;
	cone->ray_intersect = &ray_intersect_cone;
	cone->get_normal_inter = &get_normal_intersect_cone;
	cone->get_origin = &get_origin_cone;
	cone->move = &move_cone;
	cone->rotate = &rotate_cone;
	cone->get_text_coordinate = &get_text_coordinate_cone;
}

void	generate_new_cone(t_data *data)
{
	t_obj		*cone;
	t_cone		*param;
	t_3vecf		dir;

	dir = mult_3vecf_33matf(mult_3vecf_33matf(window_to_view(0, 0,
		data->size.val[0], data->size.val[1]), data->rot_mat[1]),
			data->rot_mat[0]);
	normalize_3vecf(&dir);
	if (!(cone = ft_memalloc(sizeof(t_obj))))
		return ;
	if (!(param = ft_memalloc(sizeof(t_cone))))
		return ;
	param->radius = get_random_number(time(NULL)) * 1.5;
	param->tip = add_3vecf(data->camera->origin, product_c3vecf(dir, 2));
	param->center = add_3vecf(assign_3vecf(0, 1, 0), param->tip);
	cone->obj_param = param;
	cone->text = generate_random_texture(cone);
	assign_cone_function(cone);
	set_bump_own(cone);
	add_object(cone, data);
	data->new_obj = 1;
}
