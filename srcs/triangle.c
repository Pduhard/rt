/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   triangle.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aplat <aplat@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/15 21:11:13 by aplat             #+#    #+#             */
/*   Updated: 2020/05/15 21:15:36 by aplat            ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

static void	get_triangle_corner(t_3vecf corner[3], t_obj *triangle, int sp_id)
{
	t_triangle *param;

	param = (t_triangle *)triangle->obj_param;
	corner[0] = sp_id ? move_3vecf(param->a,
		triangle->motions, sp_id) : param->a;
	corner[1] = sp_id ? move_3vecf(param->b,
		triangle->motions, sp_id) : param->b;
	corner[2] = sp_id ? move_3vecf(param->c,
		triangle->motions, sp_id) : param->c;
}

static double	get_inverse_delta(t_3vecf a_vec, t_3vecf dir_ac)
{
	double delta;

	delta = dot_product_3vecf(a_vec, dir_ac);
	if (is_null(delta))
		return (0);
	return (1. / delta);
}

static int		check_triangle_dist(double u, double v, t_dist dist, double root)
{
	if (v < 0 || u + v > 1)
		return (0);
	if (root < *(dist.dist) && root > dist.min_dist && root < dist.max_dist)
	{
		*(dist.dist) = root;
		return (1);
	}
	return (0);
}

int		ray_intersect_triangle(t_leq l, t_obj *triangle, t_dist dist, int sp_id)
{
	t_3vecf	a_vec[2];
	t_3vecf corner[3];
	t_3vecf	dir_ac;
	t_3vecf	tq_vec[2];
	double	tmp[3];

	get_triangle_corner(corner, triangle, sp_id);
	a_vec[0] = sub_3vecf(corner[1], corner[0]);
	a_vec[1] = sub_3vecf(corner[2], corner[0]);
	dir_ac = product_3vecf(l.dir, a_vec[1]);
	if (is_null((tmp[0] = get_inverse_delta(a_vec[0], dir_ac))))
		return (0);
	tq_vec[0] = sub_3vecf(l.orig, corner[0]);
	tmp[1] = dot_product_3vecf(tq_vec[0], dir_ac) * tmp[0];
	if (tmp[1] < 0 || tmp[1] > 1)
		return (0);
	tq_vec[1] = product_3vecf(tq_vec[0], a_vec[0]);
	tmp[2] = dot_product_3vecf(l.dir, tq_vec[1]) * tmp[0];
	return (check_triangle_dist(tmp[1], tmp[2], dist,
		dot_product_3vecf(a_vec[1], tq_vec[1]) * tmp[0]));
}
