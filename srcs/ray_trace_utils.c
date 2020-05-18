/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray_trace_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aplat <aplat@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/18 05:22:26 by aplat             #+#    #+#             */
/*   Updated: 2020/05/18 05:28:37 by aplat            ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

t_dist	new_tdist(double *cdist)
{
	return ((t_dist){cdist, BIAS, MAX_VIEW});
}

t_3vecf	reflect_ray(t_3vecf ray, t_3vecf normal_inter)
{
	t_3vecf	ref;
	double	norm_dot_ray;

	norm_dot_ray = dot_product_3vecf(normal_inter, ray);
	ref.val[0] = 2 * normal_inter.val[0] * norm_dot_ray - ray.val[0];
	ref.val[1] = 2 * normal_inter.val[1] * norm_dot_ray - ray.val[1];
	ref.val[2] = 2 * normal_inter.val[2] * norm_dot_ray - ray.val[2];
	return (ref);
}

t_3vecf	refract_ray(t_3vecf ray, t_3vecf normal_inter,
	double refraction_index)
{
	t_3vecf	ref;
	double	cosi;
	double	eta;
	double	k;
	t_2vecf	etat;

	cosi = dot_product_3vecf(neg_3vecf(ray), normal_inter);
	etat.val[0] = 1.;
	etat.val[1] = refraction_index;
	eta = etat.val[0] / etat.val[1];
	k = 1 - eta * eta * (1 - cosi * cosi);
	k = eta * cosi - sqrt(k);
	ref.val[0] = eta * ray.val[0] + k * normal_inter.val[0];
	ref.val[1] = eta * ray.val[1] + k * normal_inter.val[1];
	ref.val[2] = eta * ray.val[2] + k * normal_inter.val[2];
	return (ref);
}

t_3vecf	get_refl_color(t_rayt_param p, t_inter i)
{
	t_3vecf refl_ray;

	refl_ray = reflect_ray(p.inv_dir, i.normal_inter);
	normalize_3vecf(&refl_ray);
	return (ray_trace((t_leq){i.inter_point, refl_ray},
		p.data, p.depth - 1, p.sp_id));
}

t_3vecf	get_refr_color(t_rayt_param p, t_inter i, t_leq l, t_obj *obj)
{
	t_3vecf refr_ray;

	refr_ray = refract_ray(l.dir, i.normal_inter, obj->refraction);
	normalize_3vecf(&refr_ray);
	return (ray_trace((t_leq){i.inter_point, refr_ray},
		p.data, p.depth - 1, p.sp_id));
}
