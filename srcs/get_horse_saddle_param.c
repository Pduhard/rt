/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_horse_saddle_param.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aplat <aplat@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/18 01:33:16 by aplat             #+#    #+#             */
/*   Updated: 2020/05/18 01:33:17 by aplat            ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

t_2vecf	get_text_coordinate_horse_saddle(t_3vecf inter_point,
	t_3vecf normal_inter, t_obj *horse_saddle)
{
	return ((t_2vecf){{0, 0}});
	(void)inter_point;
	(void)horse_saddle;
	(void)normal_inter;
}

t_3vecf	get_origin_horse_saddle(t_obj *horse_saddle)
{
	return (((t_horse_saddle *)horse_saddle->obj_param)->origin);
}

t_3vecf	get_horse_saddle_origin(t_obj *horse_saddle, t_horse_saddle *param,
	int sp_id)
{
	if (sp_id)
		return (move_3vecf(param->origin, horse_saddle->motions, sp_id));
	return (param->origin);
}

t_3vecf	get_normal_intersect_horse_saddle(t_3vecf inter_point,
	t_obj *horse_saddle, int sp_id)
{
	t_horse_saddle	*param;
	t_3vecf			normal_inter;
	double			x;
	double			z;
	t_3vecf			horse_saddle_origin;

	param = (t_horse_saddle *)horse_saddle->obj_param;
	horse_saddle_origin = get_horse_saddle_origin(horse_saddle, param, sp_id);
	x = inter_point.val[0] - horse_saddle_origin.val[0];
	z = inter_point.val[2] - horse_saddle_origin.val[2];
	normal_inter.val[0] = (2 * x) / (param->x_fact * param->x_fact);
	normal_inter.val[1] = -1;
	normal_inter.val[2] = (-2 * z) / (param->y_fact * param->y_fact);
	normalize_3vecf(&normal_inter);
	return (normal_inter);
}
