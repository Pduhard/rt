/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   plane.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aplat <aplat@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/15 20:52:38 by aplat             #+#    #+#             */
/*   Updated: 2020/05/18 05:11:25 by aplat            ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

int		ray_intersect_plane(t_leq l, t_obj *plane, t_dist dist, int sp_id)
{
	t_plane	*plane_param;
	double	div;
	double	inter_dist;
	t_3vecf	plane_origin;

	plane_param = (t_plane *)plane->obj_param;
	plane_origin = get_plane_origin(plane, plane_param, sp_id);
	div = dot_product_3vecf(l.dir, plane_param->normal);
	if (div == 0 || div != div)
		return (0);
	inter_dist = dot_product_3vecf(sub_3vecf(plane_origin, l.orig),
		plane_param->normal) / div;
	if (inter_dist < *(dist.dist) && inter_dist > dist.min_dist
		&& inter_dist < dist.max_dist)
	{
		*(dist.dist) = inter_dist;
		return (1);
	}
	return (0);
}
