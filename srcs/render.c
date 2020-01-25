/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   render.c                                         .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: pduhard- <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2019/12/21 22:42:45 by pduhard-     #+#   ##    ##    #+#       */
/*   Updated: 2020/01/25 20:58:45 by pduhard-    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "rt.h"

void	print_mat(double mat[4][4])
{
	printf("%+f %+f %+f %+f\n%+f %+f %+f %+f\n%+f %+f %+f %+f\n%+f %+f %+f %+f\n", mat[0][0], mat[0][1], mat[0][2], mat[0][3], mat[1][0], mat[1][1], mat[1][2], mat[1][3], mat[2][0], mat[2][1], mat[2][2], mat[2][3], mat[3][0], mat[3][1], mat[3][2], mat[3][3]);
}

void	print_vec(double vec[3])
{
	printf("x: %f\ny: %f\nz: %f\n", vec[0], vec[1], vec[2]);
}

t_3vecf	window_to_view(int x, int y)
{
	t_3vecf	vec;

		vec.val[0] = (float)x / (float)WIN_WIDTH;
			vec.val[1] = (float)y / (float)WIN_HEIGHT;
				if (WIN_WIDTH > WIN_HEIGHT)
							vec.val[0] *= (float)WIN_WIDTH / (float)WIN_HEIGHT;
					else if (WIN_HEIGHT > WIN_WIDTH)
								vec.val[1] *= (float)WIN_HEIGHT / (float)WIN_WIDTH;
						vec.val[2] = 1;
							return (vec);
}

t_obj	*ray_first_intersect(t_3vecf orig, t_3vecf dir, double min_dist, double max_dist, double *closest_dist, t_obj *objs)
{
	t_obj	*closest_obj;

	closest_obj = NULL;
	*closest_dist = FLT_MAX;
	while (objs)
	{
		//objs->ray_interset() could be smart
		if (objs->ray_intersect(orig, dir, objs, closest_dist, min_dist, max_dist))
			closest_obj = objs;
		///else if (objs->obj_type == OBJ_PLANE && ray_intersect_plane(orig, dir, objs, closest_dist, min_dist, max_dist))
		//	closest_obj = objs;
/*		if (hit_point.val[0] < *closest_dist && min_dist < hit_point.val[0] && hit_point.val[0] < max_dist)
		{
			*closest_dist = hit_point.val[0];
		}
		if (hit_point.val[1] < *closest_dist && min_dist < hit_point.val[1] && hit_point.val[1] < max_dist)
		{
			*closest_dist = hit_point.val[1];
			closest_obj = objs;
		}
*/		objs = objs->next;
	}
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

t_3vecf	refract_ray(t_3vecf ray, t_3vecf normal_inter, double refraction_index)
{
	t_3vecf	ref;
	double	cosi;
	double	etai;
	double	etat;
	double	eta;
	double	k;

	cosi = dot_product_3vecf(ray, normal_inter);
	if (cosi < -1)
		cosi = -1;
	else if (cosi > 1)
		cosi = 1;
	cosi = -cosi;
	etai = 1;
	etat = refraction_index;
	eta = etai / etat;
	k = 1 - eta * eta * (1 - cosi * cosi);
	if (k < 0)
		return (assign_3vecf(0, 0, 0));
	ref.val[0] = eta * ray.val[0] + (eta * cosi - sqrtf(k)) * normal_inter.val[0];
	ref.val[1] = eta * ray.val[1] + (eta * cosi - sqrtf(k)) * normal_inter.val[1];
	ref.val[2] = eta * ray.val[2] + (eta * cosi - sqrtf(k)) * normal_inter.val[2];
	return (ref);
}

double	compute_fresnel_ratio(t_3vecf dir, t_3vecf normal_inter, double refraction_index)
{
	double	cosi;
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
}

