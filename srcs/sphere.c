/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   sphere.c                                         .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: aplat <aplat@student.le-101.fr>            +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2019/12/30 16:52:54 by pduhard-     #+#   ##    ##    #+#       */
/*   Updated: 2020/03/12 18:20:55 by pduhard-         ###   ########lyon.fr   */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "rt.h"

int		check_inside_sphere(t_3vecf point, t_obj *sphere)
{
	t_sphere	*param;

	param = (t_sphere *)sphere->obj_param;
	if (get_length_3vecf(sub_3vecf(point, param->origin)) > param->radius)
		return (0);
	return (1);
}

t_2vecf	get_text_coordinate_sphere(t_3vecf inter_point, t_3vecf normal_inter, t_obj *sphere)
{
	t_2vecf	text_coord;

	text_coord.val[1] = (1 - fmod((atan2(normal_inter.val[0], normal_inter.val[2]) / (2 * M_PI) + 0.5), 1));
	text_coord.val[0] = (normal_inter.val[1] * 0.5 + 0.5);
	//printf("%f %f\n", text_coord.val[0], text_coord.val[1]);
	return (text_coord);
	(void)inter_point;
	(void)sphere;
}

void	move_sphere(t_obj *sphere, t_3vecf dir, double fact)
{
	t_sphere	*param;
	t_cut		*cuts;

	param = (t_sphere *)sphere->obj_param;
	param->origin.val[0] += dir.val[0] * fact;
	param->origin.val[1] += dir.val[1] * fact;
	param->origin.val[2] += dir.val[2] * fact;
	cuts = sphere->cuts;
	while (cuts)
	{
		if (cuts->move && cuts->cut_type != CUT_STATIC)
			cuts->move(cuts, dir, fact);
		cuts = cuts->next;
	}
}

void	rotate_sphere(t_obj *sphere, t_3vecf orig, t_33matf rot_mat[2])
{
	t_sphere	*param;
	t_cut		*cuts;
	param = (t_sphere *)sphere->obj_param;
	param->origin = sub_3vecf(param->origin, orig);

	param->origin = mult_3vecf_33matf(param->origin, rot_mat[1]);
	param->origin = mult_3vecf_33matf(param->origin, rot_mat[0]);
	param->origin = add_3vecf(param->origin, orig);
	cuts = sphere->cuts;
	while (cuts)
	{
		if (cuts->rotate && cuts->cut_type != CUT_STATIC)
			cuts->rotate(cuts, orig, rot_mat);
		cuts = cuts->next;
	}
}

t_3vecf	get_origin_sphere(t_obj *sphere)
{
	return (((t_sphere *)sphere->obj_param)->origin);
}

t_3vecf	get_normal_intersect_sphere(t_3vecf inter_point, t_obj *sphere, int sp_id)
{
	t_sphere	*param;
	t_3vecf		sph_orig;
	t_3vecf		normal;

	param = (t_sphere *)sphere->obj_param;
	sph_orig = sp_id ? move_3vecf(param->origin, sphere->motions, sp_id) : param->origin;

	normal = sub_3vecf(inter_point, sph_orig);
	normalize_3vecf(&normal);
	return (normal);
}

