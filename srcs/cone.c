/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cone.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aplat <aplat@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/15 17:18:53 by aplat             #+#    #+#             */
/*   Updated: 2020/05/15 17:26:23 by aplat            ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

t_3vecf	get_cone_quadratic_cst(t_cone *cone_param, t_3vecf cone_origin,
	t_3vecf cone_tip, t_leq l)
{
	double	m;
	t_3vecf	h;
	double	dp_dh;
	double	dp_wh;
	t_3vecf	w;

	h = sub_3vecf(cone_origin, cone_tip);
	m = (cone_param->radius * cone_param->radius) /
		(get_length_3vecf(h) * get_length_3vecf(h));
	normalize_3vecf(&h);
	w = sub_3vecf(l.orig, cone_tip);
	dp_dh = dot_product_3vecf(l.dir, h);
	dp_wh = dot_product_3vecf(w, h);
	return ((t_3vecf){{
			dot_product_3vecf(l.dir, l.dir) - m * dp_dh * dp_dh - dp_dh * dp_dh,
			2 * (dot_product_3vecf(l.dir, w) - m * dp_dh
				* dp_wh - dp_dh * dp_wh),
			dot_product_3vecf(w, w) - m * dp_wh * dp_wh - dp_wh * dp_wh}});
}

int		ray_intersect_cone(t_leq l, t_obj *cone, t_dist dist, int sp_id)
{
	t_3vecf cst;
	int		check;
	t_cone	*cone_param;
	t_2vecf roots;

	check = 0;
	cone_param = (t_cone *)cone->obj_param;
	cst = get_cone_quadratic_cst(
					cone_param,
					get_cone_origin(cone, cone_param, sp_id),
					get_cone_tip(cone, cone_param, sp_id),
					l);
	roots = solve_quadratic(cst.val[0], cst.val[1], cst.val[2]);
	check |= is_closest_intersect(dist, roots.val[0]);
	check |= is_closest_intersect(dist, roots.val[1]);
	return (check);
}
