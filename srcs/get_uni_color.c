/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_uni_color.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aplat <aplat@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/18 01:55:04 by aplat             #+#    #+#             */
/*   Updated: 2020/05/18 01:55:05 by aplat            ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

t_4vecf	get_uni_color(t_3vecf inter_point, t_3vecf normal_inter, t_obj *obj)
{
	t_text_proc	*text;

	text = (t_text_proc *)obj->text.text_param;
	return (text->color[0]);
	(void)inter_point;
	(void)normal_inter;
}
