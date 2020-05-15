#include "rt.h"

t_3vecf	window_to_view(double x, double y, double win_w, double win_h)
{
	t_3vecf	vec;

	vec.val[0] = x / win_w;
	vec.val[1] = y / win_h;
	if (win_w > win_h)
		vec.val[0] *= win_w / win_h;
	else if (win_h > win_w)
		vec.val[1] *= win_h / win_w;
	vec.val[2] = 1;
	return (vec);
}

int		check_inside_negative(t_leq l, t_dist dist, t_data *data, int sp_id)
{
	t_obj	*negative_objs;
	t_3vecf	inter_point;

	inter_point = add_3vecf(l.orig, product_c3vecf(l.dir, *(dist.dist)));
	inter_point.val[0] = l.orig.val[0] + l.dir.val[0] * *(dist.dist);
	inter_point.val[1] = l.orig.val[1] + l.dir.val[1] * *(dist.dist);
	inter_point.val[2] = l.orig.val[2] + l.dir.val[2] * *(dist.dist);
	negative_objs = data->negative_objs;
	while (negative_objs)
	{
		if (negative_objs->check_inside(inter_point, negative_objs))
			return (check_cuts(l, dist,
				(t_cut_fparam){negative_objs, NULL, sp_id, 1}, data) ? 1 : 0);
			negative_objs = negative_objs->next;
	}
	return (0);
}

t_obj	*ray_first_intersect(t_leq l, t_dist dist, t_obj *objs, int sp_id)
{
	t_obj	*closest_obj;
	t_obj	*objs_save;
	t_data *data;

	data = objs ? objs->data : NULL;
	objs_save = objs;
	closest_obj = NULL;
	*(dist.dist) = MAX_VIEW;
	while (objs)
	{
		if (objs->ray_intersect(l, objs, dist, sp_id))
			closest_obj = objs;
		objs = objs->next;
	}
	if (closest_obj && data->negative_objs
		&& check_inside_negative(l, dist, data, sp_id))
		return (ray_first_intersect(l,
			(t_dist){dist.dist, *(dist.dist), dist.max_dist}, objs_save, sp_id));
	if (closest_obj && closest_obj->cuts)
		return (check_cuts(l, dist,
			(t_cut_fparam){closest_obj, objs_save, sp_id, 0}, data));
	return (closest_obj);
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
	double refraction_index, int inside)
{
	t_3vecf	ref;
	double	cosi;
	double	etai;
	double	etat;
	double	eta;
	double	k;

	cosi = dot_product_3vecf(neg_3vecf(ray), normal_inter);
	etai = inside ? refraction_index : 1.;
	etat = inside ? 1. : refraction_index;
	eta = etai / etat;
	k = 1 - eta * eta * (1 - cosi * cosi);
	k = eta * cosi - sqrt(k);
	ref.val[0] = eta * ray.val[0] + k * normal_inter.val[0];
	ref.val[1] = eta * ray.val[1] + k * normal_inter.val[1];
	ref.val[2] = eta * ray.val[2] + k * normal_inter.val[2];
	return (ref);
}

/* fresnel_ratio:	SCHLICK approximation	3 times faster*/

double	compute_fresnel_ratio(t_3vecf dir, t_3vecf normal_inter, double refraction_index, int inside)
{
	double	r0;
	t_3vecf	inv_dir;
	double	cos_theta;
	double	inv_ctheta;

	inv_dir = assign_3vecf(-dir.val[0], -dir.val[1], -dir.val[2]);
	cos_theta = dot_product_3vecf(inv_dir, normal_inter);
	if (inside)
		r0 = (refraction_index - 1) / (refraction_index + 1);
	else
		r0 = (1 - refraction_index) / (1 + refraction_index);
	r0 *= r0;
	inv_ctheta = 1 - cos_theta;
	inv_ctheta = inv_ctheta * inv_ctheta * inv_ctheta * inv_ctheta * inv_ctheta;
	return (r0 + (1 - r0) * (1 - cos_theta) * inv_ctheta);
}

