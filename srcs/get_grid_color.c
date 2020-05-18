/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_grid_color.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aplat <aplat@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/18 01:32:49 by aplat             #+#    #+#             */
/*   Updated: 2020/05/18 01:32:50 by aplat            ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

t_4vecf	get_grid_color(t_3vecf inter_point, t_3vecf normal_inter, t_obj *obj)
{
	t_2vecf		text_coord;
	t_text_proc	*text;
	int			u_grid;
	int			v_grid;

	text = (t_text_proc *)obj->text.text_param;
	text_coord = obj->get_text_coordinate(inter_point, normal_inter, obj);
	text_coord.val[0] *= obj->text.scale.val[0];
	text_coord.val[1] *= obj->text.scale.val[1];
	text_coord.val[0] -= text_coord.val[0] < 0 ?
		(1 + fmod(text_coord.val[0], 1)) : fmod(text_coord.val[0], 1);
	text_coord.val[1] -= text_coord.val[1] < 0 ?
		(1 + fmod(text_coord.val[1], 1)) : fmod(text_coord.val[1], 1);
	u_grid = (fmod(text_coord.val[0], 2) == 0);
	v_grid = (fmod(text_coord.val[1], 2) == 0);
	if ((u_grid ^ v_grid))
		return (text->color[0]);
	return (text->color[1]);
}
