/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   perlin_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aplat <aplat@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/15 20:42:45 by aplat             #+#    #+#             */
/*   Updated: 2020/05/15 20:43:53 by aplat            ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

static double	quintic_poly(float val)
{
	return (val * val * val * (val * (val * 6.0 - 15.0) + 10.0));
}

void			set_quintic_factors(double q[3], t_3vecf f)
{
	q[2] = quintic_poly(f.val[0]);
	q[1] = quintic_poly(f.val[1]);
	q[0] = quintic_poly(f.val[2]);
}

void			handle_perlin_inter_point(t_3vecf *int_part,
	t_3vecf *floating_part, t_3vecf inter_point, double scale)
{
	inter_point.val[0] *= scale;
	inter_point.val[1] *= scale;
	inter_point.val[2] *= scale;
	*int_part = assign_3vecf((int)inter_point.val[0], (int)inter_point.val[1],
		(int)inter_point.val[2]);
	if (inter_point.val[0] < 0)
		int_part->val[0] -= 1;
	if (inter_point.val[1] < 0)
		int_part->val[1] -= 1;
	if (inter_point.val[2] < 0)
		int_part->val[2] -= 1;
	*floating_part = assign_3vecf(inter_point.val[0] - int_part->val[0],
		inter_point.val[1] - int_part->val[1], inter_point.val[2]
			- int_part->val[2]);
	int_part->val[0] = (int)int_part->val[0] & 255;
	int_part->val[1] = (int)int_part->val[1] & 255;
	int_part->val[2] = (int)int_part->val[2] & 255;
}
