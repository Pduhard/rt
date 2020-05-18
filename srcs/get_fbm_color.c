/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_fbm_color.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aplat <aplat@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/18 01:25:19 by aplat             #+#    #+#             */
/*   Updated: 2020/05/18 01:25:20 by aplat            ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

t_4vecf	get_fbm_color(t_3vecf inter_point, t_3vecf normal_inter, t_obj *obj)
{
	double		fbm_f;
	t_4vecf		color;
	t_text_proc	*text;

	text = (t_text_proc *)obj->text.text_param;
	fbm_f = compute_3dfbm_factor(inter_point, obj->text.scale.val[0]);
	color.val[0] = linear_interpolate(text->color[0].val[0],
		text->color[1].val[0], fbm_f);
	color.val[1] = linear_interpolate(text->color[0].val[1],
		text->color[1].val[1], fbm_f);
	color.val[2] = linear_interpolate(text->color[0].val[2],
		text->color[1].val[2], fbm_f);
	color.val[3] = linear_interpolate(text->color[0].val[3],
		text->color[1].val[3], fbm_f);
	return (color);
	(void)normal_inter;
}