int	ray_intersect_sphere(t_3vecf orig, t_3vecf dir, t_obj *sphere, double *dist, double min_dist, double max_dist, int sp_id)
{
	t_3vecf	dist_vec;
	double	a, b, c;
	double	delta;
	t_2vecf	hit_point;
	t_sphere	*sphere_param;
	int		check = 0;
	t_3vecf	sph_origin;

	sphere_param = (t_sphere *)sphere->obj_param;
	sph_origin = sp_id ? move_3vecf(sphere_param->origin, sphere->motions, sp_id) : sphere_param->origin;
	dist_vec = sub_3vecf(orig, sph_origin);
	a = dot_product_3vecf(dir, dir);
	b = 2.f * dot_product_3vecf(dist_vec, dir);
	c = dot_product_3vecf(dist_vec, dist_vec) - sphere_param->radius * sphere_param->radius;
	delta = b * b - 4.f * a * c;
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

void	generate_new_sphere(t_data *data)
{
	t_obj		*sphere;
	t_sphere	*param;
	t_3vecf		dir;

	dir = mult_3vecf_33matf(mult_3vecf_33matf(window_to_view(0, 0, data->size.val[0], data->size.val[1]), data->rot_mat[1]), data->rot_mat[0]);
	normalize_3vecf(&dir);
	if (!(sphere = ft_memalloc(sizeof(t_obj))))
		return ;
	if (!(param = ft_memalloc(sizeof(t_sphere))))
		return ;
	param->radius = get_random_number(time(NULL)) * 2.5;
	param->origin.val[0] = data->camera->origin.val[0] + dir.val[0] * param->radius * 2;
	param->origin.val[1] = data->camera->origin.val[1] + dir.val[1] * param->radius * 2;
	param->origin.val[2] = data->camera->origin.val[2] + dir.val[2] * param->radius * 2;
	sphere->obj_param = param;
	sphere->obj_type = OBJ_SPHERE;
	sphere->check_inside = &check_inside_sphere;
	sphere->ray_intersect = &ray_intersect_sphere;
	sphere->get_normal_inter = &get_normal_intersect_sphere;
	sphere->get_origin = &get_origin_sphere;
	sphere->move = &move_sphere;
	sphere->rotate = &rotate_sphere;
	sphere->get_text_coordinate = &get_text_coordinate_sphere;
	sphere->get_text_color = &get_uni_color;
	sphere->text = generate_random_texture();
	//sphere->get_bump_mapping = NULL;
	set_bump_own(sphere);
	//texture needed
	add_object(sphere, data);
	data->new_obj = 1;
}


/*int		parse_sphere(char *line, t_data *data)
  {
  int			i;
  t_obj		*sphere;
  t_sphere	*sphere_param;

  if (!(sphere = malloc(sizeof(t_obj))) || !(sphere_param = malloc(sizeof(t_sphere))))
  return (0);
  i = 6;
  while (ft_isspace(line[i]))
  ++i;
  if (line[i] != '(' || (i = parse_3vecf(line, i, &sphere_param->origin)) == -1)
  {
  ft_printf("Syntax error: sphere syntax: sphere(origin)(radius)(color)(reflection)(refraction)\n");
  return (0);
  }
  while (ft_isspace(line[i]))
  ++i;
  if (line[i] != '(' || (i = parse_double(line, i, &sphere_param->radius)) == -1)
  {
  ft_printf("Syntax error: sphere syntax: sphere(origin)(radius)(color)(reflection)(refraction)\n");
  return (0);
  }
  while (ft_isspace(line[i]))
  ++i;
  if (line[i] != '(' || (i = parse_texture(line, i, sphere)) == -1)
  {
  ft_printf("Syntax error: sphere syntax: sphere(origin)(radius)(color)(reflection)(refraction)\n");
  return (0);
  }
  while (ft_isspace(line[i]))
  ++i;
  if (line[i] != '(' || (i = parse_double(line, i, &sphere->reflection)) == -1)
  {
  ft_printf("Syntax error: sphere syntax: sphere(origin)(radius)(color)(reflection)(refraction)\n");
  return (0);
  }
  while (ft_isspace(line[i]))
  ++i;
  if (line[i] != '(' || (i = parse_double(line, i, &sphere->refraction)) == -1)
  {
  ft_printf("Syntax error: sphere syntax: sphere(origin)(radius)(color)(reflection)(refraction)\n");
  return (0);
  }

//printf("sphere : %f %f %f && %f && %f %f %f\n", sphere_param->origin.val[0], sphere_param->origin.val[1], sphere_param->origin.val[2], sphere_param->radius, sphere->color.val[0], sphere->color.val[1], sphere->color.val[2]);
sphere->obj_param = sphere_param;
sphere->obj_type = OBJ_SPHERE;
sphere->ray_intersect = &ray_intersect_sphere;
sphere->get_normal_inter = &get_normal_intersect_sphere;
sphere->get_text_coordinate = &get_text_coordinate_sphere;
if (data->objs)
{
sphere->next = data->objs;
}
else
sphere->next = NULL;
data->objs = sphere;
return (1);
}*/
