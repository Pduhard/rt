/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_classic.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aplat <aplat@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/18 05:30:06 by aplat             #+#    #+#             */
/*   Updated: 2020/05/18 05:30:39 by aplat            ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

void	compute_classic(t_data *data, t_leq l, int i, int j)
{
	t_3vecf		clr;
	t_anti_al	a;
	t_3vecf		color;

	a = init_anti_al(data->aa_adapt, 0, data->aa_adapt * data->aa_adapt);
	color = assign_3vecf(0, 0, 0);
	while (a.offset < a.anti_al_iter)
	{
		l.dir = init_ray_dir(i, j, a, data);
		clr = !data->motion_blur ? ray_trace(l, data, RAY_DEPTH, 0)
									: motion_trace(l.orig, l.dir, data);
		color = add_3vecf(color, clr);
		a.offset++;
	}
	color = product_c3vecf(color, 1. / a.anti_al_iter);
	ray_put_pixel(i, j, data->mlx->img_str, color, data);
	check_subsampling(data, i, j, color);
}
