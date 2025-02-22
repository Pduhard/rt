/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray_first_intersect.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aplat <aplat@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/18 05:14:25 by aplat             #+#    #+#             */
/*   Updated: 2020/05/19 19:08:28 by aplat            ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

static int	check_inside_negative(t_leq l, t_dist dist,
	t_data *data, int sp_id)
{
	t_obj	*negative_objs;
	t_3vecf	inter_point;

	inter_point = add_3vecf(l.orig, product_c3vecf(l.dir, *(dist.dist)));
	inter_point.val[0] = l.orig.val[0] + l.dir.val[0] * *(dist.dist);
	inter_point.val[1] = l.orig.val[1] + l.dir.val[1] * *(dist.dist);
	inter_point.val[2] = l.orig.val[2] + l.dir.val[2] * *(dist.dist);
	negative_objs = data->negative_objs;
	while (negative_objs)
	{
		if (negative_objs->check_inside(inter_point, negative_objs))
			return (check_cuts(l, dist, (t_cut_fparam){negative_objs, NULL, sp_id, 1}, data) ? 1 : 0);
		negative_objs = negative_objs->next;
	}
	return (0);
}

t_obj		*ray_first_intersect(t_leq l, t_dist dist, t_obj *objs,
	int sp_id)
{
	t_obj	*closest_obj;
	t_obj	*objs_save;
	t_data	*data;

	data = objs ? objs->data : NULL;
	objs_save = objs;
	closest_obj = NULL;
	*(dist.dist) = MAX_VIEW;
	while (objs)
	{
		if (objs->ray_intersect(l, objs, dist, sp_id))
			closest_obj = objs;
		objs = objs->next;
	}
	if (closest_obj && data->negative_objs
		&& check_inside_negative(l, dist, data, sp_id))
	{
		return (ray_first_intersect(l,
			(t_dist){dist.dist, *(dist.dist), dist.max_dist},
				objs_save, sp_id));
	}
	if (closest_obj && closest_obj->cuts)
		return (check_cuts(l, dist, (t_cut_fparam){closest_obj,
			objs_save, sp_id, 0}, data));
	return (closest_obj);
}