t_3vecf	compute_lights(t_3vecf inter_point, t_3vecf normal_inter, t_3vecf inv_dir, t_light *lights, t_obj *objs)
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

	light_fact = assign_3vecf(0, 0, 0);
	while (lights)
	{
		if (lights->light_type == LIGHT_AMBIENT)
		{
			light_fact.val[0] += lights->intensity.val[0];
			light_fact.val[1] += lights->intensity.val[1];
			light_fact.val[2] += lights->intensity.val[2];
		}
		else
		{
			if (lights->light_type == LIGHT_POINT)
			{
				light_dir = sub_3vecf(lights->origin, inter_point);
				light_len = get_length_3vecf(light_dir);
			}
			else if (lights->light_type == LIGHT_DIRECTIONAL)
			{
				light_dir = assign_3vecf(-lights->origin.val[0], -lights->origin.val[1], -lights->origin.val[2]);
				light_len = FLT_MAX;
			}
			//get_length_3vecf(light_dir);
			normalize_3vecf(&light_dir);// same
			shadow_inter_point = inter_point;
			transp_fact = assign_3vecf(1, 1, 1);
			while ((shadow_obj = ray_first_intersect(shadow_inter_point, light_dir, 0.01, light_len, &shadow_dist, objs)))
			{
		//		printf("wefwef\n");
				if (shadow_obj->refraction > 0)
				{
					t_3vecf obj_color = shadow_obj->get_text_color(inter_point, normal_inter, shadow_obj);
					transp_fact.val[0] -= shadow_obj->refraction > 1 ? 0.1/* false */ : ((1 -shadow_obj->refraction) * (1 - obj_color.val[0])); // ->transmitance !!
					transp_fact.val[1] -= shadow_obj->refraction > 1 ? 0.1/* false */ : ((1 - shadow_obj->refraction) * (1 - obj_color.val[1])); // ->transmitance !!
					transp_fact.val[2] -= shadow_obj->refraction > 1 ? 0.1/* false */ : ((1 - shadow_obj->refraction) * (1 - obj_color.val[2])); // ->transmitance !!
				}
				else
					break;
				shadow_inter_point.val[0] += (light_dir.val[0] * shadow_dist);
				shadow_inter_point.val[1] += (light_dir.val[1] * shadow_dist);
				shadow_inter_point.val[2] += (light_dir.val[2] * shadow_dist);
				light_len -= shadow_dist;
			}
	//		shadow_obj = ray_first_intersect(inter_point, light_dir, 0.01, light_len, &shadow_dist, objs);
	//		transp_fact = 1;
		//	printf("flute\n");
		//	shadow_obj = NULL;//ray_first_intersect(inter_point, light_dir, 0.001, light_len, &shadow_dist, objs);
		//	printf("%f %f %f\n", shadow_obj->color.val[0], shadow_obj->color.val[1], shadow_obj->color.val[2]);
			if (transp_fact.val[0] < 0)
				transp_fact.val[0] = 0;
			if (transp_fact.val[1] < 0)
				transp_fact.val[1] = 0;
			if (transp_fact.val[2] < 0)
				transp_fact.val[2] = 0;
			if (!shadow_obj && transp_fact.val[0] + transp_fact.val[1] + transp_fact.val[2] > 0)// || shadow_dist > get_length_3vecf(light_dir))
			{
			//	printf("wefwef\n");
				norm_dot_ldir = dot_product_3vecf(normal_inter, light_dir);
				if (norm_dot_ldir > 0)
				{
					light_fact.val[0] += lights->intensity.val[0] * transp_fact.val[0] * norm_dot_ldir /  get_length_3vecf(light_dir);
					light_fact.val[1] += lights->intensity.val[1] * transp_fact.val[1] * norm_dot_ldir /  get_length_3vecf(light_dir);
					light_fact.val[2] += lights->intensity.val[2] * transp_fact.val[2] * norm_dot_ldir /  get_length_3vecf(light_dir);
				}
				spec_vec = reflect_ray(light_dir, normal_inter);
			/*	spec_vec.val[0] = 2 * normal_inter.val[0] * norm_dot_ldir - light_dir.val[0];
				spec_vec.val[1] = 2 * normal_inter.val[1] * norm_dot_ldir - light_dir.val[1];
				spec_vec.val[2] = 2 * normal_inter.val[2] * norm_dot_ldir - light_dir.val[2];
			*/	ref_dot_idir = dot_product_3vecf(spec_vec, inv_dir);
				if (ref_dot_idir > 0)
				{
					light_fact.val[0] += lights->intensity.val[0] * transp_fact.val[0] * powf(ref_dot_idir / (get_length_3vecf(spec_vec) * get_length_3vecf(inv_dir)), 100);
					light_fact.val[1] += lights->intensity.val[1] * transp_fact.val[1] * powf(ref_dot_idir / (get_length_3vecf(spec_vec) * get_length_3vecf(inv_dir)), 100);
					light_fact.val[2] += lights->intensity.val[2] * transp_fact.val[2] * powf(ref_dot_idir / (get_length_3vecf(spec_vec) * get_length_3vecf(inv_dir)), 100);
			
				}
			}
		}
		lights = lights->next;
	}
	return (light_fact);
}

