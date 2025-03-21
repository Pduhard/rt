/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   polynomial.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aplat <aplat@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/15 20:55:07 by aplat             #+#    #+#             */
/*   Updated: 2020/05/15 20:57:04 by aplat            ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

static t_3vecf	solve_no_cube(double a, double b, double c)
{
	t_2vecf	quadra_roots;
	t_3vecf	roots;

	quadra_roots = solve_quadratic(a, b, c);
	roots.val[0] = quadra_roots.val[0];
	roots.val[1] = quadra_roots.val[1];
	roots.val[2] = quadra_roots.val[0];
	return (roots);
}

static t_3vecf	solve_one_root(double r, double delta, double b)
{
	double	s;
	double	t;
	t_3vecf	roots;

	s = r + sqrt(delta);
	s = s < 0 ? -powf(-s, (1. / 3.)) : powf(s, (1. / 3.));
	t = r - sqrt(delta);
	t = t < 0 ? -powf(-t, (1. / 3.)) : powf(t, (1. / 3.));
	roots.val[0] = -b + s + t;
	roots.val[1] = roots.val[0];
	roots.val[2] = roots.val[0];
	return (roots);
}

static t_3vecf	solve_three_roots(double r, double q, double b)
{
	double	q3;
	t_3vecf	roots;
	double	r13;

	if (q < 0)
		q = -q;
	r13 = 2 * sqrt(q);
	q3 = q * q * q;
	q3 = acos(r / sqrt(q3));
	roots.val[0] = -b + r13 * cos(q3 / 3.);
	roots.val[1] = -b + r13 * cos((q3 + (2. * M_PI)) / 3.);
	roots.val[2] = -b + r13 * cos((q3 + (4. * M_PI)) / 3.);
	return (roots);
}

static t_3vecf	solve_two_roots(double r, double b)
{
	double	r13;
	t_3vecf	roots;

	if (r < 0)
		r13 = -powf(-r, 1. / 3.);
	else
		r13 = powf(r, 1. / 3.);
	roots.val[0] = -b + 2. * r13;
	roots.val[1] = -r13 - b;
	roots.val[2] = roots.val[1];
	return (roots);
}

t_3vecf			solve_cubic(double a, double b, double c, double d)
{
	double	q;
	double	r;
	double	delta;

	if (is_null(a))
		return (solve_no_cube(b, c, d));
	if (is_null(d))
		return (solve_no_cube(a, b, c));
	b /= a;
	c /= a;
	d /= a;
	q = (3. * c - b * b) / 9.;
	r = ((-27. * d) + b * (9. * c - 2. * b * b)) / 54.;
	delta = q * q * q + r * r;
	if (delta > 0)
		return (solve_one_root(r, delta, b / 3.));
	else if (delta < 0)
		return (solve_three_roots(r, q, b / 3.));
	return (solve_two_roots(r, b / 3.));
}