void	cel_shade(double *val)
{
	if (*val < CEL_BOUND_1)
		*val = 0.;
	else if (*val < CEL_BOUND_2)
		*val = CEL_BOUND_1;
	else if (*val < CEL_BOUND_3)
		*val = CEL_BOUND_2;
	else if (*val < CEL_BOUND_4)
		*val = CEL_BOUND_3;
	else if (*val < CEL_BOUND_6)
		*val = CEL_BOUND_4;
	else
		*val = CEL_BOUND_6;
}

void    get_light_dir(t_3vecf *light_dir, double *light_len, t_light *lights, t_3vecf inter_point)
{
	if (lights->light_type == LIGHT_POINT)
	{
		*light_dir = sub_3vecf(lights->param, inter_point);
		*light_len = get_length_3vecf(*light_dir);
	}
	else if (lights->light_type == LIGHT_DIRECTIONAL)
	{
		*light_dir = neg_3vecf(lights->param);
		// assign_3vecf(-lights->param.val[0], -lights->param.val[1], -lights->param.val[2]);
		*light_len = MAX_VIEW;
	}
	normalize_3vecf(light_dir);// same
}

void  update_transparency(t_3vecf *transp_fact, t_4vecf obj_color)
{
	transp_fact->val[0] -= ((1 - obj_color.val[3]) * (1.1 - obj_color.val[0]));
	transp_fact->val[1] -= ((1 - obj_color.val[3]) * (1.1 - obj_color.val[1]));
	transp_fact->val[2] -= ((1 - obj_color.val[3]) * (1.1 - obj_color.val[2]));
}

t_obj *check_for_shadow(t_3vecf *transp_fact, t_3vecf light_dir,
	double light_len, t_compute_light_param *p)
{
	t_3vecf	inter_point;
	double	shadow_dist;
	t_obj		*shadow_obj;
	t_3vecf tex_normal_inter;
	t_4vecf	obj_color;

	inter_point = p->inter_point;
	*transp_fact = assign_3vecf(1, 1, 1);
	shadow_dist = MAX_VIEW;
	while ((shadow_obj = ray_first_intersect((t_leq){inter_point, light_dir},
		(t_dist){&shadow_dist, BIAS, light_len}, p->objs, p->sp_id)))
	{
		inter_point = add_3vecf(inter_point, product_c3vecf(light_dir, shadow_dist));
		light_len -= shadow_dist;
		tex_normal_inter = shadow_obj->get_normal_inter(inter_point, shadow_obj, p->sp_id);
		normalize_3vecf(&tex_normal_inter);
		obj_color = shadow_obj->get_text_color(inter_point, tex_normal_inter, shadow_obj);
		if (obj_color.val[3] > 0)
			update_transparency(transp_fact, obj_color);
		else
			return (shadow_obj);
	}
	return (NULL);
}

void    clamp_transparency(t_3vecf *transp_fact)
{
	if (transp_fact->val[0] < 0)
		transp_fact->val[0] = 0;
	if (transp_fact->val[1] < 0)
		transp_fact->val[1] = 0;
	if (transp_fact->val[2] < 0)
		transp_fact->val[2] = 0;
}

t_3vecf    add_diffuse(t_compute_light_param p,
	t_3vecf transp_fact, t_3vecf light_dir)
{
	double	dp_n_ldir;
	t_3vecf	diffuse;

	dp_n_ldir = dot_product_3vecf(p.normal_inter, light_dir);
	if (dp_n_ldir > 0)
	{
		diffuse.val[0] = p.lights->color.val[0] * transp_fact.val[0] * dp_n_ldir;
		diffuse.val[1] = p.lights->color.val[1] * transp_fact.val[1] * dp_n_ldir;
		diffuse.val[2] = p.lights->color.val[2] * transp_fact.val[2] * dp_n_ldir;
	}
	else
		return (assign_3vecf(0, 0, 0));
	return (diffuse);
}

t_3vecf add_specular(t_compute_light_param p, t_3vecf transp_fact,
	t_3vecf light_dir, t_3vecf inv_dir)
{
	t_3vecf spec_vec;
	t_3vecf specular;
	double	ref_dot_idir;
	double  spec_fact;

	if (p.shininess)
	{
		spec_vec = reflect_ray(light_dir, p.normal_inter);
		ref_dot_idir = dot_product_3vecf(spec_vec, inv_dir);
		if (ref_dot_idir > 0)
		{
			spec_fact = powf(ref_dot_idir / (get_length_3vecf(spec_vec)
				* get_length_3vecf(inv_dir)), p.shininess);
			specular.val[0] = p.lights->color.val[0] * transp_fact.val[0] * spec_fact;
			specular.val[1] = p.lights->color.val[1] * transp_fact.val[1] * spec_fact;
			specular.val[2] = p.lights->color.val[2] * transp_fact.val[2] * spec_fact;
			return (specular);
		}
	}
	return (assign_3vecf(0, 0, 0));
}

