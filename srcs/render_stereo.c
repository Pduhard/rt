/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_stereo.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aplat <aplat@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/18 05:42:23 by aplat             #+#    #+#             */
/*   Updated: 2020/05/19 19:21:45 by aplat            ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

static t_3vecf	get_stereo_clr(t_3vecf origs[2], t_leq l, t_data *data)
{
	t_3vecf		colors[2];

	if (!data->motion_blur)
	{
		colors[0] = ray_trace((t_leq){origs[0], l.dir}, data, RAY_DEPTH, 0);
		colors[1] = ray_trace((t_leq){origs[1], l.dir}, data, RAY_DEPTH, 0);
	}
	else
	{
		colors[0] = motion_trace(origs[0], l.dir, data);
		colors[1] = motion_trace(origs[1], l.dir, data);
	}
	return (assign_3vecf(colors[1].val[0], colors[0].val[1], colors[0].val[2]));
}

void			compute_stereoscopy(t_data *data, t_leq l, int i, int j)
{
	t_3vecf		origs[2];
	t_3vecf		diff;
	t_3vecf		color;
	t_anti_al	a;

	a = init_anti_al(data->aa_adapt, 0, data->aa_adapt * data->aa_adapt);
	diff = mult_3vecf_33matf(assign_3vecf(0.1, 0, 0), data->rot_mat[1]);
	origs[0] = add_3vecf(l.orig, diff);
	origs[1] = add_3vecf(l.orig, neg_3vecf(diff));
	color = assign_3vecf(0, 0, 0);
	while (a.offset < a.anti_al_iter)
	{
		l.dir = init_ray_dir(i, j, a, data);
		color = add_3vecf(color, get_stereo_clr(origs, l, data));
		a.offset++;
	}
	color = product_c3vecf(color, 1. / a.anti_al_iter);
	ray_put_pixel((t_pixel){i, j}, data->mlx->img_str, color, data);
	check_subsampling(data, i, j, color);
}
