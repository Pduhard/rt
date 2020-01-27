/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   sphere.c                                         .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: aplat <aplat@student.le-101.fr>            +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2019/12/30 16:52:54 by pduhard-     #+#   ##    ##    #+#       */
/*   Updated: 2020/01/27 18:14:52 by aplat       ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "rt.h"

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

t_3vecf	get_normal_intersect_sphere(t_3vecf inter_point, t_obj *sphere)
{
	t_sphere *param;

	param = (t_sphere *)sphere->obj_param;
	return (sub_3vecf(inter_point, param->origin));
}

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
