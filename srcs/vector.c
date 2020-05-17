/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aplat <aplat@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/15 17:09:55 by aplat             #+#    #+#             */
/*   Updated: 2020/05/15 17:10:00 by aplat            ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

void	normalize_3vecf(t_3vecf *vec)
{
	double	length;

	length = get_length_3vecf(*vec);
	vec->val[0] = vec->val[0] / length;
	vec->val[1] = vec->val[1] / length;
	vec->val[2] = vec->val[2] / length;
}

t_3vecf	add_c3vecf(t_3vecf a, double c)
{
	t_3vecf	vec;

	vec.val[0] = a.val[0] + c;
	vec.val[1] = a.val[1] + c;
	vec.val[2] = a.val[2] + c;
	return (vec);
}

t_3vecf	assign_3vecf(double x, double y, double z)
{
	t_3vecf	vec;

	vec.val[0] = x;
	vec.val[1] = y;
	vec.val[2] = z;
	return (vec);
}


t_3vecf	product_c3vecf(t_3vecf a, double c)
{
	t_3vecf	vec;

	vec.val[0] = a.val[0] * c;
	vec.val[1] = a.val[1] * c;
	vec.val[2] = a.val[2] * c;
	return (vec);
}

t_3vecf	neg_3vecf(t_3vecf vec)
{
	return ((t_3vecf){{-vec.val[0], -vec.val[1], -vec.val[2]}});
}