t_3vecf add_light_effect(t_compute_light_param p, t_3vecf light_fact)
{
	if (p.data->caustics_gi)
		light_fact = add_3vecf(light_fact,
			compute_global_illumination(p.inter_point, p.normal_inter,
				p.data->caustic_map, NN_CAUSTIC_PHOTON_MAX));
	if (p.data->indirect_gi)
		light_fact = add_3vecf(light_fact,
			compute_global_illumination(p.inter_point, p.normal_inter,
				p.data->indirect_map, NN_INDIRECT_PHOTON_MAX));
	if (p.data->cel_shading)
	{
		cel_shade(&(light_fact.val[0]));
		cel_shade(&(light_fact.val[1]));
		cel_shade(&(light_fact.val[2]));
	}
	return (light_fact);
}

t_3vecf	compute_lights(t_compute_light_param p)
{
	t_3vecf	light_fact;
	t_3vecf	transp_fact;
	double	light_len;
	t_3vecf	light_dir;
	t_obj		*shadow_obj;

	light_fact = assign_3vecf(0, 0, 0);
	while (p.lights)
	{
		if (p.lights->light_type == LIGHT_AMBIENT)
			light_fact = add_3vecf(light_fact, p.lights->color);
		else
		{
			get_light_dir(&light_dir, &light_len, p.lights, p.inter_point);
			shadow_obj = check_for_shadow(&transp_fact, light_dir, light_len, &p);
			clamp_transparency(&transp_fact);
			if (!shadow_obj && (transp_fact.val[0] ||
				transp_fact.val[1] || transp_fact.val[2]))
				light_fact = add_3vecf(light_fact, add_3vecf(
					add_diffuse(p, transp_fact, light_dir),
					add_specular(p, transp_fact, light_dir, neg_3vecf(p.dir))));
		}
		p.lights = p.lights->next;
	}
	return (add_light_effect(p, light_fact));
}

int		is_on_cell_boundary(t_3vecf origin, t_3vecf inter_point,
	t_3vecf normal_inter, t_obj *closest_obj, int sp_id)
{
	t_3vecf	new_dir;
	t_3vecf	new_inter_point;
	double	dist;

	new_inter_point.val[0] = inter_point.val[0] + normal_inter.val[0] * CEL_BOUNDARY;
	new_inter_point.val[1] = inter_point.val[1] + normal_inter.val[1] * CEL_BOUNDARY;
	new_inter_point.val[2] = inter_point.val[2] + normal_inter.val[2] * CEL_BOUNDARY;
	new_dir = sub_3vecf(new_inter_point, origin);
	normalize_3vecf(&new_dir);
	dist = MAX_VIEW;
	if (closest_obj->ray_intersect((t_leq){origin, new_dir}, closest_obj, (t_dist){&dist, 2 * CEL_BOUNDARY, MAX_VIEW}, sp_id))
		return (0);
	return (1);
}

