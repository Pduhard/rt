/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   plane_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aplat <aplat@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/18 05:11:38 by aplat             #+#    #+#             */
/*   Updated: 2020/05/18 05:11:57 by aplat            ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

int		check_inside_plane(t_3vecf inter_point, t_obj *plane)
{
	t_plane *param;
	t_3vecf	inter_orig;

	param = (t_plane *)plane->obj_param;
	inter_orig = sub_3vecf(inter_point, param->origin);
	if (is_null_3vecf(inter_orig))
		return (1);
	if (is_null(dot_product_3vecf(inter_orig, param->normal)))
		return (1);
	return (0);
}

void	move_plane(t_obj *plane, t_3vecf dir, double fact)
{
	t_plane	*param;
	t_cut	*cuts;

	param = (t_plane *)plane->obj_param;
	param->origin.val[0] += dir.val[0] * fact;
	param->origin.val[1] += dir.val[1] * fact;
	param->origin.val[2] += dir.val[2] * fact;
	cuts = plane->cuts;
	while (cuts)
	{
		if (cuts->move && cuts->cut_type != CUT_STATIC)
			cuts->move(cuts, dir, fact);
		cuts = cuts->next;
	}
}

void	rotate_plane(t_obj *plane, t_3vecf orig, t_33matf rot_mat[2])
{
	t_plane	*param;
	t_cut	*cuts;

	param = (t_plane *)plane->obj_param;
	param->origin = sub_3vecf(param->origin, orig);
	param->origin = mult_3vecf_33matf(param->origin, rot_mat[1]);
	param->origin = mult_3vecf_33matf(param->origin, rot_mat[0]);
	param->normal = mult_3vecf_33matf(param->normal, rot_mat[1]);
	param->normal = mult_3vecf_33matf(param->normal, rot_mat[0]);
	param->x2d_axis = mult_3vecf_33matf(param->x2d_axis, rot_mat[1]);
	param->x2d_axis = mult_3vecf_33matf(param->x2d_axis, rot_mat[0]);
	param->origin = add_3vecf(param->origin, orig);
	cuts = plane->cuts;
	while (cuts)
	{
		if (cuts->rotate && cuts->cut_type != CUT_STATIC)
			cuts->rotate(cuts, orig, rot_mat);
		cuts = cuts->next;
	}
}

void	assign_plane_function(t_obj *plane)
{
	plane->obj_type = OBJ_PLANE;
	plane->check_inside = &check_inside_plane;
	plane->ray_intersect = &ray_intersect_plane;
	plane->get_normal_inter = &get_normal_intersect_plane;
	plane->get_origin = &get_origin_plane;
	plane->move = &move_plane;
	plane->rotate = &rotate_plane;
	plane->get_text_coordinate = &get_text_coordinate_plane;
}

void	generate_new_plane(t_data *data)
{
	t_obj	*plane;
	t_plane	*param;
	t_3vecf	dir;

	dir = mult_3vecf_33matf(mult_3vecf_33matf(window_to_view(0, 0,
		data->size.val[0], data->size.val[1]), data->rot_mat[1]),
			data->rot_mat[0]);
	normalize_3vecf(&dir);
	if (!(plane = ft_memalloc(sizeof(t_obj))))
		return ;
	if (!(param = ft_memalloc(sizeof(t_plane))))
		return ;
	param->origin = add_3vecf(data->camera->origin, product_c3vecf(dir, 2));
	param->normal = assign_3vecf(
		get_random_number((time(NULL) * 0xcacacaca) << 16) - 0.5,
		get_random_number((time(NULL) * 0xfeabcdef) << 8) - 0.5,
		get_random_number((time(NULL) * 0x1056ffe) << 4) - 0.5);
	normalize_3vecf(&param->normal);
	plane->obj_param = param;
	assign_plane_function(plane);
	plane->text = generate_random_texture(plane);
	set_bump_own(plane);
	add_object(plane, data);
	data->new_obj = 1;
}
