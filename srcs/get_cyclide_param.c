/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_cyclide_param.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aplat <aplat@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/18 01:23:17 by aplat             #+#    #+#             */
/*   Updated: 2020/05/18 01:23:25 by aplat            ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

t_2vecf	get_text_coordinate_cyclide(t_3vecf inter_point, t_3vecf normal_inter,
	t_obj *cyclide)
{
	return ((t_2vecf){{0, 0}});
	(void)normal_inter;
	(void)inter_point;
	(void)cyclide;
}

t_3vecf	get_origin_cyclide(t_obj *cyclide)
{
	return (((t_cyclide *)cyclide->obj_param)->origin);
}

t_3vecf	get_cyclide_origin(t_obj *cyclide, t_cyclide *param, int sp_id)
{
	if (sp_id)
		return (move_3vecf(param->origin, cyclide->motions, sp_id));
	return (param->origin);
}

t_3vecf	get_normal_intersect_cyclide(t_3vecf inter_point,
	t_obj *cyclide, int sp_id)
{
	t_cyclide	*param;
	t_3vecf		normal_inter;
	t_3vecf		cyclide_origin;
	double		c[3];

	param = (t_cyclide *)cyclide->obj_param;
	cyclide_origin = get_cyclide_origin(cyclide, param, sp_id);
	c[0] = inter_point.val[0] - cyclide_origin.val[0];
	c[1] = inter_point.val[1] - cyclide_origin.val[1];
	c[2] = inter_point.val[2] - cyclide_origin.val[2];
	normal_inter.val[0] = 2 * c[0] * c[2] + 2 * param->param * c[0];
	normal_inter.val[1] = 2 * c[1] * c[2] - 2 * param->param * c[1];
	normal_inter.val[2] = c[0] * c[0] + c[1] * c[1] + 3 * c[2] * c[2]
		- param->param * param->param;
	normalize_3vecf(&normal_inter);
	return (normal_inter);
}
