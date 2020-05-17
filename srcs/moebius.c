/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   moebius.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aplat <aplat@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/15 19:43:19 by aplat             #+#    #+#             */
/*   Updated: 2020/05/15 19:47:45 by aplat            ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

t_4vecf	get_moebius_cubis_cst2(double o[3], double d[3], double r)
{
	t_4vecf		cst;

	cst.val[3] = (-r * r * o[1]) + (o[0] * o[0] * o[1])
		+ (o[1] * o[1] * o[1]) - (2 * r * o[0] * o[2])
			- (2 * o[0] * o[0] * o[2]) - (2 * o[1] * o[1] * o[2])
				+ (o[1] * o[2] * o[2]);
	cst.val[2] = (-r * r * d[1])
		+ (o[0] * o[0] * d[1]) + (2 * o[0] * o[1] * d[0])
			+ (o[1] * o[1] * d[1]) + (2 * o[1] * o[1] * d[1])
				- (2 * r * o[0] * d[2]) - (2 * r * o[2] * d[0])
					- (2 * o[0] * o[0] * d[2]) - (4 * o[0] * o[2] * d[0])
						- (2 * o[1] * o[1] * d[2]) - (4 * o[1] * o[2] * d[1])
						+ (2 * o[1] * o[2] * d[2]) + (o[2] * o[2] * d[1]);
	cst.val[1] = (2 * o[0] * d[0] * d[1]) + (o[1] * d[0] * d[0])
		+ (2 * o[1] * d[1] * d[1]) + (o[1] * d[1] * d[1])
			- (2 * r * d[0] * d[2]) - (4 * o[0] * d[0] * d[2])
				- (2 * o[2] * d[0] * d[0]) - (4 * o[1] * d[1] * d[2])
					- (2 * o[2] * d[1] * d[1]) + (o[1] * d[2] * d[2])
					+ (2 * o[2] * d[1] * d[2]);
	cst.val[0] = (d[0] * d[0] * d[1]) + (d[1] * d[1] * d[1])
		- (2 * d[0] * d[0] * d[2]) - (2 * d[1] * d[1] * d[2])
			+ (d[1] * d[2] * d[2]);
	return (cst);
}

t_4vecf	get_moebius_cubis_cst(t_moebius *param, t_3vecf moebius_origin,
	t_leq l)
{
	double		o[3];
	double		d[3];

	o[0] = l.orig.val[0] - moebius_origin.val[0];
	o[1] = l.orig.val[1] - moebius_origin.val[1];
	o[2] = l.orig.val[2] - moebius_origin.val[2];
	d[0] = l.dir.val[0];
	d[1] = l.dir.val[1];
	d[2] = l.dir.val[2];
	return (get_moebius_cubis_cst2(o, d, param->radius));
}

int		check_moebius_bnd(t_leq l, t_3vecf moebius_origin, double root,
	t_obj *moebius)
{
	t_3vecf	coord;

	coord.val[0] = l.orig.val[0] + l.dir.val[0] * root - moebius_origin.val[0];
	coord.val[1] = l.orig.val[1] + l.dir.val[1] * root - moebius_origin.val[1];
	coord.val[2] = l.orig.val[2] + l.dir.val[2] * root - moebius_origin.val[2];
	if ((moebius->check_inside(coord, moebius)))
		return (1);
	return (0);
}

int		ray_intersect_moebius(t_leq l, t_obj *moebius, t_dist dist, int sp_id)
{
	t_4vecf		cst;
	t_3vecf		roots;
	int			check;
	t_3vecf		moebius_origin;
	int			i;

	i = -1;
	check = 0;
	moebius_origin = get_moebius_origin(moebius,
		(t_moebius *)moebius->obj_param, sp_id);
	cst = get_moebius_cubis_cst((t_moebius *)moebius->obj_param,
		moebius_origin, l);
	roots = solve_cubic(cst.val[0], cst.val[1], cst.val[2], cst.val[3]);
	while (++i < 3)
		if (check_moebius_bnd(l, moebius_origin, roots.val[i], moebius))
			check |= is_closest_intersect(dist, roots.val[i]);
	return (check);
}