t_3vecf		compute_glare(t_leq l, t_light *lights, t_3vecf *inter_point)
{
	t_3vecf	glare;
	t_3vecf	light_dir;
	t_3vecf	light_pos;
	double	obj_dist;
	double	light_dist;

	obj_dist = inter_point ? get_length_3vecf(sub_3vecf(*inter_point, l.orig)) : MAX_VIEW;
	light_pos = assign_3vecf(0, 0, 0);
	glare = assign_3vecf(0, 0, 0);
	while (lights)
	{
	//	if (lights->light_type != LIGHT_AMBIENT)
	//	{
		if (lights->light_type == LIGHT_POINT)
		{
			light_dir = sub_3vecf(lights->param, l.orig);
			light_dist = get_length_3vecf(light_dir);
			normalize_3vecf(&light_dir);
			light_pos = lights->param;

			if (light_dist < obj_dist)
			{
				double fact;
				t_3vecf	light_npoint;

				light_npoint.val[0] = l.orig.val[0] + l.dir.val[0] * light_dist;
				light_npoint.val[1] = l.orig.val[1] + l.dir.val[1] * light_dist;
				light_npoint.val[2] = l.orig.val[2] + l.dir.val[2] * light_dist;
				double	lndist;
				lndist = get_length_3vecf(sub_3vecf(light_pos, light_npoint));
				if (lndist < 0.2)
					fact = (0.2 - lndist) * 5;
				else
					fact = 0;
				/*dot_product_3vecf(l.dir, light_dir);
				if (fact > 0.9)
				fact = exp(fact) - exp(1. - 0.05 / light_dist);
				fact = fact < 0 ? 0 : fact;
				fact *= exp(fact * 400);
				*/
				glare.val[0] += fact;
				glare.val[1] += fact;
				glare.val[2] += fact;
			}
		}
		lights = lights->next;
	}
	return (glare);
}

