/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sphere.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aplat <aplat@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/15 20:58:25 by aplat             #+#    #+#             */
/*   Updated: 2020/05/15 21:00:31 by aplat            ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

t_3vecf	get_sphere_quadratic_cst(t_sphere *sphere_param, t_3vecf sphere_origin,
	t_leq l)
{
	t_3vecf	dist_vec;
	t_3vecf cst;

	dist_vec = sub_3vecf(l.orig, sphere_origin);
	cst.val[0] = dot_product_3vecf(l.dir, l.dir);
	cst.val[1] = 2.f * dot_product_3vecf(dist_vec, l.dir);
	cst.val[2] = dot_product_3vecf(dist_vec, dist_vec)
		- sphere_param->radius * sphere_param->radius;
	return (cst);
}

int		ray_intersect_sphere(t_leq l, t_obj *sphere, t_dist dist, int sp_id)
{
	t_2vecf		roots;
	t_3vecf		cst;
	t_sphere	*sphere_param;
	int			check;
	t_3vecf		sphere_origin;

	check = 0;
	sphere_param = (t_sphere *)sphere->obj_param;
	sphere_origin = get_sphere_origin(sphere, sphere_param, sp_id);
	cst = get_sphere_quadratic_cst(sphere_param, sphere_origin, l);
	roots = solve_quadratic(cst.val[0], cst.val[1], cst.val[2]);
	check |= is_closest_intersect(dist, roots.val[0]);
	check |= is_closest_intersect(dist, roots.val[1]);
	return (check);
}
