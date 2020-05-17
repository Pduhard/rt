/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_cone_param.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aplat <aplat@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/15 17:18:53 by aplat             #+#    #+#             */
/*   Updated: 2020/05/15 17:26:23 by aplat            ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

t_2vecf	get_text_coordinate_cone(t_3vecf inter_point,
		t_3vecf normal_inter, t_obj *cone)
{
	t_2vecf	text_coord;
	t_3vecf	cp;
	t_3vecf	cone_axis[3];
	t_cone	*param;

	param = (t_cone *)cone->obj_param;
	get_uv_axis(cone_axis, sub_3vecf(param->tip, param->center));
	cp = sub_3vecf(inter_point, param->center);
	text_coord.val[0] = -dot_product_3vecf(cone_axis[1], cp) / (M_PI * 2);
	text_coord.val[1] = atan2(dot_product_3vecf(cone_axis[0], cp),
		dot_product_3vecf(cone_axis[2], cp)) / (M_PI * 2);
	return (text_coord);
	(void)normal_inter;
}

t_3vecf	get_origin_cone(t_obj *cone)
{
	return (((t_cone *)cone->obj_param)->tip);
}

t_3vecf	get_cone_origin(t_obj *cone, t_cone *cone_param, int sp_id)
{
	if (sp_id)
		return (move_3vecf(cone_param->center, cone->motions, sp_id));
	return (cone_param->center);
}

t_3vecf	get_cone_tip(t_obj *cone, t_cone *cone_param, int sp_id)
{
	if (sp_id)
		return (move_3vecf(cone_param->tip, cone->motions, sp_id));
	return (cone_param->tip);
}

t_3vecf	get_normal_intersect_cone(t_3vecf inter_point, t_obj *cone, int sp_id)
{
	t_cone	*cone_param;
	t_3vecf	hp;
	t_3vecf	normal;
	t_3vecf	cone_tip;
	t_3vecf	cone_origin;

	cone_param = (t_cone *)cone->obj_param;
	cone_origin = get_cone_origin(cone, cone_param, sp_id);
	cone_tip = get_cone_tip(cone, cone_param, sp_id);
	hp = sub_3vecf(cone_tip, inter_point);
	normal = product_3vecf(hp, product_3vecf(hp,
			sub_3vecf(cone_origin, inter_point)));
	normalize_3vecf(&normal);
	return (normal);
}
