/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_triangle_param.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aplat <aplat@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/18 01:54:38 by aplat             #+#    #+#             */
/*   Updated: 2020/05/18 01:54:49 by aplat            ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

t_2vecf	get_text_coordinate_triangle(t_3vecf inter_point,
	t_3vecf normal_inter, t_obj *triangle)
{
	t_2vecf	text_coord;

	text_coord.val[1] = 0;
	text_coord.val[0] = 0;
	return (text_coord);
	(void)inter_point;
	(void)triangle;
	(void)normal_inter;
}

t_3vecf	get_origin_triangle(t_obj *triangle)
{
	return (((t_triangle *)triangle->obj_param)->origin);
}

t_3vecf	get_triangle_origin(t_obj *triangle,
	t_triangle *triangle_param, int sp_id)
{
	if (sp_id)
		return (move_3vecf(triangle_param->origin, triangle->motions, sp_id));
	return (triangle_param->origin);
}

t_3vecf	get_normal_intersect_triangle(t_3vecf inter_point,
	t_obj *triangle, int sp_id)
{
	t_triangle	*param;
	t_3vecf		normal;

	param = (t_triangle *)triangle->obj_param;
	normal = product_3vecf(sub_3vecf(param->b, param->a),
		sub_3vecf(param->c, param->a));
	normalize_3vecf(&normal);
	return (normal);
	(void)inter_point;
	(void)sp_id;
}
