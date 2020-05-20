/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_marble_color.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aplat <aplat@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/18 01:35:04 by aplat             #+#    #+#             */
/*   Updated: 2020/05/19 21:28:20 by aplat            ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

static double	marble_noise(t_3vecf inter_point, double scale,
	const unsigned char permutation[512], const t_3vecf gradient[16])
{
	double	t;

	t = 0;
	while (scale <= ROUGHCAST_LIMIT)
	{
		t += fabs(compute_3dperlin_factor(inter_point, scale,
			permutation, gradient) / scale);
		scale *= 2;
	}
	return (t);
}

double			compute_marble_factor(t_3vecf inter_point, double scale,
	const unsigned char permutation[512], const t_3vecf gradient[16])
{
	double t;

	t = (0.5 + 0.5 * sin(scale * 2 * M_PI * (inter_point.val[0] + 2
		* marble_noise(inter_point, scale, permutation, gradient))));
	return (t * t - .5);
}

t_4vecf			get_marble_color(t_3vecf inter_point, t_3vecf normal_inter,
	t_obj *obj)
{
	double		marble_f;
	t_4vecf		color;
	t_text_proc	*text;

	text = (t_text_proc *)obj->text.text_param;
	marble_f = compute_marble_factor(inter_point, obj->text.scale.val[0],
		((t_data *)obj->data)->permutation, ((t_data *)obj->data)->gradient);
	color.val[0] = linear_interpolate(text->color[0].val[0],
		text->color[1].val[0], marble_f);
	color.val[1] = linear_interpolate(text->color[0].val[1],
		text->color[1].val[1], marble_f);
	color.val[2] = linear_interpolate(text->color[0].val[2],
		text->color[1].val[2], marble_f);
	color.val[3] = linear_interpolate(text->color[0].val[3],
		text->color[1].val[3], marble_f);
	return (color);
	(void)normal_inter;
}
