/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray_trace.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aplat <aplat@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/18 05:17:51 by aplat             #+#    #+#             */
/*   Updated: 2020/05/25 14:30:37 by aplat            ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

static void		compute_reflection_and_refraction(t_leq l, t_inter i,
	t_obj *obj, t_rayt_param p)
{
	t_3vecf	refr_color;
	t_3vecf refl_color;
	double	fresnel_ratio;

	fresnel_ratio = compute_fresnel_ratio(l.dir, i.normal_inter,
		obj->refraction);
	refr_color = fresnel_ratio < 0.999999 ?
		get_refr_color(p, i, l, obj) : assign_3vecf(0, 0, 0);
	refl_color = fresnel_ratio > 0.01 ?
		get_refl_color(p, i) : assign_3vecf(0, 0, 0);
	refr_color.val[0] = p.lighted_color->val[0] * (1 - p.obj_color.val[3])
									+ refr_color.val[0] * p.obj_color.val[3];
	refr_color.val[1] = p.lighted_color->val[1] * (1 - p.obj_color.val[3])
									+ refr_color.val[1] * p.obj_color.val[3];
	refr_color.val[2] = p.lighted_color->val[2] * (1 - p.obj_color.val[3])
									+ refr_color.val[2] * p.obj_color.val[3];
	p.lighted_color->val[0] = refr_color.val[0] * (1 - fresnel_ratio)
									+ refl_color.val[0] * fresnel_ratio;
	p.lighted_color->val[1] = refr_color.val[1] * (1 - fresnel_ratio)
									+ refl_color.val[1] * fresnel_ratio;
	p.lighted_color->val[2] = refr_color.val[2] * (1 - fresnel_ratio)
									+ refl_color.val[2] * fresnel_ratio;
}

static void		compute_transparency(t_inter i, t_leq l, t_rayt_param p)
{
	t_3vecf	refr_color;

	refr_color = ray_trace((t_leq){i.inter_point, l.dir},
		p.data, p.depth - 1, p.sp_id);
	p.lighted_color->val[0] = p.lighted_color->val[0] * (1 - p.obj_color.val[3])
									+ refr_color.val[0] * p.obj_color.val[3];
	p.lighted_color->val[1] = p.lighted_color->val[1] * (1 - p.obj_color.val[3])
									+ refr_color.val[1] * p.obj_color.val[3];
	p.lighted_color->val[2] = p.lighted_color->val[2] * (1 - p.obj_color.val[3])
									+ refr_color.val[2] * p.obj_color.val[3];
}

static void		compute_reflection_only(t_inter i, t_leq l, t_obj *obj, t_rayt_param p)
{
	t_3vecf refl_color;

	if (p.obj_color.val[3] > 0)
		compute_transparency(i, l, p);
	refl_color = get_refl_color(p, i);
	p.lighted_color->val[0] = p.lighted_color->val[0] * (1 - obj->reflection)
									+ refl_color.val[0] * obj->reflection;
	p.lighted_color->val[1] = p.lighted_color->val[1] * (1 - obj->reflection)
									+ refl_color.val[1] * obj->reflection;
	p.lighted_color->val[2] = p.lighted_color->val[2] * (1 - obj->reflection)
									+ refl_color.val[2] * obj->reflection;
}

static void		compute_recurs_ray(t_leq l, t_inter i, t_obj *cobj,
				t_rayt_param p)
{
	if (cobj->refraction)
		compute_reflection_and_refraction(l, i, cobj, p);
	else if (cobj->reflection)
		compute_reflection_only(i, l, cobj, p);
	else if (p.obj_color.val[3] > 0)
		compute_transparency(i, l, p);
}

t_3vecf			ray_trace(t_leq l, t_data *data, int depth, int sp_id)
{
	double		cdist;
	t_obj		*cobj;
	t_3vecf		clr;
	t_inter		i;
	t_4vecf		o_clr;

	if (!(cobj = ray_first_intersect(l, new_tdist(&cdist), data->objs, sp_id)))
	{
		clr = assign_3vecf(0, 0, 0);
		compute_fog(data, MAX_VIEW, &clr);
		return (add_3vecf(clr, compute_glare(l, data->lights, NULL)));
	}
	i = init_inter(l, cdist, cobj, sp_id);
	o_clr = get_obj_color(&i, cobj, l);
	if (init_lighted_color(cobj, i, l, (t_ilc_p){&clr, o_clr, data, sp_id}))
	{
		if (!depth)
			return (clr);
		compute_recurs_ray(l, i, cobj, (t_rayt_param){&clr,
			neg_3vecf(l.dir), o_clr, data, depth, sp_id});
	}
	return (add_color_effect(data, (t_clre_param){cdist, l, depth}, clr, i));
}