t_3vecf	ray_trace(t_3vecf orig, t_3vecf dir, double min_dist, double max_dist, t_data *data, int depth)
{
	double	closest_dist;
	t_obj	*closest_obj;

//	printf("orig then dir\n");
//	print_vec(orig.val);
//	print_vec(dir.val);
//	if (!depth)
//		return (assign_3vecf(0, 0, 0));
		//return (lighted_color);
	closest_obj = ray_first_intersect(orig, dir, min_dist, max_dist, &closest_dist, data->objs);
	if (!closest_obj)
		return (assign_3vecf(0, 0, 0));
	//
	// new function i think ^^
	//
	//return (closest_obj->color);
//	if (closest_obj->obj_type == OBJ_CONE)
//		return (closest_obj->color);
	t_3vecf		inter_point;
	t_3vecf		normal_inter;
	t_3vecf		lighted_color;
	t_3vecf		inv_dir =  assign_3vecf(-dir.val[0], -dir.val[1], -dir.val[2]);
	t_3vecf		light_fact;
//	double		normal_length;

	inter_point.val[0] = orig.val[0] + dir.val[0] * closest_dist;
	inter_point.val[1] = orig.val[1] + dir.val[1] * closest_dist;
	inter_point.val[2] = orig.val[2] + dir.val[2] * closest_dist;
	normal_inter = closest_obj->get_normal_inter(inter_point, closest_obj);
	normalize_3vecf(&normal_inter);
	if (dot_product_3vecf(normal_inter, dir) > 0)
		normal_inter = assign_3vecf(-normal_inter.val[0], -normal_inter.val[1], -normal_inter.val[2]);
	if (closest_obj->obj_type == OBJ_PLANE)
	{
		double	bump_factor = 0.03;
		t_3vecf	normal_inter_save = normal_inter;
	
		normal_inter.val[0] += inter_point.val[0] + data->f;
		normal_inter.val[1] = 50 * normal_inter.val[1] + data->f + 100 * inter_point.val[1] + data->f ;
		normal_inter.val[2] += inter_point.val[2] + data->f;
	
		double	bump_x = compute_perlin_factor(assign_3vecf(normal_inter.val[0] - bump_factor, normal_inter.val[1], normal_inter.val[2]))
			-compute_perlin_factor(assign_3vecf(normal_inter.val[0] + bump_factor, normal_inter.val[1], normal_inter.val[2]));
		double	bump_y = compute_perlin_factor(assign_3vecf(normal_inter.val[0], normal_inter.val[1] - bump_factor, normal_inter.val[2]))
			-compute_perlin_factor(assign_3vecf(normal_inter.val[0], normal_inter.val[1] + bump_factor, normal_inter.val[2]));
		double	bump_z = compute_perlin_factor(assign_3vecf(normal_inter.val[0], normal_inter.val[1], normal_inter.val[2] - bump_factor))
			-compute_perlin_factor(assign_3vecf(normal_inter.val[0], normal_inter.val[1], normal_inter.val[2] + bump_factor));
	
	/*	double	bump_x = compute_wood_factor(assign_3vecf(normal_inter.val[0] - bump_factor, normal_inter.val[1], normal_inter.val[2]))
		-compute_wood_factor(assign_3vecf(normal_inter.val[0] + bump_factor, normal_inter.val[1], normal_inter.val[2]));
	double	bump_y = compute_wood_factor(assign_3vecf(normal_inter.val[0], normal_inter.val[1] - bump_factor, normal_inter.val[2]))
		-compute_wood_factor(assign_3vecf(normal_inter.val[0], normal_inter.val[1] + bump_factor, normal_inter.val[2]));
	double	bump_z = compute_wood_factor(assign_3vecf(normal_inter.val[0], normal_inter.val[1], normal_inter.val[2] - bump_factor))
		-compute_wood_factor(assign_3vecf(normal_inter.val[0], normal_inter.val[1], normal_inter.val[2] + bump_factor));

	double	bump_x = compute_marble_factor(assign_3vecf(normal_inter.val[0] - bump_factor, normal_inter.val[1], normal_inter.val[2]), normal_inter_save, closest_obj)
		-compute_marble_factor(assign_3vecf(normal_inter.val[0] + bump_factor, normal_inter.val[1], normal_inter.val[2]), normal_inter_save, closest_obj);
	double	bump_y = compute_marble_factor(assign_3vecf(normal_inter.val[0], normal_inter.val[1] - bump_factor, normal_inter.val[2]), normal_inter_save, closest_obj)
		-compute_marble_factor(assign_3vecf(normal_inter.val[0], normal_inter.val[1] + bump_factor, normal_inter.val[2]), normal_inter, closest_obj);
	double	bump_z = compute_marble_factor(assign_3vecf(normal_inter.val[0], normal_inter.val[1], normal_inter.val[2] - bump_factor), normal_inter, closest_obj)
		-compute_marble_factor(assign_3vecf(normal_inter.val[0], normal_inter.val[1], normal_inter.val[2] + bump_factor), normal_inter, closest_obj);

*/
		normal_inter.val[0] = normal_inter_save.val[0] + bump_x;
		normal_inter.val[1] = normal_inter_save.val[1] + bump_y;
		normal_inter.val[2] = normal_inter_save.val[2] + bump_z;
	}
//	if (closest_obj->obj_type != OBJ_PLANE)
/*	else
	{
		normal_inter.val[0] = normal_inter_save.val[0];
		normal_inter.val[1] = normal_inter_save.val[1];
		normal_inter.val[2] = normal_inter_save.val[2];
	}
*/

/*	normal_length = get_length_3vecf(normal_inter);
	normal_inter.val[0] /= normal_length;
	normal_inter.val[1] /= normal_length;
	normal_inter.val[2] /= normal_length;
*/	
	t_3vecf	obj_color;
	obj_color = closest_obj->get_text_color(inter_point, normal_inter, closest_obj);
	light_fact = compute_lights(inter_point, normal_inter, inv_dir, data->lights, data->objs);
	lighted_color.val[0] = obj_color.val[0] * light_fact.val[0];
	lighted_color.val[1] = obj_color.val[1] * light_fact.val[1];
	lighted_color.val[2] = obj_color.val[2] * light_fact.val[2];

	if (closest_obj->refraction < 1 && closest_obj->reflection > 0) // reflection
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
		t_3vecf	refl_color = ray_trace(inter_point, refl_ray, 0.01, FLT_MAX, data, depth - 1);
	
		lighted_color.val[0] = lighted_color.val[0] * (1 - closest_obj->reflection) + refl_color.val[0] * closest_obj->reflection;
		lighted_color.val[1] = lighted_color.val[1] * (1 - closest_obj->reflection) + refl_color.val[1] * closest_obj->reflection;
		lighted_color.val[2] = lighted_color.val[2] * (1 - closest_obj->reflection) + refl_color.val[2] * closest_obj->reflection;
		//lighted_color.val[1] = closest_obj->color.val[1] * light_fact;
		//lighted_color.val[2] = closest_obj->color.val[2] * light_fact;
	}
	else if (closest_obj->refraction > 1) // reflection and refraction
	{
		t_3vecf	refr_color = assign_3vecf(0, 0, 0);
		if (!depth)
			return (assign_3vecf(0, 0, 0));
			//return (lighted_color);
		double fresnel_ratio = compute_fresnel_ratio(dir, normal_inter, closest_obj->refraction);
		if (fresnel_ratio < 1)
		{
			t_3vecf	refr_ray = refract_ray(dir, normal_inter, closest_obj->refraction);
			normalize_3vecf(&refr_ray);
			refr_color = ray_trace(inter_point, refr_ray, 0.01, FLT_MAX, data, depth - 1);
		}
		t_3vecf	refl_ray = reflect_ray(inv_dir, normal_inter);
		normalize_3vecf(&refl_ray);
		t_3vecf	refl_color = ray_trace(inter_point, refl_ray, 0.01, FLT_MAX, data, depth - 1);

		/*	not sure */	
/*		light_fact = compute_lights(inter_point, normal_inter, inv_dir, data->lights, data->objs);
		lighted_color.val[0] = closest_obj->color.val[0] * light_fact;
		lighted_color.val[1] = closest_obj->color.val[1] * light_fact;
		lighted_color.val[2] = closest_obj->color.val[2] * light_fact;*/

//		refr_color.val[0] = lighted_color.val[0] * (1 - closest_obj->reflection) + refr_color.val[0] * closest_obj->reflection; // !!!! ->reflection => ->transmitance
//		refr_color.val[1] = lighted_color.val[1] * (1 - closest_obj->reflection) + refr_color.val[1] * closest_obj->reflection;
//		refr_color.val[2] = lighted_color.val[2] * (1 - closest_obj->reflection) + refr_color.val[2] * closest_obj->reflection;
		/*	-------- */
		
		lighted_color.val[0] = refr_color.val[0] * (1 - fresnel_ratio) + refl_color.val[0] * fresnel_ratio;
		lighted_color.val[1] = refr_color.val[1] * (1 - fresnel_ratio) + refl_color.val[1] * fresnel_ratio;
		lighted_color.val[2] = refr_color.val[2] * (1 - fresnel_ratio) + refl_color.val[2] * fresnel_ratio;
	}
	else if (closest_obj->refraction <= 1) // transparency
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
			refr_color = ray_trace(inter_point, dir, 0.01, FLT_MAX, data, depth - 1);
