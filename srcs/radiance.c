/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   radiance.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aplat <aplat@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/18 05:12:51 by aplat             #+#    #+#             */
/*   Updated: 2020/05/18 05:14:02 by aplat            ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

t_3vecf		compute_radiance_estimation(
	t_photon **nearest_n, t_3vecf inter_point, double farest, int nn_photon)
{
	t_3vecf	radiance;
	int		i;
	double	k;
	double	dist;
	double	cone_f;

	k = 1.05;
	i = -1;
	radiance = assign_3vecf(0, 0, 0);
	while (++i < nn_photon)
		if (nearest_n[i])
		{
			dist = get_length_3vecf(sub_3vecf(inter_point,
				nearest_n[i]->position));
			cone_f = 1 - dist / (k * farest);
			radiance = add_3vecf(radiance,
				product_c3vecf(nearest_n[i]->color, cone_f));
		}
	radiance.val[0] /= M_PI * farest * farest * (1. - 2. / (3. * k));
	radiance.val[1] /= M_PI * farest * farest * (1. - 2. / (3. * k));
	radiance.val[2] /= M_PI * farest * farest * (1. - 2. / (3. * k));
	return (radiance);
}