t_3vecf	ray_trace(t_leq l, double min_dist, double max_dist, t_data *data, int depth, int sp_id)
{
	double	closest_dist;
	t_obj		*closest_obj;
	t_3vecf		light_fact;
	t_3vecf	lighted_color;

	lighted_color = assign_3vecf(0, 0, 0);
	closest_obj = ray_first_intersect(l, (t_dist){&closest_dist, min_dist, max_dist}, data->objs, sp_id);
	//printf("rtrace\n");
	if (!closest_obj)
	{
		light_fact = compute_glare(l, data->lights, NULL);
		return (light_fact);
		if (data->fog.val[0] || data->fog.val[1])
		{

			double	fog_fact;

			if (closest_dist < data->fog.val[0])
				fog_fact = 1;
			else if (closest_dist < data->fog.val[1])
				fog_fact = (data->fog.val[1] - closest_dist) / (data->fog.val[1] - data->fog.val[0]);
			else
				fog_fact = 0;
			//	exp(-1 * (closest_dist / FOG_DIST));
			//double	fog_fact = (closest_dist / FOG_DIST);
		//	printf("%f\n", closest_dist);
			lighted_color.val[0] = 1 * (1 - fog_fact) + lighted_color.val[0] * fog_fact;
			lighted_color.val[1] = 1 * (1 - fog_fact) + lighted_color.val[1] * fog_fact;
			lighted_color.val[2] = 1 * (1 - fog_fact) + lighted_color.val[2] * fog_fact;
		}
		return (lighted_color);
	}
	t_3vecf		inter_point;
	t_3vecf		normal_inter;
	t_3vecf		inv_dir =  assign_3vecf(-l.dir.val[0], -l.dir.val[1], -l.dir.val[2]);
	t_4vecf		obj_color;

	inter_point.val[0] = l.orig.val[0] + l.dir.val[0] * closest_dist;
	inter_point.val[1] = l.orig.val[1] + l.dir.val[1] * closest_dist;
	inter_point.val[2] = l.orig.val[2] + l.dir.val[2] * closest_dist;
	normal_inter = closest_obj->get_normal_inter(inter_point, closest_obj, sp_id);
	normalize_3vecf(&normal_inter);
	t_3vecf	tex_normal_inter = normal_inter;
	int inside = 0;
	if (dot_product_3vecf(normal_inter, l.dir) > 0 && (inside = 1))
		normal_inter = assign_3vecf(-normal_inter.val[0], -normal_inter.val[1], -normal_inter.val[2]);
	if (closest_obj->get_bump_mapping)
		normal_inter = closest_obj->get_bump_mapping(inter_point, normal_inter, closest_obj);
	inside = 0;
	obj_color = closest_obj->get_text_color(inter_point, tex_normal_inter, closest_obj);
	if (closest_obj->obj_type != OBJ_SKYBOX)
		light_fact = compute_lights((t_compute_light_param){inter_point, normal_inter, l.dir, data->lights, data->objs, sp_id, data, closest_obj->shininess});
	else
		light_fact = assign_3vecf(1, 1, 1);
	lighted_color.val[0] = obj_color.val[0] * light_fact.val[0];
	lighted_color.val[1] = obj_color.val[1] * light_fact.val[1];
	lighted_color.val[2] = obj_color.val[2] * light_fact.val[2];
	if (data->cel_shading && is_on_cell_boundary(l.orig, inter_point, normal_inter, closest_obj, sp_id))
	{
		lighted_color.val[0] = 0;
		lighted_color.val[1] = 0;
		lighted_color.val[2] = 0;
	}
	else if (closest_obj->refraction) // reflection and refraction
	{
		t_3vecf	refr_color = assign_3vecf(0, 0, 0);
		t_3vecf refl_color = assign_3vecf(0, 0, 0);
		if (!depth)
			return (assign_3vecf(0, 0, 0));
		double fresnel_ratio = compute_fresnel_ratio(l.dir, normal_inter, closest_obj->refraction, inside);
//		fresnel_ratio = 0;
//		printf("%f\n", fresnel_ratio);
		if (fresnel_ratio < 0.999999) // else reflection
		{
			t_3vecf	refr_ray = refract_ray(l.dir, normal_inter, closest_obj->refraction, inside);
			normalize_3vecf(&refr_ray);
			refr_color = ray_trace((t_leq){inter_point, refr_ray}, BIAS, MAX_VIEW, data, depth - 1, sp_id);
		}
		if (fresnel_ratio > 0.01) // eles full refraction
		{
			t_3vecf	refl_ray = reflect_ray(inv_dir, normal_inter);
			normalize_3vecf(&refl_ray);
			refl_color = ray_trace((t_leq){inter_point, refl_ray}, BIAS, MAX_VIEW, data, depth - 1, sp_id);
		}
		/*	not sure */
/*		light_fact = compute_lights(inter_point, normal_inter, inv_dir, data->lights, data->objs);
		lighted_color.val[0] = closest_obj->color.val[0] * light_fact;
		lighted_color.val[1] = closest_obj->color.val[1] * light_fact;
		lighted_color.val[2] = closest_obj->color.val[2] * light_fact;*/

		refr_color.val[0] = lighted_color.val[0] * (1 - obj_color.val[3]) + refr_color.val[0] * obj_color.val[3];
		refr_color.val[1] = lighted_color.val[1] * (1 - obj_color.val[3]) + refr_color.val[1] * obj_color.val[3];
		refr_color.val[2] = lighted_color.val[2] * (1 - obj_color.val[3]) + refr_color.val[2] * obj_color.val[3];

	//	lighted_color.val[0] = lighted_color.val[0] * (1 - obj_color.val[3]) + refr_color.val[0] * obj_color.val[3];
	//	lighted_color.val[1] = lighted_color.val[1] * (1 - obj_color.val[3]) + refr_color.val[1] * obj_color.val[3];
	//	lighted_color.val[2] = lighted_color.val[2] * (1 - obj_color.val[3]) + refr_color.val[2] * obj_color.val[3];

		/*	-------- */

		lighted_color.val[0] = refr_color.val[0] * (1 - fresnel_ratio) + refl_color.val[0] * fresnel_ratio;
		lighted_color.val[1] = refr_color.val[1] * (1 - fresnel_ratio) + refl_color.val[1] * fresnel_ratio;
		lighted_color.val[2] = refr_color.val[2] * (1 - fresnel_ratio) + refl_color.val[2] * fresnel_ratio;
	}
	else if (closest_obj->reflection) // reflection
	{
	/*	light_fact = compute_lights(inter_point, normal_inter, inv_dir, data->lights, data->objs);
		lighted_color.val[0] = closest_obj->color.val[0] * light_fact;
		lighted_color.val[1] = closest_obj->color.val[1] * light_fact;
		lighted_color.val[2] = closest_obj->color.val[2] * light_fact;
	*/
		if (!depth)
			return (lighted_color);
		t_3vecf	refl_ray = reflect_ray(inv_dir, normal_inter);
		normalize_3vecf(&refl_ray);
		t_3vecf	refl_color = ray_trace((t_leq){inter_point, refl_ray}, BIAS, MAX_VIEW, data, depth - 1, sp_id);

		lighted_color.val[0] = lighted_color.val[0] * (1 - closest_obj->reflection) + refl_color.val[0] * closest_obj->reflection;
		lighted_color.val[1] = lighted_color.val[1] * (1 - closest_obj->reflection) + refl_color.val[1] * closest_obj->reflection;
		lighted_color.val[2] = lighted_color.val[2] * (1 - closest_obj->reflection) + refl_color.val[2] * closest_obj->reflection;
		//lighted_color.val[1] = closest_obj->color.val[1] * light_fact;
		//lighted_color.val[2] = closest_obj->color.val[2] * light_fact;
	}
	else if (obj_color.val[3] > 0) // transparency
	{
		t_3vecf	refr_color = assign_3vecf(0, 0, 0);
		if (!depth)
			return (assign_3vecf(0, 0, 0));
			//return (lighted_color);
		//double fresnel_ratio = compute_fresnel_ratio(l.dir, normal_inter, closest_obj->refraction);
	/*	light_fact = compute_lights(inter_point, normal_inter, inv_dir, data->lights, data->objs);
		lighted_color.val[0] = closest_obj->color.val[0] * light_fact;
		lighted_color.val[1] = closest_obj->color.val[1] * light_fact;
		lighted_color.val[2] = closest_obj->color.val[2] * light_fact;*/
//		if (fresnel_ratio < 1)
//		{
			//t_3vecf	refr_ray = refract_ray(l.dir, normal_inter, closest_obj->refraction);
			//normalize_3vecf(&refr_ray);
		refr_color = ray_trace((t_leq){inter_point, l.dir}, BIAS, MAX_VIEW, data, depth - 1, sp_id);
//		}
	//	t_3vecf	refl_ray = reflect_ray(inv_dir, normal_inter);
	//	normalize_3vecf(&refl_ray);
	//	t_3vecf	refl_color = ray_trace(inter_point, refl_ray, 0.01, MAX_VIEW, data, depth - 1);
		lighted_color.val[0] = lighted_color.val[0] * (1 - obj_color.val[3]) + refr_color.val[0] * obj_color.val[3];
		lighted_color.val[1] = lighted_color.val[1] * (1 - obj_color.val[3]) + refr_color.val[1] * obj_color.val[3];
		lighted_color.val[2] = lighted_color.val[2] * (1 - obj_color.val[3]) + refr_color.val[2] * obj_color.val[3];

	}
/*
	else //diffus only
	{
		light_fact = compute_lights(inter_point, normal_inter, inv_dir, data->lights, data->objs);
		lighted_color.val[0] = closest_obj->color.val[0] * light_fact;
		lighted_color.val[1] = closest_obj->color.val[1] * light_fact;
		lighted_color.val[2] = closest_obj->color.val[2] * light_fact;
	//	return (lighted_color);
	}
*/
	if (data->fog.val[0] || data->fog.val[1])
	{

		double	fog_fact;

		if (closest_dist < data->fog.val[0])
			fog_fact = 1;
		else if (closest_dist < data->fog.val[1])
			fog_fact = (data->fog.val[1] - closest_dist) / (data->fog.val[1] - data->fog.val[0]);
		else
			fog_fact = 0;
		//	exp(-1 * (closest_dist / FOG_DIST));
		//double	fog_fact = (closest_dist / FOG_DIST);
	//	printf("%f\n", closest_dist);
		lighted_color.val[0] = 1 * (1 - fog_fact) + lighted_color.val[0] * fog_fact;
		lighted_color.val[1] = 1 * (1 - fog_fact) + lighted_color.val[1] * fog_fact;
		lighted_color.val[2] = 1 * (1 - fog_fact) + lighted_color.val[2] * fog_fact;
	}
	if (depth == RAY_DEPTH)
	{
		t_3vecf	glare = compute_glare(l, data->lights, &inter_point);
		lighted_color.val[0] += glare.val[0];
		lighted_color.val[1] += glare.val[2];
		lighted_color.val[2] += glare.val[2];
	/*	if (lighted_color.val[0] > 1)
			lighted_color.val[0] = 1;
		if (lighted_color.val[1] > 1)
			lighted_color.val[1] = 1;
		if (lighted_color.val[2] > 1)
			lighted_color.val[2] = 1;
	*/
	}
	return (lighted_color);
}

