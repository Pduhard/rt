/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cast_photon.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aplat <aplat@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/18 01:02:48 by aplat             #+#    #+#             */
/*   Updated: 2020/05/19 18:34:07 by aplat            ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

static t_3vecf	init_refractive_normal_inter(t_4vecf *clr, t_obj *obj,
	t_photon photon, t_leq l)
{
	t_3vecf	normal_inter;

	normal_inter = obj->get_normal_inter(photon.position, obj, 0);
	*clr = obj->get_text_color(photon.position, normal_inter, obj);
	if (dot_product_3vecf(normal_inter, l.dir) > 0)
		normal_inter = neg_3vecf(normal_inter);
	if (obj->get_bump_mapping)
		normal_inter = obj->get_bump_mapping(photon.position,
			normal_inter, obj);
	return (normal_inter);
}

static void		cast_photon_refractive_mat(t_leq l, t_phtn_cast p, t_obj *obj,
	t_photon photon)
{
	double			rr_f;
	t_3vecf			normal_inter;
	t_4vecf			clr;
	t_phtn_prob		prob;
	double			fr_r;

	rr_f = get_random_number(p.rand_iter * 0xfab1dede << (p.rand_iter & 7));
	normal_inter = init_refractive_normal_inter(&clr, obj, photon, l);
	fr_r = compute_fresnel_ratio(l.dir, normal_inter, obj->refraction);
	prob = get_prob((1 - fr_r) * (1 - clr.val[3]), (1 - fr_r) * clr.val[3],
		fr_r);
	if (rr_f < prob.absorb_prob)
		return (absorb_photon(l, p, photon));
	p.pwr = add_color_bleed(p.pwr, clr);
	if (rr_f < prob.refract_prob + prob.absorb_prob)
		return (refract_photon((t_leq){photon.position,
			refract_ray(l.dir, normal_inter, obj->refraction)}, p));
	else if (rr_f < prob.refract_prob + prob.absorb_prob
		+ prob.reflect_prob_spe)
	{
		return (reflect_photon_spec((t_leq){photon.position,
		reflect_ray(neg_3vecf(l.dir), normal_inter)}, p, p.photon_type));
	}
	return (reflect_photon_diff(l, p, photon, normal_inter));
}

static void		cast_photon_diffuse_mat(t_leq l, t_phtn_cast p,
	t_obj *obj, t_photon photon)
{
	double		rr_f;
	t_3vecf		normal_inter;
	t_4vecf		obj_color;
	t_phtn_prob	prob;

	rr_f = get_random_number(p.rand_iter * 0xfab1dede << (p.rand_iter & 7));
	normal_inter = init_refractive_normal_inter(&obj_color, obj, photon, l);
	prob = get_prob(obj->reflection ? 1 - obj->reflection : 0.4,
		0., obj->reflection ? obj->reflection : 0.6);
	if (rr_f < prob.absorb_prob)
		return (absorb_photon(l, p, photon));
	p.pwr = add_color_bleed(p.pwr, obj_color);
	if (rr_f < prob.absorb_prob + prob.reflect_prob_spe)
	{
		return (reflect_photon_spec(
			(t_leq){photon.position, reflect_ray(neg_3vecf(l.dir),
			normal_inter)}, p, obj->reflection == 1 ? p.photon_type : 1));
	}
	return (reflect_photon_diff(l, p, photon, normal_inter));
}

void			cast_photon(t_leq l, t_phtn_cast p)
{
	t_photon	photon;
	t_obj		*obj;
	double		dist;
	int			inside;

	inside = 0;
	if (!p.depth || is_null_3vecf(p.pwr))
		return ;
	obj = ray_first_intersect(l, (t_dist){&dist, BIAS, MAX_VIEW},
		p.data->objs, 0);
	if (!obj)
		return ;
	photon.position = add_3vecf(l.orig, product_c3vecf(l.dir, dist));
	if (obj->refraction)
		return (cast_photon_refractive_mat(l, p, obj, photon));
	return (cast_photon_diffuse_mat(l, p, obj, photon));
}
