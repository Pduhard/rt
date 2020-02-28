/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   vector.c                                         .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: pduhard- <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2019/12/21 23:02:40 by pduhard-     #+#   ##    ##    #+#       */
/*   Updated: 2020/02/28 03:51:40 by pduhard-         ###   ########lyon.fr   */
/*                                                         /                  */
/*                                                        /                   */
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
	//sqrtf(vec->val[0] * vec->val[0] + vec->val[1] * vec->val[1] + vec->val[2] * vec->val[2]);
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