int		clip_color(double color)
{
	if (color > 255)
		return (255);
	else if (color < 0)
		return (0);
	else
		return ((int)color);
}

void	ray_put_pixel(int i, int j, int *img, t_3vecf color, t_data *data)
{
	int		rgb_color;

	i = (int)data->size.val[0] / 2 + i;
	j = (int)data->size.val[1] / 2 + j;
	if (data->apply_color_filter)
		color = data->apply_color_filter(color);
	rgb_color = (clip_color(color.val[0] * 255) << 16);
	rgb_color |= (clip_color(color.val[1] * 255) << 8);
	rgb_color |= clip_color(color.val[2] * 255);
	img[j * (int)data->size.val[0] + i] = rgb_color;
}

void	init_threads(t_thread threads[NB_THREADS], t_data *data)
{
	int		index = -1;

	while (++index < NB_THREADS)
	{
		threads[index].start = (int)-data->size.val[0] / 2 + (int)data->size.val[0] * index / NB_THREADS;
		threads[index].end = (int)-data->size.val[0] / 2 + (int)data->size.val[0] * (index + 1) / NB_THREADS;
		threads[index].data = data;
	}
}

void	*render_thread(void *param)
{
	t_leq 	l;
	// t_3vecf	l.orig;
	// t_3vecf	l.dir;
	t_3vecf	color;
	int		i;
	int		j;
	t_thread	*thread = (t_thread *)param;
	t_data	*data = thread->data;

	i = thread->start;
	l.orig = data->camera->origin;
//	printf("salut thread\n");
	while (i < thread->end)
	{
		j = -data->size.val[1] / 2;
		while (j < data->size.val[1] / 2)
		{
			if (data->stereoscopy)
			{
				t_3vecf	origs[2];
			//	t_3vecf	dirs[2];
				t_3vecf	colors[2];
				t_3vecf	diff;

				diff = mult_3vecf_33matf(assign_3vecf(0.1, 0, 0), data->rot_mat[1]);
				origs[0] = l.orig;
				origs[1] = l.orig;
				origs[0].val[0] += diff.val[0];
				origs[0].val[1] += diff.val[1];
				origs[0].val[2] += diff.val[2];
				origs[1].val[0] -= diff.val[0];
				origs[1].val[1] -= diff.val[1];
				origs[1].val[2] -= diff.val[2];
				l.dir = mult_3vecf_33matf(mult_3vecf_33matf(window_to_view(i, j, data->size.val[0], data->size.val[1]), data->rot_mat[1]), data->rot_mat[0]);
				normalize_3vecf(&(l.dir));
				if (!data->motion_blur)
				{
					colors[0] = ray_trace((t_leq){origs[0], l.dir}, BIAS, MAX_VIEW, data, RAY_DEPTH, 0);
					colors[1] = ray_trace((t_leq){origs[1], l.dir}, BIAS, MAX_VIEW, data, RAY_DEPTH, 0);
				}
				else
				{
					colors[0] = motion_trace(origs[0], l.dir, data);
					colors[1] = motion_trace(origs[1], l.dir, data);
				}
				color.val[0] = colors[1].val[0];
				color.val[1] = colors[0].val[1];
				color.val[2] = colors[0].val[2];
				ray_put_pixel(i, j, data->mlx->img_str, color, data);
			}
		/*	else if (!data->anti_al)
			{

				l.dir = mult_3vecf_33matf(mult_3vecf_33matf(window_to_view(i, j, data->size.val[0], data->size.val[1]), data->rot_mat[1]), data->rot_mat[0]);
				normalize_3vecf(&l.dir);
				if (!data->motion_blur)
					color = ray_trace(l.orig, l.dir, BIAS, MAX_VIEW, data, RAY_DEPTH, 0);
				else
					color = motion_trace(l.orig, l.dir, data);
				ray_put_pixel(i, j, data->mlx->img_str, color, data);
			}
			else
			{
				t_3vecf	clr;
				int		anti_all_iter;
				int		offset;

				offset = 0;
				data->anti_al = 4;
				anti_all_iter = data->anti_al * data->anti_al;
				color = assign_3vecf(0, 0, 0);
				while (offset < anti_all_iter)
				{
					l.dir = mult_3vecf_33matf(mult_3vecf_33matf(window_to_view(i * data->anti_al + offset / data->anti_al, j * data->anti_al + offset % data->anti_al, WIN_WIDTH * data->anti_al, WIN_HEIGHT * data->anti_al), data->rot_mat[1]), data->rot_mat[0]);
					clr = ray_trace(l.orig, l.dir, BIAS, MAX_VIEW, data, 6, 0);
					color.val[0] += clr.val[0];
					color.val[1] += clr.val[1];
					color.val[2] += clr.val[2];
					offset++;
				}
				color.val[0] /= (double)anti_all_iter;
				color.val[1] /= (double)anti_all_iter;
				color.val[2] /= (double)anti_all_iter;
				ray_put_pixel(i, j, data->mlx->img_str, color, data);
			}
			*/
			else
			{
				t_3vecf	clr;
				int		anti_all_iter;
				int		offset;
				int		aa;

				aa = data->aa_adapt;
				offset = 0;
				anti_all_iter = data->aa_adapt * data->aa_adapt;
				if (anti_all_iter < 1)
				{
					aa = 1;
					anti_all_iter = 1;
				}
				color = assign_3vecf(0, 0, 0);
				while (offset < anti_all_iter)
				{
					l.dir = mult_3vecf_33matf(mult_3vecf_33matf(window_to_view(i * aa + offset / aa, j * aa + offset % aa, (int)data->size.val[0] * aa, (int)data->size.val[1] * aa), data->rot_mat[1]), data->rot_mat[0]);
					normalize_3vecf(&l.dir);
					if (!data->motion_blur)
						clr = ray_trace(l, BIAS, MAX_VIEW, data, RAY_DEPTH, 0);
					else
						clr = motion_trace(l.orig, l.dir, data);
					color.val[0] += clr.val[0];
					color.val[1] += clr.val[1];
					color.val[2] += clr.val[2];
					offset++;
				}
			//	printf("wefwef\n");
				color.val[0] /= (double)anti_all_iter;
				color.val[1] /= (double)anti_all_iter;
				color.val[2] /= (double)anti_all_iter;
			//	printf("wefwef\n");
				ray_put_pixel(i, j, data->mlx->img_str, color, data);
				if (data->aa_adapt < 1)
				{
					if ((i + 1) * 2 != data->size.val[0])
						ray_put_pixel(i + 1, j, data->mlx->img_str, color, data);
					if ((j + 1) * 2 != data->size.val[1])
						ray_put_pixel(i, j + 1, data->mlx->img_str, color, data);
					if ((j + 1) * 2 != data->size.val[1] && (i + 1) * 2 != data->size.val[0])
						ray_put_pixel(i + 1, j + 1, data->mlx->img_str, color, data);
				}
			}

			if (data->aa_adapt == MIN_AA)
				j += 2;
			else
				++j;
		}
		if (data->aa_adapt == MIN_AA)
			i += 2;
		else
			++i;
	}
	pthread_exit(NULL);
	return (NULL);
}

