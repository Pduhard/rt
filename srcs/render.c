#include "rt.h"

void	print_mat(double mat[4][4])
{
	printf("%+f %+f %+f %+f\n%+f %+f %+f %+f\n%+f %+f %+f %+f\n%+f %+f %+f %+f\n", mat[0][0], mat[0][1], mat[0][2], mat[0][3], mat[1][0], mat[1][1], mat[1][2], mat[1][3], mat[2][0], mat[2][1], mat[2][2], mat[2][3], mat[3][0], mat[3][1], mat[3][2], mat[3][3]);
}

void	print_vec(double vec[3])
{
	printf("x: %f\ny: %f\nz: %f\n", vec[0], vec[1], vec[2]);
}

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

int		check_inside_negative(t_3vecf inter_point, t_obj *negative_objs)
{
	while (negative_objs)
	{
		if (negative_objs->check_inside(inter_point, negative_objs))
			return (1);
		negative_objs = negative_objs->next;
	}
	return (0);
}

t_obj	*ray_first_intersect(t_3vecf orig, t_3vecf dir, double min_dist, double max_dist, double *closest_dist, t_obj *objs, int sp_id, t_data *data)
{
	t_obj	*closest_obj;
	t_obj	*objs_save;

	objs_save = objs;
	closest_obj = NULL;
	*closest_dist = MAX_VIEW;
	while (objs)
	{
		if (objs->ray_intersect(orig, dir, objs, closest_dist, min_dist, max_dist, sp_id))
			closest_obj = objs;
		objs = objs->next;
	}
	if (closest_obj && data->negative_objs)
	{
		t_3vecf inter_point;

		inter_point.val[0] = orig.val[0] + dir.val[0] * *closest_dist;
		inter_point.val[1] = orig.val[1] + dir.val[1] * *closest_dist;
		inter_point.val[2] = orig.val[2] + dir.val[2] * *closest_dist;
		if (check_inside_negative(inter_point, data->negative_objs))
			return (ray_first_intersect(orig, dir, *closest_dist, max_dist, closest_dist, objs_save, sp_id, data));
	/*	min_dist = *closest_dist;
		*closest_dist = MAX_VIEW;
		if (closest_obj->ray_intersect(orig, dir, closest_obj, closest_dist, min_dist, max_dist, sp_id))
		{
	//		exit(0);
			return (ray_first_intersect(orig, dir, *closest_dist, max_dist, closest_dist, objs_save, sp_id));
		}
		else
		{
			*closest_dist = min_dist;
			return (ray_first_intersect(orig, dir, *closest_dist, max_dist, closest_dist, objs_save, sp_id));
			//return (NULL);
		}
		*/
	}
	//	return (check_cuts(orig, dir, closest_obj, min_dist, max_dist, closest_dist, objs_save, sp_id));
	if (closest_obj && closest_obj->cuts)
		return (check_cuts(orig, dir, closest_obj, min_dist, max_dist, closest_dist, objs_save, sp_id, data));
	return (closest_obj);
	(void)data;
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

t_3vecf	refract_ray(t_3vecf ray, t_3vecf normal_inter, double refraction_index, int inside)
{
	t_3vecf	ref;
	double	cosi;
	double	etai;
	double	etat;
	double	eta;
	double	k;

//	printf("%f %f\n", get_length_3vecf(ray), get_length_3vecf(normal_inter));
	cosi = dot_product_3vecf(assign_3vecf(-ray.val[0], -ray.val[1], -ray.val[2]), normal_inter);
//	if (cosi < -1)
//		cosi = -1;
//	else if (cosi > 1)
//		cosi = 1;
	etai = inside ? refraction_index : 1.;
	etat = inside ? 1. : refraction_index;
	eta = etai / etat;
	// SNELL'S LAW
//	printf("%f\n", eta * eta * (1 - cosi * cosi));
	k = 1 - eta * eta * (1 - cosi * cosi);
	k = eta * cosi - sqrt(k);
	ref.val[0] = eta * ray.val[0] + k * normal_inter.val[0];
	ref.val[1] = eta * ray.val[1] + k * normal_inter.val[1];
	ref.val[2] = eta * ray.val[2] + k * normal_inter.val[2];

/*	double	sin_sq_theta;

	etai = 1;
	etat = refraction_index;
	eta = etai / etat;
	sin_sq_theta = eta * eta * 
*/
//	ref.val[0] = k * normal_inter.val[0] - eta * -ray.val[0];
//	ref.val[1] = k * normal_inter.val[1] - eta * -ray.val[1];
//	ref.val[2] = k * normal_inter.val[2] - eta * -ray.val[2];
	return (ref);
}

double	compute_fresnel_ratio(t_3vecf dir, t_3vecf normal_inter, double refraction_index, int inside)
{
/*		TRUE FRESNEL RATIO
 *	double	cosi;
	double	etai;
	double	etat;
	double	sint;
	double	cost;
	double	rs;
	double	rp;

	cosi = dot_product_3vecf(dir, normal_inter);
	if (cosi < -1)
		cosi = -1;
	else if (cosi > 1)
		cosi = 1;
	if (cosi > 0)
	{
		etai = refraction_index;
		etat = 1;
	}
	else
	{
		etat = refraction_index;
		etai = 1;
	}
	sint = 1 - cosi * cosi;
	sint = sint > 0 ? etai / etat * sqrtf(sint) : 0;
	if (sint >= 1)
	{
		printf("total internal reflection\n");
		return (1);
	}
	cost = 1 - sint * sint;
	cost = cost > 0 ? sqrtf(cost) : 0;
	cosi = ft_fabs(cosi);
	rs = (etat * cosi - etai * cost) / (etat * cosi + etai * cost);
	rp = (etai * cosi - etat * cost) / (etai * cosi + etat * cost);
	return ((rs * rs + rp * rp) / 2);
*/
	/*	SCHLICK approximation	3 times faster*/
	//	R0 + (1 - R0)(1 - cos(theta))^5;
	
	double	r0;
	t_3vecf	inv_dir;
	double	cos_theta;
	double	n_cos_theta;

	inv_dir = assign_3vecf(-dir.val[0], -dir.val[1], -dir.val[2]);
	cos_theta = dot_product_3vecf(inv_dir, normal_inter);
	r0 = inside ? (refraction_index - 1) / (refraction_index + 1) : (1 - refraction_index) / (1 + refraction_index);
	r0 *= r0;
	n_cos_theta = 1 - cos_theta;
	n_cos_theta = n_cos_theta * n_cos_theta * n_cos_theta * n_cos_theta * n_cos_theta; 
	return (r0 + (1 - r0) * (1 - cos_theta) * n_cos_theta);
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

t_3vecf	compute_lights(t_3vecf inter_point, t_3vecf normal_inter, t_3vecf dir, t_light *lights, t_obj *objs, int sp_id, t_data *data)
{
	t_3vecf	light_fact;
	double	norm_dot_ldir;
	double	ref_dot_idir;
	double	shadow_dist;
	double	light_len;
	t_3vecf	transp_fact;
	t_3vecf	shadow_inter_point;
	t_3vecf	light_dir;
	t_3vecf	spec_vec;
	t_obj	*shadow_obj;
	t_3vecf	inv_dir = assign_3vecf(-dir.val[0], -dir.val[1], -dir.val[2]);

	light_fact = assign_3vecf(0, 0, 0);
	while (lights)
	{
		if (lights->light_type == LIGHT_AMBIENT)
		{
		//	printf("lights type: AMBIENT param: %f %f %f\n", lights->color.val[0], lights->color.val[1], lights->color.val[2]);
			light_fact.val[0] += lights->color.val[0];
			light_fact.val[1] += lights->color.val[1];
			light_fact.val[2] += lights->color.val[2];
		}
		else
		{
			if (lights->light_type == LIGHT_POINT)
			{
		//		printf("lights type: iPOINT  param: %f %f %f\n", lights->param.val[0], lights->param.val[1], lights->param.val[2]);
		//		printf("lights type: iPOINT  param: %f %f %f\n", lights->color.val[0], lights->color.val[1], lights->color.val[2]);
				light_dir = sub_3vecf(lights->param, inter_point);
				light_len = get_length_3vecf(light_dir);
			}
			else if (lights->light_type == LIGHT_DIRECTIONAL)
			{
		//		printf("lights type: DIR  param: %f %f %f\n", lights->param.val[0], lights->param.val[1], lights->param.val[2]);
				light_dir = assign_3vecf(-lights->param.val[0], -lights->param.val[1], -lights->param.val[2]);
				light_len = MAX_VIEW;
			}
			//get_length_3vecf(light_dir);
			normalize_3vecf(&light_dir);// same
			shadow_inter_point = inter_point;
			transp_fact = assign_3vecf(1, 1, 1);
			while ((shadow_obj = ray_first_intersect(shadow_inter_point, light_dir, BIAS, light_len, &shadow_dist, objs, sp_id, data)))
			{
		//		printf("wefwef\n");
				shadow_inter_point.val[0] += (light_dir.val[0] * shadow_dist);
				shadow_inter_point.val[1] += (light_dir.val[1] * shadow_dist);
				shadow_inter_point.val[2] += (light_dir.val[2] * shadow_dist);
				light_len -= shadow_dist;	
				t_3vecf tex_normal_inter = shadow_obj->get_normal_inter(shadow_inter_point, shadow_obj, sp_id);
				normalize_3vecf(&tex_normal_inter);
		//		if (dot_product_3vecf(tex_normal_inter, light_dir) < 0)
	//				tex_normal_inter = assign_3vecf(-tex_normal_inter.val[0], -tex_normal_inter.val[1], -tex_normal_inter.val[2]);
				t_4vecf obj_color = shadow_obj->get_text_color(shadow_inter_point, tex_normal_inter, shadow_obj);
				if (obj_color.val[3] > 0)
				{
		//			break ; //!!!!!!!!!!!!!!!!!!!! need to manage shadow
	/*				t_3vecf	trans;
					
					trans.val[0] = ((1 - obj_color.val[3]) * (1 - obj_color.val[0]));
					trans.val[1] = ((1 - obj_color.val[3]) * (1 - obj_color.val[1]));
					trans.val[2] = ((1 - obj_color.val[3]) * (1 - obj_color.val[2]));
					transp_fact.val[0] -= trans.val[0] ? trans.val[0] : 0.1; // ->transmitance !!
					transp_fact.val[1] -= trans.val[1] ? trans.val[1] : 0.1; // ->transmitance !!
					transp_fact.val[2] -= trans.val[2] ? trans.val[2] : 0.1; // ->transmitance !!
	*/				transp_fact.val[0] -= ((1 - obj_color.val[3]) * (1.1 - obj_color.val[0])); // ->transmitance !!
					transp_fact.val[1] -= ((1 - obj_color.val[3]) * (1.1 - obj_color.val[1])); // ->transmitance !!
					transp_fact.val[2] -= ((1 - obj_color.val[3]) * (1.1 - obj_color.val[2])); // ->transmitance !!
				}
				else
					break;
			}
		//	shadow_obj = ray_first_intersect(inter_point, light_dir, 0.01, light_len, &shadow_dist, objs, sp_id);
	//		transp_fact = 1;
		//	printf("flute\n");
		//	shadow_obj = NULL;//ray_first_intersect(inter_point, light_dir, 0.001, light_len, &shadow_dist, objs);
		//	printf("%f %f %f\n", transp_fact.val[0], transp_fact.val[1], transp_fact.val[2]);
			if (transp_fact.val[0] <= 0)
				transp_fact.val[0] = 0;
			if (transp_fact.val[1] <= 0)
				transp_fact.val[1] = 0;
			if (transp_fact.val[2] <= 0)
				transp_fact.val[2] = 0;
	/*		if (transp_fact.val[0] + transp_fact.val[1] + transp_fact.val[2] < 3 || shadow_obj)
			{	
				t_3vecf	global;
				global = compute_global_illumination(inter_point, normal_inter, data);
				light_fact.val[0] += global.val[0];
				light_fact.val[1] += global.val[1];
				light_fact.val[2] += global.val[2];
			}
	*/		if (!shadow_obj && transp_fact.val[0] + transp_fact.val[1] + transp_fact.val[2] > 0)// || shadow_dist > get_length_3vecf(light_dir))
			{
			//	printf("wefwef\n");
				norm_dot_ldir = dot_product_3vecf(normal_inter, light_dir);
				if (norm_dot_ldir > 0)
				{
					light_fact.val[0] += lights->color.val[0] * transp_fact.val[0] * norm_dot_ldir /  get_length_3vecf(light_dir);
					light_fact.val[1] += lights->color.val[1] * transp_fact.val[1] * norm_dot_ldir /  get_length_3vecf(light_dir);
					light_fact.val[2] += lights->color.val[2] * transp_fact.val[2] * norm_dot_ldir /  get_length_3vecf(light_dir);
				}

				if (objs->shininess)
				{
					spec_vec = reflect_ray(light_dir, normal_inter);
				/*	spec_vec.val[0] = 2 * normal_inter.val[0] * norm_dot_ldir - light_dir.val[0];
					spec_vec.val[1] = 2 * normal_inter.val[1] * norm_dot_ldir - light_dir.val[1];
					spec_vec.val[2] = 2 * normal_inter.val[2] * norm_dot_ldir - light_dir.val[2];
				*/	ref_dot_idir = dot_product_3vecf(spec_vec, inv_dir);
					if (ref_dot_idir > 0)// && !CEL_SHADING)
					{
						light_fact.val[0] += lights->color.val[0] * transp_fact.val[0] * powf(ref_dot_idir / (get_length_3vecf(spec_vec) * get_length_3vecf(inv_dir)), objs->shininess);
						light_fact.val[1] += lights->color.val[1] * transp_fact.val[1] * powf(ref_dot_idir / (get_length_3vecf(spec_vec) * get_length_3vecf(inv_dir)), objs->shininess);
						light_fact.val[2] += lights->color.val[2] * transp_fact.val[2] * powf(ref_dot_idir / (get_length_3vecf(spec_vec) * get_length_3vecf(inv_dir)), objs->shininess);
			
					}
				}
			}
		/*	else if (GLOBAL_ILLUMINATION) {
				t_3vecf	global;
				global = compute_global_illumination(inter_point, normal_inter, data);
				light_fact.val[0] += global.val[0];
				light_fact.val[1] += global.val[1];
				light_fact.val[2] += global.val[2];
			}*/
		}
		lights = lights->next;
	}
	
	if (CAUSTIC_GI)
	{
		t_3vecf	global;

	//	return (compute_global_illumination(inter_point, normal_inter, data));
		global = compute_global_illumination(inter_point, normal_inter, data->caustic_map, MAX_CAUSTIC_RADIUS, NN_CAUSTIC_PHOTON_MAX);
		light_fact.val[0] += global.val[0];
		light_fact.val[1] += global.val[1];
		light_fact.val[2] += global.val[2];
	}
	if (INDIRECT_GI)
	{
		t_3vecf	global;

	//	return (compute_global_illumination(inter_point, normal_inter, data));
		global = compute_global_illumination(inter_point, normal_inter, data->indirect_map, MAX_INDIRECT_RADIUS, NN_INDIRECT_PHOTON_MAX);
		light_fact.val[0] += global.val[0];
		light_fact.val[1] += global.val[1];
		light_fact.val[2] += global.val[2];
	}

	if (CEL_SHADING)
	{
		cel_shade(&(light_fact.val[0]));
		cel_shade(&(light_fact.val[1]));
		cel_shade(&(light_fact.val[2]));
	}
	return (light_fact);
}

int		is_on_cell_boundary(t_3vecf origin, t_3vecf inter_point, t_3vecf normal_inter, t_obj *closest_obj, int sp_id)
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
	if (closest_obj->ray_intersect(origin, new_dir, closest_obj, &dist, 2 * CEL_BOUNDARY, MAX_VIEW, sp_id))
		return (0);
	return (1);
}

t_3vecf		compute_glare(t_3vecf orig, t_3vecf dir, t_light *lights, t_3vecf *inter_point)
{
	t_3vecf	glare;
	t_3vecf	light_dir;
	t_3vecf	light_pos;
	double	obj_dist;
	double	light_dist;

	obj_dist = inter_point ? get_length_3vecf(sub_3vecf(*inter_point, orig)) : MAX_VIEW;
	glare = assign_3vecf(0, 0, 0);
	while (lights)
	{
		if (lights->light_type != LIGHT_AMBIENT)
		{
			if (lights->light_type == LIGHT_POINT)
			{
				light_dir = sub_3vecf(lights->param, orig);
				light_dist = get_length_3vecf(light_dir);
				normalize_3vecf(&light_dir);
				light_pos = lights->param;
			}	
			if (light_dist < obj_dist)
			{
				double fact;
				t_3vecf	light_npoint;

				light_npoint.val[0] = orig.val[0] + dir.val[0] * light_dist;
				light_npoint.val[1] = orig.val[1] + dir.val[1] * light_dist;
				light_npoint.val[2] = orig.val[2] + dir.val[2] * light_dist;
				double	lndist;
				lndist = get_length_3vecf(sub_3vecf(light_pos, light_npoint));
				if (lndist < 0.2)
					fact = (0.2 - lndist) * 5;
				else
					fact = 0;
				/*dot_product_3vecf(dir, light_dir);
				if (fact > 0.9)
				fact = exp(fact) - exp(1. - 0.05 / light_dist);
				fact = fact < 0 ? 0 : fact;
				fact *= exp(fact * 400);
				*/glare.val[0] += fact;
				glare.val[1] += fact;
				glare.val[2] += fact;
			}
		}
		lights = lights->next;
	}
	(void)dir;
	return (glare);
}

t_3vecf	ray_trace(t_3vecf orig, t_3vecf dir, double min_dist, double max_dist, t_data *data, int depth, int sp_id)
{
	double	closest_dist;
	t_obj	*closest_obj;
	t_3vecf		light_fact;

	closest_obj = ray_first_intersect(orig, dir, min_dist, max_dist, &closest_dist, data->objs, sp_id, data);
	if (!closest_obj)
	{
		light_fact = compute_glare(orig, dir, data->lights, NULL);
	/*	if (light_fact.val[0] > 1)
			light_fact.val[0] = 1;
		if (light_fact.val[1] > 1)
			light_fact.val[1] = 1;
		if (light_fact.val[2] > 1)
			light_fact.val[2] = 1;
	*/	return (light_fact);
	}
	t_3vecf		inter_point;
	t_3vecf		normal_inter;
	t_3vecf		lighted_color;
	t_3vecf		inv_dir =  assign_3vecf(-dir.val[0], -dir.val[1], -dir.val[2]);
	t_4vecf		obj_color;

	inter_point.val[0] = orig.val[0] + dir.val[0] * closest_dist;
	inter_point.val[1] = orig.val[1] + dir.val[1] * closest_dist;
	inter_point.val[2] = orig.val[2] + dir.val[2] * closest_dist;
	normal_inter = closest_obj->get_normal_inter(inter_point, closest_obj, sp_id);
	normalize_3vecf(&normal_inter);
	t_3vecf	tex_normal_inter = normal_inter;
	int inside = 0;
	if (dot_product_3vecf(normal_inter, dir) > 0 && (inside = 1))
		normal_inter = assign_3vecf(-normal_inter.val[0], -normal_inter.val[1], -normal_inter.val[2]);
	if (closest_obj->get_bump_mapping)
		normal_inter = closest_obj->get_bump_mapping(inter_point, normal_inter, closest_obj);
	inside = 0;
	obj_color = closest_obj->get_text_color(inter_point, tex_normal_inter, closest_obj);
	light_fact = compute_lights(inter_point, normal_inter, dir, data->lights, data->objs, sp_id, data);
	lighted_color.val[0] = obj_color.val[0] * light_fact.val[0];
	lighted_color.val[1] = obj_color.val[1] * light_fact.val[1];
	lighted_color.val[2] = obj_color.val[2] * light_fact.val[2];
	if (CEL_SHADING && is_on_cell_boundary(orig, inter_point, normal_inter, closest_obj, sp_id))
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
		double fresnel_ratio = compute_fresnel_ratio(dir, normal_inter, closest_obj->refraction, inside);
//		fresnel_ratio = 0;
//		printf("%f\n", fresnel_ratio);
		if (fresnel_ratio < 0.999999) // else reflection
		{
			t_3vecf	refr_ray = refract_ray(dir, normal_inter, closest_obj->refraction, inside);
			normalize_3vecf(&refr_ray);
			refr_color = ray_trace(inter_point, refr_ray, BIAS, MAX_VIEW, data, depth - 1, sp_id);
		}
		if (fresnel_ratio > 0.01) // eles full refraction
		{
			t_3vecf	refl_ray = reflect_ray(inv_dir, normal_inter);
			normalize_3vecf(&refl_ray);
			refl_color = ray_trace(inter_point, refl_ray, BIAS, MAX_VIEW, data, depth - 1, sp_id);
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
		t_3vecf	refl_color = ray_trace(inter_point, refl_ray, BIAS, MAX_VIEW, data, depth - 1, sp_id);
	
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
		//double fresnel_ratio = compute_fresnel_ratio(dir, normal_inter, closest_obj->refraction);
	/*	light_fact = compute_lights(inter_point, normal_inter, inv_dir, data->lights, data->objs);
		lighted_color.val[0] = closest_obj->color.val[0] * light_fact;
		lighted_color.val[1] = closest_obj->color.val[1] * light_fact;
		lighted_color.val[2] = closest_obj->color.val[2] * light_fact;*/
//		if (fresnel_ratio < 1)
//		{
			//t_3vecf	refr_ray = refract_ray(dir, normal_inter, closest_obj->refraction);
			//normalize_3vecf(&refr_ray);
		refr_color = ray_trace(inter_point, dir, BIAS, MAX_VIEW, data, depth - 1, sp_id);
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
	if (FOG)
	{
		
		double	fog_fact;

		if (closest_dist < FOG_NEAR)
			fog_fact = 1;
		else if (closest_dist < FOG_FAR)
			fog_fact = (FOG_FAR - closest_dist) / (FOG_FAR - FOG_NEAR);
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
		t_3vecf	glare = compute_glare(orig, dir, data->lights, &inter_point);
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
	t_3vecf	orig;
	t_3vecf	dir;
	t_3vecf	color;
	int		i;
	int		j;
	t_thread	*thread = (t_thread *)param;
	t_data	*data = thread->data;

	i = thread->start;
	orig = data->camera->origin;
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
				origs[0] = orig;
				origs[1] = orig;
				origs[0].val[0] += diff.val[0];
				origs[0].val[1] += diff.val[1];
				origs[0].val[2] += diff.val[2];
				origs[1].val[0] -= diff.val[0];
				origs[1].val[1] -= diff.val[1];
				origs[1].val[2] -= diff.val[2];
				dir = mult_3vecf_33matf(mult_3vecf_33matf(window_to_view(i, j, data->size.val[0], data->size.val[1]), data->rot_mat[1]), data->rot_mat[0]);
				normalize_3vecf(&dir);
				if (!data->motion_blur)
				{
					colors[0] = ray_trace(origs[0], dir, BIAS, MAX_VIEW, data, RAY_DEPTH, 0);
					colors[1] = ray_trace(origs[1], dir, BIAS, MAX_VIEW, data, RAY_DEPTH, 0);
				}
				else
				{
					colors[0] = motion_trace(origs[0], dir, data);
					colors[1] = motion_trace(origs[1], dir, data);
				}
				color.val[0] = colors[1].val[0];
				color.val[1] = colors[0].val[1];
				color.val[2] = colors[0].val[2];
				ray_put_pixel(i, j, data->mlx->img_str, color, data);
			}
			else if (!ANTI_AL)
			{

				dir = mult_3vecf_33matf(mult_3vecf_33matf(window_to_view(i, j, data->size.val[0], data->size.val[1]), data->rot_mat[1]), data->rot_mat[0]);
				normalize_3vecf(&dir);
				if (!data->motion_blur)
					color = ray_trace(orig, dir, BIAS, MAX_VIEW, data, RAY_DEPTH, 0);
				else
					color = motion_trace(orig, dir, data);
				ray_put_pixel(i, j, data->mlx->img_str, color, data);
			}
		/*	else
			{
				t_3vecf	clr;
				int		anti_all_iter;
				int		offset;

				offset = 0;
				anti_all_iter = ANTI_AL * ANTI_AL;
				color = assign_3vecf(0, 0, 0);
				while (offset < anti_all_iter)
				{
					dir = mult_3vecf_33matf(mult_3vecf_33matf(window_to_view(i * ANTI_AL + offset / ANTI_AL, j * ANTI_AL + offset % ANTI_AL, WIN_WIDTH * ANTI_AL, WIN_HEIGHT * ANTI_AL), data->rot_mat[1]), data->rot_mat[0]);
					clr = ray_trace(orig, dir, 0.01, MAX_VIEW, data, 6);
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
		*/	++j;
		}
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
//	orig = data->camera->origin;
	//dir = mult_3vecf_33matf(window_to_view(0, 0), data->rot_mat[1]);
	//printf("%f %f %f\n", dir.val[0], dir.val[1], dir.val[2]);
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
