/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quadratic_poly.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aplat <aplat@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/18 05:12:35 by aplat             #+#    #+#             */
/*   Updated: 2020/05/18 05:12:35 by aplat            ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

t_2vecf			solve_quadratic(double a, double b, double c)
{
	double	delta;
	t_2vecf	roots;

	if (is_null(a))
	{
		roots.val[0] = (-c / b);
		roots.val[1] = (-c / b);
		return (roots);
	}
	delta = b * b - 4 * a * c;
	if (delta < 0)
	{
		roots.val[0] = 0;
		roots.val[1] = 0;
		return (roots);
	}
	roots.val[0] = (-b - sqrt(delta)) / (2 * a);
	roots.val[1] = (-b + sqrt(delta)) / (2 * a);
	return (roots);
}
