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

int		is_null_3vecf(t_3vecf vec)
{
	if (is_null(vec.val[0]) && is_null(vec.val[1]) && is_null(vec.val[2]))
		return (1);
	return (0);
}

double	dot_product_3vecf(t_3vecf a, t_3vecf b)
{
	return (a.val[0] * b.val[0] + a.val[1] * b.val[1] + a.val[2] * b.val[2]);
}

double	dot_product_2vecf(t_2vecf a, t_2vecf b)
{
	return (a.val[0] * b.val[0] + a.val[1] * b.val[1]);
}

double	get_length_3vecf(t_3vecf vec)
{
	return (sqrtf(dot_product_3vecf(vec, vec)));
}

void	normalize_3vecf(t_3vecf *vec)
{
	double	length;

	length = get_length_3vecf(*vec);
	vec->val[0] = vec->val[0] / length;
	vec->val[1] = vec->val[1] / length;
	vec->val[2] = vec->val[2] / length;
}

t_3vecf	sub_3vecf(t_3vecf a, t_3vecf b)
{
	t_3vecf	vec;

	vec.val[0] = a.val[0] - b.val[0];
	vec.val[1] = a.val[1] - b.val[1];
	vec.val[2] = a.val[2] - b.val[2];
	return (vec);
}

t_3vecf	add_c3vecf(t_3vecf a, double c)
{
	t_3vecf	vec;

	vec.val[0] = a.val[0] + c;
	vec.val[1] = a.val[1] + c;
	vec.val[2] = a.val[2] + c;
	return (vec);
}

t_3vecf	add_3vecf(t_3vecf a, t_3vecf b)
{
	t_3vecf	vec;

	vec.val[0] = a.val[0] + b.val[0];
	vec.val[1] = a.val[1] + b.val[1];
	vec.val[2] = a.val[2] + b.val[2];
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

t_2vecf	assign_2vecf(double x, double y)
{
	t_2vecf	vec;

	vec.val[0] = x;
	vec.val[1] = y;
	return (vec);
}

t_3vecf	product_3vecf(t_3vecf a, t_3vecf b)
{
	t_3vecf	vec;

	vec.val[0] = (a.val[1] * b.val[2] - a.val[2] * b.val[1]);
	vec.val[1] = (a.val[2] * b.val[0] - a.val[0] * b.val[2]);
	vec.val[2] = (a.val[0] * b.val[1] - a.val[1] * b.val[0]);
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
