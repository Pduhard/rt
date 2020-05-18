/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_cylinder_param.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aplat <aplat@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/18 01:23:49 by aplat             #+#    #+#             */
/*   Updated: 2020/05/18 01:24:05 by aplat            ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

t_2vecf	get_text_coordinate_cylinder(t_3vecf inter_point, t_3vecf normal_inter,
	t_obj *cylinder)
{
	t_2vecf		text_coord;
	t_3vecf		cp;
	t_3vecf		cyl_axis[3];
	t_cylinder	*param;

	param = (t_cylinder *)cylinder->obj_param;
	get_uv_axis(cyl_axis, sub_3vecf(param->tip, param->center));
	cp = sub_3vecf(inter_point, param->center);
	text_coord.val[0] = (1 - dot_product_3vecf(cyl_axis[1], cp))
		/ ((2 * M_PI) * param->radius);
	text_coord.val[1] = (atan2(dot_product_3vecf(cyl_axis[0], cp),
		dot_product_3vecf(cyl_axis[2], cp))) / (2 * M_PI);
	return (text_coord);
	(void)normal_inter;
}

t_3vecf	get_origin_cylinder(t_obj *cylinder)
{
	return (((t_cylinder *)cylinder->obj_param)->center);
}

t_3vecf	get_cylinder_origin(t_obj *cylinder, t_cylinder *cylinder_param,
	int sp_id)
{
	if (sp_id)
		return (move_3vecf(cylinder_param->center, cylinder->motions, sp_id));
	return (cylinder_param->center);
}

t_3vecf	get_cylinder_tip(t_obj *cylinder, t_cylinder *cylinder_param, int sp_id)
{
	if (sp_id)
		return (move_3vecf(cylinder_param->tip, cylinder->motions, sp_id));
	return (cylinder_param->tip);
}

t_3vecf	get_normal_intersect_cylinder(t_3vecf inter_point,
		t_obj *cylinder, int sp_id)
{
	t_3vecf	ch;
	t_3vecf	inter_proj;
	double	length_ch;
	double	step_inter_proj;
	t_3vecf	cylinder_origin;

	cylinder_origin = get_cylinder_origin(cylinder, cylinder->obj_param, sp_id);
	ch = sub_3vecf(get_cylinder_tip(cylinder, cylinder->obj_param, sp_id),
		cylinder_origin);
	length_ch = get_length_3vecf(ch);
	length_ch *= length_ch;
	step_inter_proj = dot_product_3vecf(ch,
			sub_3vecf(inter_point, cylinder_origin)) / length_ch;
	inter_proj = add_3vecf(cylinder_origin,
		product_c3vecf(ch, step_inter_proj));
	return (sub_3vecf(inter_proj, inter_point));
}
