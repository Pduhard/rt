/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   bump_mapping.c                                   .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: pduhard- <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2020/01/29 15:27:10 by pduhard-     #+#   ##    ##    #+#       */
/*   Updated: 2020/01/30 18:10:01 by pduhard-    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "rt.h"
/*
t_3vecf		get_bump_mapping_perlin(t_3vecf inter_point, t_3vecf normal_inter, t_obj *obj)
{
	double	bump_x;
	double	bump_y;
	double	bump_z;
	t_3vecf	s_normal_inter;

	s_normal_inter = normal_inter;
	if (obj->obj_type == OBJ_PLANE)
	{
	//	t_2vecf	text_coord = obj->get_text_coordiante(inter_point, normal_inter, obj);

	//	bump_x = compute_2dperlin_factor(assign_3vecf(normal_inter.val[0] - obj->text.bump_fact, normal_inter.val[1], normal_inter.val[2]), obj->text.scale.val[0]);
	//	bump_x -= compute_2dperlin_factor(assign_3vecf(normal_inter.val[0] + obj->text.bump_fact, normal_inter.val[1], normal_inter.val[2]), obj->text.scale.val[0]);
	//	bump_y = compute_2dperlin_factor(assign_3vecf(normal_inter.val[0], normal_inter.val[1] - obj->text.bump_fact, normal_inter.val[2]), obj->text.scale.val[0]);
	//	bump_y -= compute_2dperlin_factor(assign_3vecf(normal_inter.val[0], normal_inter.val[1] + obj->text.bump_fact, normal_inter.val[2]), obj->text.scale.val[0]);
	//	text_coord
		normal_inter.val[0] += inter_point.val[0];
		normal_inter.val[1] = 50 * normal_inter.val[1] + 100 * inter_point.val[1];
		normal_inter.val[2] += inter_point.val[2];

//		normal_inter.val[0] += inter_point.val[0];
//		normal_inter.val[1] -= inter_point.val[1];
//		normal_inter.val[2] = 50 * normal_inter.val[2] + 100 * inter_point.val[2];

//		normal_inter.val[0] = -50 * normal_inter.val[0] + -100 * inter_point.val[0];
//		normal_inter.val[1] += inter_point.val[1];// + 100 * inter_point.val[1];
//		normal_inter.val[2] += inter_point.val[2];
		}
	bump_x = compute_3dperlin_factor(assign_3vecf(normal_inter.val[0] - obj->text.bump_fact, normal_inter.val[1], normal_inter.val[2]), obj->text.scale.val[0]);
	bump_x -= compute_3dperlin_factor(assign_3vecf(normal_inter.val[0] + obj->text.bump_fact, normal_inter.val[1], normal_inter.val[2]), obj->text.scale.val[0]);
	bump_y = compute_3dperlin_factor(assign_3vecf(normal_inter.val[0], normal_inter.val[1] - obj->text.bump_fact, normal_inter.val[2]), obj->text.scale.val[0]);
	bump_y -= compute_3dperlin_factor(assign_3vecf(normal_inter.val[0], normal_inter.val[1] + obj->text.bump_fact, normal_inter.val[2]), obj->text.scale.val[0]);
	bump_z = compute_3dperlin_factor(assign_3vecf(normal_inter.val[0], normal_inter.val[1], normal_inter.val[2] - obj->text.bump_fact), obj->text.scale.val[0]);
	bump_z -= compute_3dperlin_factor(assign_3vecf(normal_inter.val[0], normal_inter.val[1], normal_inter.val[2] + obj->text.bump_fact), obj->text.scale.val[0]);

	return (assign_3vecf(s_normal_inter.val[0] + bump_x, s_normal_inter.val[1] + bump_y, s_normal_inter.val[2] + bump_z));
	(void)inter_point;
	(void)obj;
}
*/

t_3vecf		get_bump_mapping_perlin(t_3vecf inter_point, t_3vecf normal_inter, t_obj *obj)
{
	double	bump_x;
	double	bump_y;
	double	bump_z;
	double	perlin_f;

	perlin_f = compute_3dperlin_factor(inter_point, obj->text.scale.val[0]);
	bump_x = compute_3dperlin_factor(assign_3vecf(inter_point.val[0] + obj->text.bump_fact, inter_point.val[1], inter_point.val[2]), obj->text.scale.val[0]) - perlin_f;
	bump_y = compute_3dperlin_factor(assign_3vecf(inter_point.val[0], inter_point.val[1] + obj->text.bump_fact, inter_point.val[2]), obj->text.scale.val[0]) - perlin_f;
	bump_z = compute_3dperlin_factor(assign_3vecf(inter_point.val[0], inter_point.val[1], inter_point.val[2] + obj->text.bump_fact), obj->text.scale.val[0]) - perlin_f;

//	bump_x /= obj->text.bump_fact;
//	bump_y /= obj->text.bump_fact;
//	bump_z /= obj->text.bump_fact;

	return (assign_3vecf(normal_inter.val[0] - bump_x , normal_inter.val[1] - bump_y, normal_inter.val[2] - bump_z));
}

