/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_perlin_color.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aplat <aplat@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/18 01:51:59 by aplat             #+#    #+#             */
/*   Updated: 2020/05/18 01:52:00 by aplat            ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

t_4vecf	get_perlin_color(t_3vecf inter_point, t_3vecf normal_inter, t_obj *obj)
{
	double		perlin_f;
	t_text_proc	*text;
	t_4vecf		color;

	text = (t_text_proc *)obj->text.text_param;
	perlin_f = fabs(compute_3dperlin_factor(inter_point, obj->text.scale.val[0],
		obj->data->permutation, obj->data->gradient));
	color.val[0] = text->color[0].val[0] * perlin_f;
	color.val[1] = text->color[0].val[1] * perlin_f;
	color.val[2] = text->color[0].val[2] * perlin_f;
	color.val[3] = text->color[0].val[3] * (perlin_f + PERLIN_TRANSP_ADD);
	return (color);
	(void)normal_inter;
}
