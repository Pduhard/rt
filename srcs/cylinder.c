/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cylinder.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aplat <aplat@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/15 17:45:12 by aplat             #+#    #+#             */
/*   Updated: 2020/05/15 17:53:52 by aplat            ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

/*
** http://www.illusioncatalyst.com/notes_files/mathematics/
**	line_cylinder_intersection.php
*/

t_3vecf	get_cylinder_quadratic_cst(t_cylinder *cylinder_param,
	t_3vecf cylinder_origin, t_3vecf cylinder_tip, t_leq l)
{
	t_3vecf	h;
	double	dp_dh;
	double	dp_wh;
	t_3vecf	w;
	t_3vecf cst;

	h = sub_3vecf(cylinder_origin, cylinder_tip);
	normalize_3vecf(&h);
	w = sub_3vecf(l.orig, cylinder_origin);
	dp_dh = dot_product_3vecf(l.dir, h);
	dp_wh = dot_product_3vecf(w, h);
	cst.val[0] = dot_product_3vecf(l.dir, l.dir) - dp_dh * dp_dh;
	cst.val[1] = 2 * (dot_product_3vecf(l.dir, w) - dp_dh * dp_wh);
	cst.val[2] = dot_product_3vecf(w, w) - dp_wh * dp_wh
		- cylinder_param->radius * cylinder_param->radius;
	return (cst);
}

int		ray_intersect_cylinder(t_leq l, t_obj *cylinder, t_dist dist, int sp_id)
{
	t_3vecf		cst;
	int			check;
	t_cylinder	*cylinder_param;
	t_2vecf		roots;

	check = 0;
	cylinder_param = (t_cylinder *)cylinder->obj_param;
	cst = get_cylinder_quadratic_cst(
		cylinder_param,
		get_cylinder_origin(cylinder, cylinder->obj_param, sp_id),
		get_cylinder_tip(cylinder, cylinder->obj_param, sp_id),
		l);
	roots = solve_quadratic(cst.val[0], cst.val[1], cst.val[2]);
	check |= is_closest_intersect(dist, roots.val[0]);
	check |= is_closest_intersect(dist, roots.val[1]);
	return (check);
}
