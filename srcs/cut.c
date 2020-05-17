/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cut.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aplat <aplat@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/15 17:28:29 by aplat             #+#    #+#             */
/*   Updated: 2020/05/15 17:32:09 by aplat            ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

int			check_cut_classic(t_cut *cut, t_3vecf inter_point)
{
	t_cut_classic	*param;
	t_3vecf			io;

	if (cut->cut_type != CUT_STATIC && cut->cut_type != CUT_REAL)
		return (0);
	param = (t_cut_classic *)cut->cut_param;
	io = sub_3vecf(param->origin, inter_point);
	if (dot_product_3vecf(io, param->normal) > 0)
		return (1);
	return (0);
}

int		check_cut_uv(t_cut *cut, t_3vecf inter_point, t_cut_fparam cp)
{
	t_2vecf		coord2d;
	t_cut_uv	*uv;
	t_3vecf		normal_inter;

	if (cut->cut_type != CUT_UV)
		return (0);
	uv = (t_cut_uv *)cut->cut_param;
	normal_inter = cp.closest_obj->get_normal_inter(inter_point, cp.closest_obj,
								cp.sp_id);
	coord2d = cp.closest_obj->get_text_coordinate(inter_point, normal_inter,
						cp.closest_obj);
	coord2d.val[0] *= cp.closest_obj->text.scale.val[0];
	coord2d.val[1] *= cp.closest_obj->text.scale.val[1];
	coord2d.val[0] += cp.closest_obj->text.offset.val[0];
	coord2d.val[1] += cp.closest_obj->text.offset.val[1];
	if (coord2d.val[0] > uv->u_range.val[1]
		|| coord2d.val[0] < uv->u_range.val[0]
		|| coord2d.val[1] > uv->v_range.val[1]
		|| coord2d.val[1] < uv->v_range.val[0])
		return (1);
	return (0);
}

int		check_cut_texture(t_cut *cut, t_3vecf inter_point, t_cut_fparam cp)
{
	t_2vecf	coord2d;
	t_3vecf	normal_inter;

	if (cut->cut_type != CUT_TEXTURE)
		return (0);
	normal_inter = cp.closest_obj->get_normal_inter(inter_point,
		cp.closest_obj, cp.sp_id);
	coord2d = cp.closest_obj->get_text_coordinate(inter_point,
		normal_inter, cp.closest_obj);
	coord2d.val[0] *= cp.closest_obj->text.scale.val[0];
	coord2d.val[1] *= cp.closest_obj->text.scale.val[1];
	coord2d.val[0] += cp.closest_obj->text.offset.val[0];
	coord2d.val[1] += cp.closest_obj->text.offset.val[1];
	if (coord2d.val[0] > 1 || coord2d.val[0] < 0 || coord2d.val[1] > 1
		|| coord2d.val[1] < 0)
		return (1);
	return (0);
}

int		check_cut_sphere(t_cut *cut, t_3vecf inter_point)
{
	t_sphere	*param;

	if (cut->cut_type != CUT_SPHERE)
		return (0);
	param = (t_sphere *)cut->cut_param;
	if (get_length_3vecf(sub_3vecf(inter_point, param->origin)) > param->radius)
		return (1);
	return (0);
}

t_obj	*check_cuts(t_leq l, t_dist dist, t_cut_fparam cp, t_data *data)
{
	t_cut	*cuts;
	t_3vecf	inter_point;

	cuts = cp.closest_obj->cuts;
	inter_point = add_3vecf(l.orig, product_c3vecf(l.dir, *(dist.dist)));
	while (cuts)
	{
		if (check_cut_classic(cuts, inter_point)
		 || check_cut_uv(cuts, inter_point, cp)
		 || check_cut_texture(cuts, inter_point, cp)
		 || check_cut_sphere(cuts, inter_point))
		 {
				if (cp.negative)
		 			return (NULL);
		 		dist.min_dist = *(dist.dist);
		 		return (ray_first_intersect(l, (t_dist){dist.dist, *(dist.dist),
		 			dist.max_dist}, cp.objs, cp.sp_id));
		 }
		cuts = cuts->next;
	}
	return (cp.closest_obj);
	(void)data;
}
