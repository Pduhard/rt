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
	double refraction_index)
{
	t_3vecf	ref;
	double	cosi;
	double	etai;
	double	etat;
	double	eta;
	double	k;

	cosi = dot_product_3vecf(neg_3vecf(ray), normal_inter);
	etai = 1.;
	etat = refraction_index;
	eta = etai / etat;
	k = 1 - eta * eta * (1 - cosi * cosi);
	k = eta * cosi - sqrt(k);
	ref.val[0] = eta * ray.val[0] + k * normal_inter.val[0];
	ref.val[1] = eta * ray.val[1] + k * normal_inter.val[1];
	ref.val[2] = eta * ray.val[2] + k * normal_inter.val[2];
	return (ref);
}

/* fresnel_ratio:	SCHLICK approximation	3 times faster*/

double	compute_fresnel_ratio(t_3vecf dir, t_3vecf normal_inter, double refraction_index)
{
	double	r0;
	t_3vecf	inv_dir;
	double	cos_theta;
	double	inv_ctheta;

	inv_dir = assign_3vecf(-dir.val[0], -dir.val[1], -dir.val[2]);
	cos_theta = dot_product_3vecf(inv_dir, normal_inter);
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

int		is_on_cell_boundary(t_3vecf origin, t_inter i,
	t_obj *closest_obj, int sp_id)
{
	t_3vecf	new_dir;
	t_3vecf	new_inter_point;
	double	dist;

	new_inter_point = add_3vecf(i.inter_point,
		product_c3vecf(i.normal_inter, CEL_BOUNDARY));
	new_dir = sub_3vecf(new_inter_point, origin);
	normalize_3vecf(&new_dir);
	dist = MAX_VIEW;
	if (closest_obj->ray_intersect((t_leq){origin, new_dir},
		closest_obj, (t_dist){&dist, 2 * CEL_BOUNDARY, MAX_VIEW}, sp_id))
		return (0);
	return (1);
}

t_3vecf   add_glare_effect(t_leq l, t_3vecf light_pos, double light_dist)
{
	double fact;
	t_3vecf	light_npoint;
	double	lndist;

	light_npoint = add_3vecf(l.orig, product_c3vecf(l.dir, light_dist));
	lndist = get_length_3vecf(sub_3vecf(light_pos, light_npoint));
	if (lndist < 0.2)
		fact = (0.2 - lndist) * 5;
	else
		return (assign_3vecf(0, 0, 0));
	return (assign_3vecf(fact, fact, fact));
}

t_3vecf		compute_glare(t_leq l, t_light *lights, t_3vecf *inter_point)
{
	t_3vecf	glare;
	t_3vecf	light_dir;
	t_3vecf	light_pos;
	double	obj_dist;
	double	light_dist;

	obj_dist = inter_point ?
		get_length_3vecf(sub_3vecf(*inter_point, l.orig)) : MAX_VIEW;
	light_pos = assign_3vecf(0, 0, 0);
	glare = assign_3vecf(0, 0, 0);
	while (lights)
	{
		if (lights->light_type == LIGHT_POINT)
		{
			light_dir = sub_3vecf(lights->param, l.orig);
			light_dist = get_length_3vecf(light_dir);
			normalize_3vecf(&light_dir);
			light_pos = lights->param;

			if (light_dist < obj_dist)
				glare = add_3vecf(glare, add_glare_effect(l, light_pos, light_dist));
		}
		lights = lights->next;
	}
	return (glare);
}

t_4vecf get_obj_color(t_inter *i, t_obj *closest_obj, t_leq l)
{
	t_3vecf		tex_normal_inter;

	tex_normal_inter = i->normal_inter;
	if (dot_product_3vecf(i->normal_inter, l.dir) > 0)
		i->normal_inter = neg_3vecf(i->normal_inter);
	if (closest_obj->get_bump_mapping)
		i->normal_inter = closest_obj->get_bump_mapping(i->inter_point, i->normal_inter, closest_obj);
	return (closest_obj->get_text_color(i->inter_point, tex_normal_inter, closest_obj));
}

t_3vecf	mult_clr_light(t_4vecf obj_color, t_3vecf light_fact)
{
	t_3vecf lighted_color;

	lighted_color.val[0] = obj_color.val[0] * light_fact.val[0];
	lighted_color.val[1] = obj_color.val[1] * light_fact.val[1];
	lighted_color.val[2] = obj_color.val[2] * light_fact.val[2];
	return (lighted_color);
}

int	init_lighted_color(t_obj *closest_obj, t_inter i,
	t_leq l, t_ilc_p p)
{
	t_3vecf light_fact;

	if (closest_obj->obj_type == OBJ_SKYBOX)
		light_fact = assign_3vecf(1, 1, 1);
	else
		light_fact = compute_lights((t_compute_light_param){
			i.inter_point, i.normal_inter,
			l.dir, p.data->lights, p.data->objs,
			p.sp_id, p.data, closest_obj->shininess});
	*(p.lighted_color) = mult_clr_light(p.obj_color, light_fact);
	if (p.data->cel_shading && is_on_cell_boundary(l.orig,
		(t_inter){i.inter_point, i.normal_inter}, closest_obj, p.sp_id))
	{
		p.lighted_color->val[0] = 0;
		p.lighted_color->val[1] = 0;
		p.lighted_color->val[2] = 0;
		return (0);
	}
	return (1);
}

t_3vecf get_refl_color(t_rayt_param p, t_inter i)
{
	t_3vecf refl_ray;

	refl_ray = reflect_ray(p.inv_dir, i.normal_inter);
	normalize_3vecf(&refl_ray);
	return (ray_trace((t_leq){i.inter_point, refl_ray},
		p.data, p.depth - 1, p.sp_id));
}

t_3vecf get_refr_color(t_rayt_param p, t_inter i, t_leq l, t_obj *obj)
{
	t_3vecf refr_ray;

	refr_ray = refract_ray(l.dir, i.normal_inter, obj->refraction);
	normalize_3vecf(&refr_ray);
	return (ray_trace((t_leq){i.inter_point, refr_ray},
		p.data, p.depth - 1, p.sp_id));
}

void  compute_reflection_and_refraction(t_leq l, t_inter i, t_obj *obj,
	t_rayt_param p)
{
	t_3vecf	refr_color;
	t_3vecf refl_color;
	double	fresnel_ratio;

	fresnel_ratio = compute_fresnel_ratio(l.dir, i.normal_inter, obj->refraction);
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


void    compute_reflection_only(t_inter i, t_obj *obj, t_rayt_param p)
{
	t_3vecf refl_color;

	refl_color = get_refl_color(p, i);
	p.lighted_color->val[0] = p.lighted_color->val[0] * (1 - obj->reflection)
													+ refl_color.val[0] * obj->reflection;
	p.lighted_color->val[1] = p.lighted_color->val[1] * (1 - obj->reflection)
													+ refl_color.val[1] * obj->reflection;
	p.lighted_color->val[2] = p.lighted_color->val[2] * (1 - obj->reflection)
													+ refl_color.val[2] * obj->reflection;
}

void    compute_transparency(t_inter i, t_leq l, t_rayt_param p)
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

t_3vecf add_color_effect(t_data *data, t_clre_param p,
	t_3vecf lighted_color, t_inter i)
{
	double	fog_fact;

	if (data->fog.val[0] || data->fog.val[1])
	{
		if (p.closest_dist < data->fog.val[0])
			fog_fact = 1;
		else if (p.closest_dist < data->fog.val[1])
			fog_fact = (data->fog.val[1] - p.closest_dist)
				/ (data->fog.val[1] - data->fog.val[0]);
		else
			fog_fact = 0;
		lighted_color.val[0] = 1 * (1 - fog_fact) + lighted_color.val[0] * fog_fact;
		lighted_color.val[1] = 1 * (1 - fog_fact) + lighted_color.val[1] * fog_fact;
		lighted_color.val[2] = 1 * (1 - fog_fact) + lighted_color.val[2] * fog_fact;
	}
	if (p.depth == RAY_DEPTH)
		return (add_3vecf(lighted_color,
			compute_glare(p.l, data->lights, &(i.inter_point))));
	return (lighted_color);
}

t_inter init_inter(t_leq l, double closest_dist, t_obj *closest_obj, int sp_id)
{
	t_inter i;

	i.inter_point = add_3vecf(l.orig, product_c3vecf(l.dir, closest_dist));
	i.normal_inter = closest_obj->get_normal_inter(i.inter_point,
		closest_obj, sp_id);
	normalize_3vecf(&i.normal_inter);
	return (i);
}

t_dist	new_tdist(double *cdist)
{
	return ((t_dist){cdist, BIAS, MAX_VIEW});
}

t_3vecf	ray_trace(t_leq l, t_data *data, int depth, int sp_id)
{
	double		cdist;
	t_obj			*cobj;
	t_3vecf		clr;
	t_inter		i;
	t_4vecf		o_clr;

	if (!(cobj = ray_first_intersect(l, new_tdist(&cdist), data->objs, sp_id)))
			return (compute_glare(l, data->lights, NULL));
	i = init_inter(l, cdist, cobj, sp_id);
	o_clr = get_obj_color(&i, cobj, l);
	if (init_lighted_color(cobj, i, l, (t_ilc_p){&clr, o_clr, data, sp_id}))
	{
		if (!depth)
			return (clr);
		if (cobj->refraction)
			compute_reflection_and_refraction(l, i, cobj, (t_rayt_param){
				&clr, neg_3vecf(l.dir), o_clr, data, depth, sp_id});
		else if (cobj->reflection)
			compute_reflection_only(i, cobj, (t_rayt_param){&clr,
				neg_3vecf(l.dir), o_clr, data, depth, sp_id});
		else if (o_clr.val[3] > 0)
			compute_transparency(i, l, (t_rayt_param){&clr,
				neg_3vecf(l.dir), o_clr, data, depth, sp_id});
	}
	return (add_color_effect(data, (t_clre_param){cdist, l, depth}, clr, i));
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
		threads[index].start = (int)-data->size.val[0] / 2
												 + (int)data->size.val[0] * index / NB_THREADS;
		threads[index].end = (int)-data->size.val[0] / 2
											 + (int)data->size.val[0] * (index + 1) / NB_THREADS;
		threads[index].data = data;
	}
}

void  check_subsampling(t_data *data, int i, int j, t_3vecf color)
{
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

t_anti_al	init_anti_al(int aa, int offset, int anti_al_iter)
{
	t_anti_al	a;

	a.offset = offset;
	if (anti_al_iter < 1)
	{
		a.aa = 1;
		a.anti_al_iter = 1;
	}
	else
	{
		a.aa = aa;
		a.anti_al_iter = anti_al_iter;
	}
	return (a);
}

t_3vecf init_ray_dir(int i, int j, t_anti_al a, t_data *data)
{
	t_3vecf dir;
	t_3vecf view;

	i = i * a.aa + a.offset / a.aa;
	j = j * a.aa + a.offset % a.aa;
	view = window_to_view(i, j, (int)data->size.val[0] * a.aa,
		(int)data->size.val[1] * a.aa);
	dir = mult_3vecf_33matf(mult_3vecf_33matf(view, data->rot_mat[1]),
		data->rot_mat[0]);
	normalize_3vecf(&dir);
	return (dir);
}

t_3vecf get_stereo_clr(t_3vecf origs[2], t_leq l, t_data *data)
{
	t_3vecf		colors[2];

	if (!data->motion_blur)
	{
		colors[0] = ray_trace((t_leq){origs[0], l.dir}, data, RAY_DEPTH, 0);
		colors[1] = ray_trace((t_leq){origs[1], l.dir}, data, RAY_DEPTH, 0);
	}
	else
	{
		colors[0] = motion_trace(origs[0], l.dir, data);
		colors[1] = motion_trace(origs[1], l.dir, data);
	}
	return (assign_3vecf(colors[1].val[0], colors[0].val[1], colors[0].val[2]));
}

void  compute_stereoscopy(t_data *data, t_leq l, int i, int j)
{
	t_3vecf		origs[2];
	t_3vecf		diff;
	t_3vecf		color;
	t_anti_al	a;

	a = init_anti_al(data->aa_adapt, 0, data->aa_adapt * data->aa_adapt);
	diff = mult_3vecf_33matf(assign_3vecf(0.1, 0, 0), data->rot_mat[1]);
	origs[0] = add_3vecf(l.orig, diff);
	origs[1] = add_3vecf(l.orig, neg_3vecf(diff));
	color = assign_3vecf(0, 0, 0);
	while (a.offset < a.anti_al_iter)
	{
		l.dir = init_ray_dir(i, j, a, data);
		color = add_3vecf(color, get_stereo_clr(origs, l, data));
		a.offset++;
	}
	color = product_c3vecf(color, 1. / a.anti_al_iter);
	ray_put_pixel(i, j, data->mlx->img_str, color, data);
	check_subsampling(data, i, j, color);
}

void  compute_classic(t_data *data, t_leq l, int i, int j)
{
	t_3vecf	clr;
	t_anti_al a;
	t_3vecf color;

	a = init_anti_al(data->aa_adapt, 0, data->aa_adapt * data->aa_adapt);
	color = assign_3vecf(0, 0, 0);
	while (a.offset < a.anti_al_iter)
	{
		l.dir = init_ray_dir(i, j, a, data);
		clr = !data->motion_blur ? ray_trace(l, data, RAY_DEPTH, 0)
														 : motion_trace(l.orig, l.dir, data);
		color = add_3vecf(color, clr);
		a.offset++;
	}
	color = product_c3vecf(color, 1. / a.anti_al_iter);
	ray_put_pixel(i, j, data->mlx->img_str, color, data);
	check_subsampling(data, i, j, color);
}

int   get_aa_step(double aa_adapt)
{
	if (aa_adapt == MIN_AA)
		return (2);
	return (1);
}

void	*render_thread(void *param)
{
	t_leq 		l;
	int				i;
	int				j;
	t_data		*data;
	t_thread	*thread;

	thread = (t_thread *)param;
	data = thread->data;
	i = thread->start;
	l.orig = data->camera->origin;
	while (i < thread->end)
	{
		j = -data->size.val[1] / 2;
		while (j < data->size.val[1] / 2)
		{
			if (data->stereoscopy)
				compute_stereoscopy(data, l, i, j);
			else
				compute_classic(data, l, i, j);
				j += get_aa_step(data->aa_adapt);
		}
		i += get_aa_step(data->aa_adapt);
	}
	pthread_exit(NULL);
	return (NULL);
}

void  init_frames_rot_mat(t_data *data)
{
	t_3vecf tm;
	double  rd;

	tm = mult_3vecf_33matf(assign_3vecf(1, 0, 0), data->rot_mat[1]);
	rd =degree_to_radian(data->camera->rotation.val[0]);
	data->rot_mat[0] = init_rotation_matrix_vec(tm, rd);
}

void	render(t_data *data)
{
	int		ret;
	int		i;
	t_thread	threads_param[NB_THREADS];
	pthread_t	threads[NB_THREADS];

	init_threads(threads_param, data);
	init_frames_rot_mat(data);
	i = 0;
	while (i < NB_THREADS)
	{
		ret = pthread_create(&(threads[i]), NULL, render_thread,
			(void *)&(threads_param[i]));
		if (ret)
			exit(EXIT_FAILURE);
		++i;
	}
	i = 0;
	while (i < NB_THREADS)
		pthread_join(threads[i++], NULL);
}