void	render(t_data *data)
{
//	t_3vecf	dir_t;
//	t_33matf	rot_mat[3];
	int		ret;
	int		i;
	t_thread	threads_param[NB_THREADS];
	pthread_t	threads[NB_THREADS];

	init_threads(threads_param, data);
//	i = -WIN_WIDTH / 2;
//	l.orig = data->camera->origin;
	//l.dir = mult_3vecf_33matf(window_to_view(0, 0), data->rot_mat[1]);
	//printf("%f %f %f\n", l.dir.val[0], l.dir.val[1], l.dir.val[2]);
	t_3vecf tm = mult_3vecf_33matf(assign_3vecf(1, 0, 0), data->rot_mat[1]);
	data->rot_mat[0] = init_rotation_matrix_vec(tm, degree_to_radian(data->camera->rotation.val[0]));
	//data->rot_mat[2] = init_rotation_matrix_z(degree_to_radian(data->camera->rotation.val[0]) * tm.val[2]);
	//rot_mat[1] = init_rotation_matrix_y(degree_to_radian(data->camera->rotation.val[1]));
	//rot_mat[2] = init_rotation_matrix_z(degree_to_radian(data->camera->rotation.val[2]));

	i = 0;
	while (i < NB_THREADS)
	{
		ret = pthread_create(&(threads[i]), NULL, render_thread, (void *)&(threads_param[i]));
		if (ret)
			exit(EXIT_FAILURE);
		++i;
	}
	i = 0;
	while (i < NB_THREADS)
		pthread_join(threads[i++], NULL);
	//exit(0);
		//printf("end");
}
