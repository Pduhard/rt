/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fbm.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pduhard- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/27 01:02:05 by pduhard-          #+#    #+#             */
/*   Updated: 2020/02/27 06:46:45 by pduhard-         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

double	fbm_rand(double p)
{
	p *= 0.010101;
	p = p - (int)p;
	p *= p + 7.5;
	p*= p + p;
	return (p - (int)p);
}

double	get_fbm_noise(t_3vecf point)
{
	t_3vecf	step;
	t_3vecf int_part;
	t_3vecf	floor_part;
	t_3vecf	u;
	double	n;

	step = assign_3vecf(110, 241, 171);
	int_part.val[0] = (int)point.val[0];
	int_part.val[1] = (int)point.val[1];
	int_part.val[2] = (int)point.val[2];

	if (point.val[0] < 0)
		int_part.val[0] -= 1;
	if (point.val[1] < 0)
		int_part.val[1] -= 1;
	if (point.val[2] < 0)
		int_part.val[2] -= 1;

	floor_part.val[0] = point.val[0] - int_part.val[0];
	floor_part.val[1] = point.val[1] - int_part.val[1];
	floor_part.val[2] = point.val[2] - int_part.val[2];
	n = dot_product_3vecf(int_part, step);
	u.val[0] = floor_part.val[0] * floor_part.val[0] * (3. - 2. * floor_part.val[0]);
	u.val[1] = floor_part.val[1] * floor_part.val[1] * (3. - 2. * floor_part.val[1]);
	u.val[2] = floor_part.val[2] * floor_part.val[2] * (3. - 2. * floor_part.val[2]);
	
	double	l1 = linear_interpolate(fbm_rand(n), fbm_rand(n + 110), u.val[0]);
	double	l2 = linear_interpolate(fbm_rand(n + 241), fbm_rand(n + 351), u.val[0]);
	
	double	l3 = linear_interpolate(fbm_rand(n + 171), fbm_rand(n + 281), u.val[0]);
	double	l4 = linear_interpolate(fbm_rand(n + 412), fbm_rand(n + 522), u.val[0]);


	
/*	double	l1 = linear_interpolate(fbm_rand(n + dot_product_3vecf(step, assign_3vecf(0, 0, 0))), fbm_rand(n + dot_product_3vecf(step, assign_3vecf(1, 0, 0))), u.val[0]);
	double	l2 = linear_interpolate(fbm_rand(n + dot_product_3vecf(step, assign_3vecf(0, 1, 0))), fbm_rand(n + dot_product_3vecf(step, assign_3vecf(1, 1, 0))), u.val[0]);
	
	double	l3 = linear_interpolate(fbm_rand(n + dot_product_3vecf(step, assign_3vecf(0, 0, 1))), fbm_rand(n + dot_product_3vecf(step, assign_3vecf(1, 0, 1))), u.val[0]);
	double	l4 = linear_interpolate(fbm_rand(n + dot_product_3vecf(step, assign_3vecf(0, 1, 1))), fbm_rand(n + dot_product_3vecf(step, assign_3vecf(1, 1, 1))), u.val[0]);
*/

	double	l5 = linear_interpolate(l1, l2, u.val[1]);
	double	l6 = linear_interpolate(l3, l4, u.val[1]);

	return (linear_interpolate(l5, l6, u.val[2]));
}

double	compute_3dfbm_factor(t_3vecf point, double scale)
{
	double	fact;
	double	ampl;
	double	shift;
	int		iter;

	shift = 100.;
	ampl = 0.5;
	fact = 0.;
	iter = 0;
	point.val[0] *= scale;
	point.val[1] *= scale;
	point.val[2] *= scale;
/*	point.val[2] *= scale;
	point.val[2] *= scale;
*/	/*if (point.val[0] < 0)
		point.val[0] -= 1;
	if (point.val[1] < 0)
		point.val[1] -= 1;
	if (point.val[2] < 0)
		point.val[2] -= 1;
	*/
	while (iter < FBM_ITER)
	{
		fact += ampl * get_fbm_noise(point);
		point.val[0] = point.val[0] * FBM_LACUNARITY + shift;
		point.val[1] = point.val[1] * FBM_LACUNARITY + shift;
		point.val[2] = point.val[2] * FBM_LACUNARITY + shift;
		ampl *= FBM_GAIN;
		iter++;
	}
	return (fact);
	(void)scale;
}