//		}
	//	t_3vecf	refl_ray = reflect_ray(inv_dir, normal_inter);
	//	normalize_3vecf(&refl_ray);
	//	t_3vecf	refl_color = ray_trace(inter_point, refl_ray, 0.01, FLT_MAX, data, depth - 1);
		lighted_color.val[0] = lighted_color.val[0] * (1 - closest_obj->refraction) + refr_color.val[0] * closest_obj->refraction;
		lighted_color.val[1] = lighted_color.val[1] * (1 - closest_obj->refraction) + refr_color.val[1] * closest_obj->refraction;
		lighted_color.val[2] = lighted_color.val[2] * (1 - closest_obj->refraction) + refr_color.val[2] * closest_obj->refraction;
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
*/	return (lighted_color);
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

void	ray_put_pixel(int i, int j, int *img, t_3vecf color)
{
	int		rgb_color;

	i = WIN_WIDTH / 2 + i;
	j = WIN_HEIGHT / 2 + j;
	rgb_color = (clip_color(color.val[0] * 255) << 16);
	rgb_color |= (clip_color(color.val[1] * 255) << 8);
	rgb_color |= clip_color(color.val[2] * 255);
	img[j * WIN_WIDTH + i] = rgb_color;
}

void	init_threads(t_thread threads[NB_THREADS], t_data *data)
{
	int		index = -1;

	while (++index < NB_THREADS)
	{
		threads[index].start = -WIN_WIDTH / 2 + WIN_WIDTH * index / NB_THREADS;
		threads[index].end = -WIN_WIDTH / 2 + WIN_WIDTH * (index + 1) / NB_THREADS;
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
		j = -WIN_HEIGHT / 2;
		while (j < WIN_HEIGHT / 2)
		{
			
			dir = /*mult_3vecf_33matf(*/mult_3vecf_33matf(mult_3vecf_33matf(window_to_view(i, j), data->rot_mat[1]), data->rot_mat[0])/*, data->rot_mat[2])*/;
			//dir = window_to_view(i, j);
			/*dir = mult_3vecf_33matf(window_to_view(i, j), data->rot_mat[1]);
			dir_t = mult_3vecf_33matf(window_to_view(i, j), data->rot_mat[0]);
			dir.val[0] = (dir.val[0] + dir_t.val[0]) / 2;
			dir.val[1] = (dir.val[1] + dir_t.val[1]) / 2;
			dir.val[2] = (dir.val[2] + dir_t.val[2]) / 2;
			*///dir = mult_3vecf_33matf(window_to_view(i, j), mult_33matf_33matf(data->rot_mat[1], data->rot_mat[0]));
			color = ray_trace(orig, dir, 0.01, FLT_MAX, data, 6);
		//	color = (i + WIN_WIDTH / 2 ) * 256 + j + WIN_HEIGHT / 2;
			ray_put_pixel(i, j, data->mlx->img_str, color);
			++j;
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
		ret = pthread_create(&(threads[i]), NULL, render_thread, (void *)(&(threads_param[i])));
		if (ret)
			exit(EXIT_FAILURE);
		++i;
	}
	i = 0;
	while (i < NB_THREADS)
		pthread_join(threads[i++], NULL);

		//printf("end");
}
