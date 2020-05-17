/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hyperboloid.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aplat <aplat@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/15 18:57:38 by aplat             #+#    #+#             */
/*   Updated: 2020/05/15 19:03:21 by aplat            ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

t_3vecf	get_hyperboloid_quadratic_cst(t_hyperboloid *param,
	t_3vecf hyperboloid_origin, t_leq l)
{
	double		f[3];
	double		o[3];
	double		d[3];
	t_3vecf		cst;

	o[0] = l.orig.val[0] - hyperboloid_origin.val[0];
	o[1] = l.orig.val[1] - hyperboloid_origin.val[1];
	o[2] = l.orig.val[2] - hyperboloid_origin.val[2];
	d[0] = l.dir.val[0];
	d[1] = l.dir.val[1];
	d[2] = l.dir.val[2];
	f[0] = param->x_fact;
	f[1] = param->y_fact;
	f[2] = param->z_fact;
	cst.val[2] = (o[0] * o[0]) / (f[0] * f[0]) - (o[1] * o[1]) / (f[1] * f[1])
		+ (o[2] * o[2]) / (f[2] * f[2]) + param->surface;
	cst.val[1] = (2 * o[0] * d[0]) / (f[0] * f[0]) - (2 * o[1] * d[1])
		/ (f[1] * f[1]) + (2 * o[2] * d[2]) / (f[2] * f[2]);
	cst.val[0] = (d[0] * d[0]) / (f[0] * f[0]) - (d[1] * d[1]) / (f[1] * f[1])
		+ (d[2] * d[2]) / (f[2] * f[2]);
	return (cst);
}

int		ray_intersect_hyperboloid(t_leq l, t_obj *hyperboloid,
	t_dist dist, int sp_id)
{
	t_hyperboloid	*param;
	t_2vecf			roots;
	int				check;
	t_3vecf			hyperboloid_origin;
	t_3vecf			cst;

	check = 0;
	param = (t_hyperboloid *)hyperboloid->obj_param;
	hyperboloid_origin = get_hyperboloid_origin(hyperboloid, param, sp_id);
	cst = get_hyperboloid_quadratic_cst(param, hyperboloid_origin, l);
	roots = solve_quadratic(cst.val[0], cst.val[1], cst.val[2]);
	check |= is_closest_intersect(dist, roots.val[0]);
	check |= is_closest_intersect(dist, roots.val[1]);
	return (check);
}
