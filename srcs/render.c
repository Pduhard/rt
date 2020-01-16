/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   render.c                                         .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: pduhard- <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2019/12/21 22:42:45 by pduhard-     #+#   ##    ##    #+#       */
/*   Updated: 2020/01/16 04:31:49 by pduhard-    ###    #+. /#+    ###.fr     */
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

	vec.val[0] = (double)x / (double)WIN_WIDTH;
	vec.val[1] = (double)y / (double)WIN_HEIGHT;
	vec.val[2] = 1;
	return (vec);
}
/*
int	ray_intersect_sphere(t_3vecf orig, t_3vecf dir, t_obj *sphere, double *dist, double min_dist, double max_dist)
{
	t_3vecf	dist_vec;
	double	a, b, c;
	double	delta;
	t_2vecf	hit_point;
	t_sphere	*sphere_param;
	int		check = 0;

	sphere_param = (t_sphere *)sphere->obj_param;
	dist_vec = sub_3vecf(orig, sphere_param->origin);
	a = dot_product_3vecf(dir, dir);
	b = 2.f * dot_product_3vecf(dist_vec, dir);
	c = dot_product_3vecf(dist_vec, dist_vec) - sphere_param->radius * sphere_param->radius;
	delta = b * b - 4.f * a * c;

	//printf("a %f b %f c %f delta %f\n", a,b ,c ,delta);	
	if (delta < 0)
		return (0);
	hit_point.val[0] = (-b + sqrtf(delta)) / (2 * a);
	hit_point.val[1] = (-b - sqrtf(delta)) / (2 * a);
	if (hit_point.val[0] < *dist && hit_point.val[0] > min_dist && hit_point.val[0] < max_dist)
	{
		check = 1;
		*dist = hit_point.val[0];
	}
	if (hit_point.val[1] < *dist && hit_point.val[1] > min_dist && hit_point.val[1] < max_dist)
	{
		check = 1;
		*dist = hit_point.val[1];
	}
	return (check);
}

int	ray_intersect_plane(t_3vecf orig, t_3vecf dir, t_obj *plane, double *dist, double min_dist, double max_dist)
{
	t_plane	*plane_param;
	double	div;
	double	inter_dist;

	plane_param = (t_plane *)plane->obj_param;
	div = dot_product_3vecf(dir, plane_param->normal);
	if (div == 0)//> -0.00000001 && div < 0.00000001)
	{
		*dist = max_dist - 0.00001;
		return (1);
	}
	inter_dist = dot_product_3vecf(sub_3vecf(plane_param->origin, orig), plane_param->normal) / div;
	if (inter_dist < *dist && inter_dist > min_dist && inter_dist < max_dist)
	{
		*dist = inter_dist;
		return (1);
	}
	return (0);
}
*/
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

double	compute_lights(t_3vecf inter_point, t_3vecf normal_inter, t_3vecf inv_dir, t_light *lights, t_obj *objs)
{
	double	light_fact;
	double	norm_dot_ldir;
	double	ref_dot_idir;
	double	shadow_dist;
	double	light_len;
	t_3vecf	light_dir;
	t_3vecf	spec_vec;
	t_obj	*shadow_obj;

	light_fact = 0.f;
	while (lights)
	{
		if (lights->light_type == LIGHT_AMBIENT)
			light_fact += lights->intensity.val[0];
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
			shadow_obj = ray_first_intersect(inter_point, light_dir, 0.01, light_len, &shadow_dist, objs);
		//	shadow_obj = NULL;//ray_first_intersect(inter_point, light_dir, 0.001, light_len, &shadow_dist, objs);
			if (!shadow_obj)// || shadow_dist > get_length_3vecf(light_dir))
			{
				norm_dot_ldir = dot_product_3vecf(normal_inter, light_dir);
				if (norm_dot_ldir > 0)
					light_fact += lights->intensity.val[0] * norm_dot_ldir / (get_length_3vecf(normal_inter) * get_length_3vecf(light_dir));
				spec_vec = reflect_ray(light_dir, normal_inter);
			/*	spec_vec.val[0] = 2 * normal_inter.val[0] * norm_dot_ldir - light_dir.val[0];
				spec_vec.val[1] = 2 * normal_inter.val[1] * norm_dot_ldir - light_dir.val[1];
				spec_vec.val[2] = 2 * normal_inter.val[2] * norm_dot_ldir - light_dir.val[2];
			*/	ref_dot_idir = dot_product_3vecf(spec_vec, inv_dir);
				if (ref_dot_idir > 0)
					light_fact += lights->intensity.val[0] * powf(ref_dot_idir / (get_length_3vecf(spec_vec) * get_length_3vecf(inv_dir)), 100);
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
	double		light_fact;
//	double		normal_length;

	inter_point.val[0] = orig.val[0] + dir.val[0] * closest_dist;
	inter_point.val[1] = orig.val[1] + dir.val[1] * closest_dist;
	inter_point.val[2] = orig.val[2] + dir.val[2] * closest_dist;
	normal_inter = closest_obj->get_normal_inter(inter_point, closest_obj);
	normalize_3vecf(&normal_inter);
	if (dot_product_3vecf(normal_inter, dir) > 0)
		normal_inter = assign_3vecf(-normal_inter.val[0], -normal_inter.val[1], -normal_inter.val[2]);
/*	normal_length = get_length_3vecf(normal_inter);
	normal_inter.val[0] /= normal_length;
	normal_inter.val[1] /= normal_length;
	normal_inter.val[2] /= normal_length;
*/	light_fact = compute_lights(inter_point, normal_inter, inv_dir, data->lights, data->objs);
	lighted_color.val[0] = closest_obj->color.val[0] * light_fact;
	lighted_color.val[1] = closest_obj->color.val[1] * light_fact;
	lighted_color.val[2] = closest_obj->color.val[2] * light_fact;

	if (!depth)
		return (lighted_color);
	t_3vecf	refl_ray = reflect_ray(inv_dir, normal_inter);
	t_3vecf	refl_color = ray_trace(inter_point, refl_ray, 0.01, FLT_MAX, data, depth - 1);

	lighted_color.val[0] = lighted_color.val[0] * (1 - closest_obj->reflection) + refl_color.val[0] * closest_obj->reflection;
	lighted_color.val[1] = lighted_color.val[1] * (1 - closest_obj->reflection) + refl_color.val[1] * closest_obj->reflection;
	lighted_color.val[2] = lighted_color.val[2] * (1 - closest_obj->reflection) + refl_color.val[2] * closest_obj->reflection;
	//lighted_color.val[1] = closest_obj->color.val[1] * light_fact;
	//lighted_color.val[2] = closest_obj->color.val[2] * light_fact;
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
			color = ray_trace(orig, dir, 0.01, FLT_MAX, data, 3);
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
