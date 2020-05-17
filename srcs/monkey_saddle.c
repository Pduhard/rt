/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monkey_saddle.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aplat <aplat@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/26 02:40:58 by pduhard-          #+#    #+#             */
/*   Updated: 2020/05/15 19:51:03 by aplat            ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

t_4vecf	get_monkey_saddle_cubis_cst(t_3vecf monkey_saddle_origin, t_leq l)
{
	double		o[3];
	double		d[3];
	t_4vecf		cst;

	o[0] = l.orig.val[0] - monkey_saddle_origin.val[0];
	o[1] = l.orig.val[1] - monkey_saddle_origin.val[1];
	o[2] = l.orig.val[2] - monkey_saddle_origin.val[2];
	d[0] = l.dir.val[0];
	d[1] = l.dir.val[1];
	d[2] = l.dir.val[2];
	cst.val[0] = (-3 * d[0] * d[2] * d[2]) + (d[0] * d[0] * d[0]);
	cst.val[1] = (-3 * o[0] * d[2] * d[2]) - (6 * o[2] * d[2] * d[0])
		+ (d[0] * d[0] * o[0]) + (2 * o[0] * d[0] * d[0]);
	cst.val[2] = (-6 * o[0] * o[2] * d[2]) - (3 * o[2] * o[2] * d[0]) - d[1]
		+ (2 * o[0] * o[0] * d[0]) + (o[0] * o[0] * d[0]);
	cst.val[3] = (-3 * o[2] * o[2] * o[0]) + (o[0] * o[0] * o[0]) - o[1];
	return (cst);
}

int		check_monkey_saddle_bnd(t_leq l, t_3vecf monkey_saddle_origin,
	double root)
{
	double	x;
	double	y;
	double	z;

	x = l.orig.val[0] - monkey_saddle_origin.val[0] + l.dir.val[0] * root;
	y = l.orig.val[1] - monkey_saddle_origin.val[1] + l.dir.val[1] * root;
	z = l.orig.val[2] - monkey_saddle_origin.val[2] + l.dir.val[2] * root;
	if (x > -1 && x < 1 && y > -1 && y < 1 && z > -1 && z < 1)
		return (1);
	return (0);
}

int		ray_intersect_monkey_saddle(t_leq l, t_obj *monkey_saddle,
	t_dist dist, int sp_id)
{
	t_4vecf	cst;
	t_3vecf	roots;
	int		check;
	t_3vecf	monkey_saddle_origin;
	int		i;

	i = -1;
	check = 0;
	monkey_saddle_origin = get_monkey_saddle_origin(monkey_saddle,
		(t_monkey_saddle *)monkey_saddle->obj_param, sp_id);
	cst = get_monkey_saddle_cubis_cst(monkey_saddle_origin, l);
	roots = solve_cubic(cst.val[0], cst.val[1], cst.val[2], cst.val[3]);
	while (++i < 3)
		if (check_monkey_saddle_bnd(l, monkey_saddle_origin, roots.val[i]))
			check |= is_closest_intersect(dist, roots.val[i]);
	return (check);
}
