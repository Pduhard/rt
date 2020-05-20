/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bump_mapping.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aplat <aplat@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/15 17:13:20 by aplat             #+#    #+#             */
/*   Updated: 2020/05/19 21:28:56 by aplat            ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

t_3vecf		get_bump_mapping_perlin(t_3vecf inter_point,
	t_3vecf normal_inter, t_obj *obj)
{
	double	bump_x;
	double	bump_y;
	double	bump_z;
	double	perlin_f;

	perlin_f = compute_3dperlin_factor(inter_point, obj->text.scale.val[0],
		((t_data *)obj->data)->permutation, ((t_data *)obj->data)->gradient);
	bump_x = compute_3dperlin_factor(assign_3vecf(
		inter_point.val[0] + obj->text.bump_fact, inter_point.val[1],
			inter_point.val[2]), obj->text.scale.val[0],
				((t_data *)obj->data)->permutation, ((t_data *)obj->data)->gradient) - perlin_f;
	bump_y = compute_3dperlin_factor(assign_3vecf(
			inter_point.val[0], inter_point.val[1] + obj->text.bump_fact,
				inter_point.val[2]), obj->text.scale.val[0],
				((t_data *)obj->data)->permutation, ((t_data *)obj->data)->gradient) - perlin_f;
	bump_z = compute_3dperlin_factor(assign_3vecf(
			inter_point.val[0], inter_point.val[1], inter_point.val[2] +
				obj->text.bump_fact), obj->text.scale.val[0],
					((t_data *)obj->data)->permutation, ((t_data *)obj->data)->gradient) - perlin_f;
	return (assign_3vecf(normal_inter.val[0] - bump_x,
		normal_inter.val[1] - bump_y, normal_inter.val[2] - bump_z));
}

t_3vecf		get_bump_mapping_water(t_3vecf inter_point,
	t_3vecf normal_inter, t_obj *obj)
{
	inter_point = add_c3vecf(inter_point, ((t_data *)obj->data)->water_f);
	return (get_bump_mapping_perlin(inter_point, normal_inter, obj));
}

t_3vecf		get_bump_mapping_fbm(t_3vecf inter_point,
	t_3vecf normal_inter, t_obj *obj)
{
	double	bump_x;
	double	bump_y;
	double	bump_z;
	double	fbm_f;

	fbm_f = compute_3dfbm_factor(inter_point, obj->text.scale.val[0]);
	bump_x = compute_3dfbm_factor(assign_3vecf(
		inter_point.val[0] + obj->text.bump_fact, inter_point.val[1],
			inter_point.val[2]), obj->text.scale.val[0]) - fbm_f;
	bump_y = compute_3dfbm_factor(assign_3vecf(
		inter_point.val[0], inter_point.val[1] + obj->text.bump_fact,
			inter_point.val[2]), obj->text.scale.val[0]) - fbm_f;
	bump_z = compute_3dfbm_factor(assign_3vecf(
		inter_point.val[0], inter_point.val[1], inter_point.val[2] +
			obj->text.bump_fact), obj->text.scale.val[0]) - fbm_f;
	return (assign_3vecf(normal_inter.val[0] - bump_x,
		normal_inter.val[1] - bump_y, normal_inter.val[2] - bump_z));
}

t_3vecf		get_bump_mapping_marble(t_3vecf inter_point,
	t_3vecf normal_inter, t_obj *obj)
{
	double	bump_x;
	double	bump_y;
	double	bump_z;
	double	marble_f;

	marble_f = compute_marble_factor(inter_point, obj->text.scale.val[0],
		((t_data *)obj->data)->permutation, ((t_data *)obj->data)->gradient);
	bump_x = compute_marble_factor(assign_3vecf(
		inter_point.val[0] + obj->text.bump_fact, inter_point.val[1],
			inter_point.val[2]), obj->text.scale.val[0],
			((t_data *)obj->data)->permutation, ((t_data *)obj->data)->gradient) - marble_f;
	bump_y = compute_marble_factor(assign_3vecf(
		inter_point.val[0], inter_point.val[1] + obj->text.bump_fact,
			inter_point.val[2]), obj->text.scale.val[0],
				((t_data *)obj->data)->permutation, ((t_data *)obj->data)->gradient) - marble_f;
	bump_z = compute_marble_factor(assign_3vecf(
		inter_point.val[0], inter_point.val[1], inter_point.val[2] +
			obj->text.bump_fact), obj->text.scale.val[0],
				((t_data *)obj->data)->permutation, ((t_data *)obj->data)->gradient) - marble_f;
	return (assign_3vecf(normal_inter.val[0] - bump_x,
		normal_inter.val[1] - bump_y, normal_inter.val[2] - bump_z));
}

t_3vecf		get_bump_mapping_wood(t_3vecf inter_point,
	t_3vecf normal_inter, t_obj *obj)
{
	double	bump_x;
	double	bump_y;
	double	bump_z;
	double	wood_f;

	wood_f = compute_wood_factor(inter_point, obj->text.scale.val[0],
		((t_data *)obj->data)->permutation, ((t_data *)obj->data)->gradient);
	bump_x = compute_wood_factor(assign_3vecf(
		inter_point.val[0] + obj->text.bump_fact, inter_point.val[1],
			inter_point.val[2]), obj->text.scale.val[0],
				((t_data *)obj->data)->permutation, ((t_data *)obj->data)->gradient) - wood_f;
	bump_y = compute_wood_factor(assign_3vecf(
		inter_point.val[0], inter_point.val[1] + obj->text.bump_fact,
			inter_point.val[2]), obj->text.scale.val[0],
				((t_data *)obj->data)->permutation, ((t_data *)obj->data)->gradient) - wood_f;
	bump_z = compute_wood_factor(assign_3vecf(
		inter_point.val[0], inter_point.val[1], inter_point.val[2] +
			obj->text.bump_fact), obj->text.scale.val[0],
				((t_data *)obj->data)->permutation, ((t_data *)obj->data)->gradient) - wood_f;
	return (assign_3vecf(normal_inter.val[0] - bump_x,
		normal_inter.val[1] - bump_y, normal_inter.val[2] - bump_z));
}
