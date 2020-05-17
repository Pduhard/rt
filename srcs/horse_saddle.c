/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   horse_saddle.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aplat <aplat@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/15 18:52:21 by aplat             #+#    #+#             */
/*   Updated: 2020/05/15 18:57:06 by aplat            ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

t_3vecf	get_horse_saddle_quadratic_cst(t_horse_saddle *param,
	t_3vecf horse_saddle_origin, t_leq l)
{
	t_3vecf		cst;
	double		o[3];
	double		d[3];
	double		a;
	double		b;

	o[0] = l.orig.val[0] - horse_saddle_origin.val[0];
	o[1] = l.orig.val[1] - horse_saddle_origin.val[1];
	o[2] = l.orig.val[2] - horse_saddle_origin.val[2];
	d[0] = l.dir.val[0];
	d[1] = l.dir.val[1];
	d[2] = l.dir.val[2];
	a = param->x_fact;
	b = param->y_fact;
	cst.val[2] = (o[0] * o[0]) / (a * a) - (o[2] * o[2]) / (b * b) - o[1];
	cst.val[1] = (2 * o[0] * d[0]) / (a * a) - (2 * o[2] * d[2])
		/ (b * b) - d[1];
	cst.val[0] = (d[0] * d[0]) / (a * a) - (d[2] * d[2]) / (b * b);
	return (cst);
}

int		check_horse_saddle_bnd(t_leq l, t_3vecf horse_saddle_origin,
	double root, t_horse_saddle *param)
{
	double	x;
	double	y;
	double	z;

	x = l.orig.val[0] - horse_saddle_origin.val[0] + l.dir.val[0] * root;
	y = l.orig.val[1] - horse_saddle_origin.val[1] + l.dir.val[1] * root;
	z = l.orig.val[2] - horse_saddle_origin.val[2] + l.dir.val[2] * root;
	if (x > -param->x_fact && x < param->x_fact && z > -param->y_fact
		&& z < param->y_fact)
		return (1);
	return (0);
}

int		ray_intersect_horse_saddle(t_leq l, t_obj *horse_saddle,
	t_dist dist, int sp_id)
{
	t_horse_saddle	*param;
	t_3vecf			cst;
	t_2vecf			roots;
	int				check;
	t_3vecf			horse_saddle_origin;

	check = 0;
	param = (t_horse_saddle *)horse_saddle->obj_param;
	horse_saddle_origin = get_horse_saddle_origin(horse_saddle, param, sp_id);
	cst = get_horse_saddle_quadratic_cst(param, horse_saddle_origin, l);
	roots = solve_quadratic(cst.val[0], cst.val[1], cst.val[2]);
	if (check_horse_saddle_bnd(l, horse_saddle_origin, roots.val[0], param))
		check |= is_closest_intersect(dist, roots.val[0]);
	if (check_horse_saddle_bnd(l, horse_saddle_origin, roots.val[1], param))
		check |= is_closest_intersect(dist, roots.val[1]);
	return (check);
}
