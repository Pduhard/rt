/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_wood_color.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aplat <aplat@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/18 01:55:24 by aplat             #+#    #+#             */
/*   Updated: 2020/05/18 01:55:25 by aplat            ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

double	compute_wood_factor(t_3vecf inter_point, double scale,
	const unsigned char permutation[512], const t_3vecf gradient[16])
{
	double	perlin_f;
	double	wood_f;

	perlin_f = compute_3dperlin_factor(inter_point,
		scale, permutation, gradient);
	wood_f = (1. + sin((perlin_f / 2.) * 150.)) / 2.;
	return (wood_f);
}

t_4vecf	get_wood_color(t_3vecf inter_point, t_3vecf normal_inter, t_obj *obj)
{
	double		wood_f;
	t_4vecf		color;
	t_text_proc	*text;

	text = (t_text_proc *)obj->text.text_param;
	wood_f = compute_wood_factor(inter_point, obj->text.scale.val[0],
		obj->data->permutation, obj->data->gradient);
	color.val[0] = linear_interpolate(text->color[0].val[0],
		text->color[1].val[0], wood_f);
	color.val[1] = linear_interpolate(text->color[0].val[1],
		text->color[1].val[1], wood_f);
	color.val[2] = linear_interpolate(text->color[0].val[2],
		text->color[1].val[2], wood_f);
	color.val[3] = linear_interpolate(text->color[0].val[3],
		text->color[1].val[3], wood_f);
	return (color);
	(void)normal_inter;
}
