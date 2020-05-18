/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_plane_param.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aplat <aplat@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/18 01:53:12 by aplat             #+#    #+#             */
/*   Updated: 2020/05/18 01:53:39 by aplat            ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

int		init_plane_2daxis(t_plane *param, t_3vecf nrml,
	t_3vecf *u_axis, t_3vecf *v_axis)
{
	if (is_null_3vecf(param->x2d_axis) ||
		!is_null(dot_product_3vecf(param->x2d_axis, param->normal)))
	{
		if (nrml.val[0] != 0)
		{
			*u_axis = assign_3vecf(0, 1, 1);
			u_axis->val[0] = (-nrml.val[1] - nrml.val[2]) / nrml.val[0];
		}
		else if (nrml.val[1] != 0)
		{
			*u_axis = assign_3vecf(1, 0, 1);
			u_axis->val[1] = (-nrml.val[0] - nrml.val[2]) / nrml.val[1];
		}
		else if (nrml.val[2] != 0)
		{
			*u_axis = assign_3vecf(1, 1, 0);
			u_axis->val[2] = (-nrml.val[0] - nrml.val[1]) / nrml.val[2];
		}
		normalize_3vecf(u_axis);
		param->x2d_axis = *u_axis;
		*v_axis = product_3vecf(*u_axis, nrml);
		normalize_3vecf(v_axis);
		return (1);
	}
	return (0);
}

t_2vecf	get_text_coordinate_plane(t_3vecf inter_point,
	t_3vecf normal_inter, t_obj *plane)
{
	t_2vecf	text_coord;
	t_3vecf	u_axis;
	t_3vecf	v_axis;
	t_plane	*param;
	t_3vecf	origin_inter;

	param = (t_plane *)plane->obj_param;
	if (!init_plane_2daxis(param, normal_inter, &u_axis, &v_axis))
	{
		v_axis = param->x2d_axis;
		u_axis = product_3vecf(v_axis, normal_inter);
		normalize_3vecf(&u_axis);
	}
	origin_inter = sub_3vecf(inter_point, param->origin);
	text_coord.val[0] = dot_product_3vecf(origin_inter, u_axis) / 2;
	text_coord.val[1] = dot_product_3vecf(origin_inter, v_axis) / 2;
	return (text_coord);
}

t_3vecf	get_origin_plane(t_obj *plane)
{
	return (((t_plane *)plane->obj_param)->origin);
}

t_3vecf	get_plane_origin(t_obj *plane, t_plane *plane_param, int sp_id)
{
	if (sp_id)
		return (move_3vecf(plane_param->origin, plane->motions, sp_id));
	return (plane_param->origin);
}

t_3vecf	get_normal_intersect_plane(t_3vecf inter_point, t_obj *plane, int sp_id)
{
	return (((t_plane *)plane->obj_param)->normal);
	(void)inter_point;
	(void)sp_id;
}
