/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cyclide.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aplat <aplat@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/15 17:40:20 by aplat             #+#    #+#             */
/*   Updated: 2020/05/15 17:44:39 by aplat            ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

t_4vecf	get_cyclide_cubic_cst(t_cyclide *param, t_leq l,
	t_3vecf cyclide_origin)
{
	t_4vecf cst;
	double	o[3];
	double	d[3];
	double	a;

	a = param->param;
	o[0] = l.orig.val[0] - cyclide_origin.val[0];
	o[1] = l.orig.val[1] - cyclide_origin.val[1];
	o[2] = l.orig.val[2] - cyclide_origin.val[2];
	d[0] = l.dir.val[0];
	d[1] = l.dir.val[1];
	d[2] = l.dir.val[2];
	cst.val[0] = (d[0] * d[0] * d[2]) + (d[1] * d[1] * d[2])
		+ (d[2] * d[2] * d[2]);
	cst.val[1] = (o[2] * d[0] * d[0]) + (2 * o[0] * d[0] * d[2])
		+ (o[2] * d[1] * d[1]) + (2 * o[1] * d[2] * d[1])
			+ (3 * o[2] * d[2] * d[2]) + (a * d[0] * d[0]) - (a * d[1] * d[1]);
	cst.val[2] = (2 * o[0] * o[2] * d[0]) + (o[0] * o[0] * d[2])
		+ (2 * o[1] * o[2] * d[1]) + (o[1] * o[1] * d[2])
			+ (3 * o[2] * o[2] * d[2]) - (a * a * d[2]) + (2 * a * o[0] * d[0])
				- (2 * a * o[1] * d[1]);
	cst.val[3] = (o[0] * o[0] * o[2]) + (o[1] * o[1] * o[2])
		+ (o[2] * o[2] * o[2]) - (a * a * o[2]) + (a * o[0] * o[0])
			- (a * o[1] * o[1]);
	return (cst);
}

int		check_cyclide_bnd(t_leq l, t_3vecf cyclide_origin, double root)
{
	double	x;
	double	y;
	double	z;

	x = l.orig.val[0] - cyclide_origin.val[0] + l.dir.val[0] * root;
	y = l.orig.val[1] - cyclide_origin.val[1] + l.dir.val[1] * root;
	z = l.orig.val[2] - cyclide_origin.val[2] + l.dir.val[2] * root;
	if (x > -1.5 && x < 1.5 && y > -2 && y < 2 && z > -1 && z < 1)
		return (1);
	return (0);
}

int		ray_intersect_cyclide(t_leq l, t_obj *cyclide, t_dist dist, int sp_id)
{
	t_4vecf		cst;
	t_3vecf		roots;
	int			check;
	t_3vecf		cyclide_origin;
	int			i;

	i = -1;
	check = 0;
	cyclide_origin = get_cyclide_origin(
		cyclide,
		(t_cyclide *)cyclide->obj_param,
		sp_id);
	cst = get_cyclide_cubic_cst((t_cyclide *)cyclide->obj_param,
		l, cyclide_origin);
	roots = solve_cubic(cst.val[0], cst.val[1], cst.val[2], cst.val[3]);
	while (++i < 3)
		if (check_cyclide_bnd(l, cyclide_origin, roots.val[i]))
			check |= is_closest_intersect(dist, roots.val[i]);
	return (check);
}
