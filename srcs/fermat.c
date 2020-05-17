/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fermat.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aplat <aplat@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/15 18:12:08 by aplat             #+#    #+#             */
/*   Updated: 2020/05/15 18:19:46 by aplat            ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

t_4vecf	get_fermat_cubic_cst(t_leq l, t_3vecf fermat_origin)
{
	double		o[3];
	double		d[3];
	t_4vecf		cst;

	o[0] = l.orig.val[0] - fermat_origin.val[0];
	o[1] = l.orig.val[1] - fermat_origin.val[1];
	o[2] = l.orig.val[2] - fermat_origin.val[2];
	d[0] = l.dir.val[0];
	d[1] = l.dir.val[1];
	d[2] = l.dir.val[2];
	cst.val[0] = d[2] * d[2] * d[2] + d[0] * d[0] * d[0] + d[1]
		* d[1] * d[1];
	cst.val[1] = 3 * o[2] * d[2] * d[2] + 3 * o[0] * d[0] * d[0]
		+ 3 * o[1] * d[1] * d[1];
	cst.val[2] = 3 * o[2] * o[2] * d[2] + 3 * o[1] * o[1] * d[1]
		+ 3 * o[0] * o[0] * d[0];
	cst.val[3] = o[2] * o[2] * o[2] + o[0] * o[0] * o[0] + o[1]
		* o[1] * o[1] - 1;
	return (cst);
}

int		check_fermat_bnd(t_leq l, t_3vecf fermat_origin, double root)
{
	double	x;
	double	y;
	double	z;

	x = l.orig.val[0] - fermat_origin.val[0] + l.dir.val[0] * root;
	y = l.orig.val[1] - fermat_origin.val[1] + l.dir.val[1] * root;
	z = l.orig.val[2] - fermat_origin.val[2] + l.dir.val[2] * root;
	if (x > -3 && x < 3 && y > -3 && y < 3 && z > -3 && z < 3)
		return (1);
	return (0);
}

int		ray_intersect_fermat(t_leq l, t_obj *fermat, t_dist dist, int sp_id)
{
	t_4vecf		cst;
	t_3vecf		roots;
	int			check;
	t_3vecf		fermat_origin;
	int			i;

	i = -1;
	check = 0;
	fermat_origin = get_fermat_origin(fermat, (t_fermat *)fermat->obj_param,
		sp_id);
	cst = get_fermat_cubic_cst(l, fermat_origin);
	roots = solve_cubic(cst.val[0], cst.val[1], cst.val[2], cst.val[3]);
	while (++i < 3)
		if (check_fermat_bnd(l, fermat_origin, roots.val[i]))
			check |= is_closest_intersect(dist, roots.val[i]);
	return (check);
}