t_3vecf		get_bump_mapping_marble(t_3vecf inter_point, t_3vecf normal_inter, t_obj *obj)
{
	double	bump_x;
	double	bump_y;
	double	bump_z;
	double	marble_f;

	marble_f = compute_marble_factor(inter_point, normal_inter, obj, obj->text.scale.val[0]);
//	obj->text.bump_fact *= 10;
	bump_x = compute_marble_factor(assign_3vecf(inter_point.val[0] + obj->text.bump_fact, inter_point.val[1], inter_point.val[2]), normal_inter, obj, obj->text.scale.val[0]) - marble_f;
	bump_y = compute_marble_factor(assign_3vecf(inter_point.val[0], inter_point.val[1] + obj->text.bump_fact, inter_point.val[2]), normal_inter, obj, obj->text.scale.val[0]) - marble_f;
	bump_z = compute_marble_factor(assign_3vecf(inter_point.val[0], inter_point.val[1], inter_point.val[2] + obj->text.bump_fact), normal_inter, obj, obj->text.scale.val[0]) - marble_f;
	bump_x = compute_marble_factor(assign_3vecf(inter_point.val[0] + obj->text.bump_fact, inter_point.val[1], inter_point.val[2]), normal_inter, obj, obj->text.scale.val[0]) - marble_f;
//	obj->text.bump_fact /= 10;
	return (assign_3vecf(normal_inter.val[0] - bump_x , normal_inter.val[1] - bump_y, normal_inter.val[2] - bump_z));
}

t_3vecf		get_bump_mapping_wood(t_3vecf inter_point, t_3vecf normal_inter, t_obj *obj)
{
	double	bump_x;
	double	bump_y;
	double	bump_z;
	double	wood_f;

	wood_f = compute_wood_factor(inter_point, obj->text.scale.val[0]);
	bump_x = compute_wood_factor(assign_3vecf(inter_point.val[0] + obj->text.bump_fact, inter_point.val[1], inter_point.val[2]), obj->text.scale.val[0]) - wood_f;
	bump_y = compute_wood_factor(assign_3vecf(inter_point.val[0], inter_point.val[1] + obj->text.bump_fact, inter_point.val[2]), obj->text.scale.val[0]) - wood_f;
	bump_z = compute_wood_factor(assign_3vecf(inter_point.val[0], inter_point.val[1], inter_point.val[2] + obj->text.bump_fact), obj->text.scale.val[0]) - wood_f;

	return (assign_3vecf(normal_inter.val[0] - bump_x , normal_inter.val[1] - bump_y, normal_inter.val[2] - bump_z));
}

t_3vecf		get_bump_mapping_image(t_3vecf inter_point, t_3vecf normal_inter, t_obj *obj)
{
	return (normal_inter);
	(void)inter_point;
	(void)obj;
}

t_3vecf		get_bump_mapping_sinus(t_3vecf inter_point, t_3vecf normal_inter, t_obj *obj)
{
	t_2vecf	text_coord = obj->get_text_coordinate(inter_point, normal_inter, obj);
	double	sin_f;
//	double	bump_x;
//	double	bump_y;
//	double	bump_z;

	sin_f = (1 + sin(text_coord.val[1]) * 50) / 2;

//	bump_x = 1 + (assign_3vecf(inter_point.val[0] + obj->text.bump_fact, inter_point.val[1], inter_point.val[2]), obj->text.scale.val[0]) - wood_f;
//	bump_y = compute_wood_factor(assign_3vecf(inter_point.val[0], inter_point.val[1] + obj->text.bump_fact, inter_point.val[2]), obj->text.scale.val[0]) - wood_f;
//	bump_z = compute_wood_factor(assign_3vecf(inter_point.val[0], inter_point.val[1], inter_point.val[2] + obj->text.bump_fact), obj->text.scale.val[0]) - wood_f;
	
	return (normal_inter);
	return (assign_3vecf(normal_inter.val[0] + sin_f, normal_inter.val[1] + sin_f, normal_inter.val[2] + sin_f));
	(void)inter_point;
	(void)obj;
}
