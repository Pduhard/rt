/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hyperboloid.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pduhard- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/25 18:28:43 by pduhard-          #+#    #+#             */
/*   Updated: 2020/02/28 23:49:41 by pduhard-         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

int		check_inside_hyperboloid(t_3vecf point, t_obj *hyperboloid)
{
	t_hyperboloid	*param;

	param = (t_hyperboloid *)hyperboloid->obj_param;
	point = sub_3vecf(point, param->origin);
	point.val[0] = (point.val[0] * point.val[0]) / (param->x_fact * param->x_fact);
	point.val[1] = (point.val[1] * point.val[1]) / (param->y_fact * param->y_fact);
	point.val[2] = (point.val[2] * point.val[2]) / (param->z_fact * param->z_fact);
	if (point.val[0] - point.val[1] + point.val[2] + param->surface > 0)
		return (0);
	return (1);
}

t_2vecf	get_text_coordinate_hyperboloid(t_3vecf inter_point, t_3vecf normal_inter, t_obj *hyperboloid)
{
	t_2vecf	text_coord;

	text_coord.val[1] = (1 - fmod((atan2(normal_inter.val[0], normal_inter.val[2]) / (2 * M_PI) + 0.5), 1));
	text_coord.val[0] = (normal_inter.val[1] * 0.5 + 0.5);
	return (text_coord);
	(void)inter_point;
	(void)hyperboloid;
}

void	move_hyperboloid(t_obj *hyperboloid, t_3vecf dir, double fact)
{
	t_hyperboloid	*param;
	t_cut			*cuts;

	param = (t_hyperboloid *)hyperboloid->obj_param;
	param->origin.val[0] += dir.val[0] * fact;
	param->origin.val[1] += dir.val[1] * fact;
	param->origin.val[2] += dir.val[2] * fact;
	cuts = hyperboloid->cuts;
	while (cuts)
	{
		if (cuts->move && cuts->cut_type != CUT_STATIC)
			cuts->move(cuts, dir, fact);
		cuts = cuts->next;
	}
}

t_3vecf	get_origin_hyperboloid(t_obj *hyperboloid)
{
	return (((t_hyperboloid *)hyperboloid->obj_param)->origin);
}

t_3vecf	get_normal_intersect_hyperboloid(t_3vecf inter_point, t_obj *hyperboloid, int sp_id)
{
	t_hyperboloid	*param;
	t_3vecf		normal_inter;
	double		x;
	double		y;
	double		z;
	t_3vecf		hyperboloid_origin;

	param = (t_hyperboloid *)hyperboloid->obj_param;
	hyperboloid_origin = sp_id ? move_3vecf(param->origin, hyperboloid->motions, sp_id) : param->origin;
	x = inter_point.val[0] - hyperboloid_origin.val[0];
	y = inter_point.val[1] - hyperboloid_origin.val[1];
	z = inter_point.val[2] - hyperboloid_origin.val[2];
	normal_inter.val[0] = (2 * x) / (param->x_fact * param->x_fact);
	normal_inter.val[1] = (-2 * y) / (param->y_fact * param->y_fact);
	normal_inter.val[2] = (2 * z) / (param->z_fact * param->z_fact);
	normalize_3vecf(&normal_inter);
	return (normal_inter);
}

int	ray_intersect_hyperboloid(t_3vecf orig, t_3vecf dir, t_obj *hyperboloid, double *dist, double min_dist, double max_dist, int sp_id)
{
	t_hyperboloid	*param;
	double		ox;
	double		oy;
	double		oz;
	double		dx;
	double		dy;
	double		dz;
	t_3vecf		t_fact;
	t_2vecf		roots;
	int			check;
	t_3vecf		hyperboloid_origin;

	(void)sp_id;
	check = 0;
	param = (t_hyperboloid *)hyperboloid->obj_param;
	hyperboloid_origin = sp_id ? move_3vecf(param->origin, hyperboloid->motions, sp_id) : param->origin;
	ox = orig.val[0] - hyperboloid_origin.val[0];
	oy = orig.val[1] - hyperboloid_origin.val[1];
	oz = orig.val[2] - hyperboloid_origin.val[2];
	dx = dir.val[0];
	dy = dir.val[1];
	dz = dir.val[2];
	double	a;
	double	b;
	double	c;

	a = param->x_fact;
	b = param->y_fact;
	c = param->z_fact;
	t_fact.val[2] = (ox * ox) / (a * a) - (oy * oy) / (b * b) + (oz * oz) / (c * c) + param->surface;//c
	t_fact.val[1] = (2 * ox * dx) / (a * a) - (2 * oy * dy) / (b * b) + (2 * oz * dz) / (c * c);//b
	t_fact.val[0] = (dx * dx) / (a * a) - (dy * dy) / (b * b) + (dz * dz) / (c * c);//a

	double delta = t_fact.val[1] * t_fact.val[1] - 4. * t_fact.val[0] * t_fact.val[2];

	if (delta < 0)
		return (0);
	roots.val[0] = (-t_fact.val[1] + sqrtf(delta)) / (2 * t_fact.val[0]);
	roots.val[1] = (-t_fact.val[1] - sqrtf(delta)) / (2 * t_fact.val[0]);
	if (roots.val[0] < *dist && roots.val[0] > min_dist && roots.val[0] < max_dist)
	{
		check = 1;
		*dist = roots.val[0];
	}
	if (roots.val[1] < *dist && roots.val[1] > min_dist && roots.val[1] < max_dist)
	{
		check = 1;
		*dist = roots.val[1];
	}
	return (check);
}

void	generate_new_hyperboloid(t_data *data)
{
	t_obj		*hyperboloid;
	t_hyperboloid	*param;
	t_3vecf		dir;

	dir = mult_3vecf_33matf(mult_3vecf_33matf(window_to_view(0, 0, data->size.val[0], data->size.val[1]), data->rot_mat[1]), data->rot_mat[0]);
	normalize_3vecf(&dir);
	if (!(hyperboloid = ft_memalloc(sizeof(t_obj))))
		return ;
	if (!(param = ft_memalloc(sizeof(t_hyperboloid))))
		return ;
	param->origin.val[0] = data->camera->origin.val[0] + dir.val[0] * 2;
	param->origin.val[1] = data->camera->origin.val[1] + dir.val[1] * 2;
	param->origin.val[2] = data->camera->origin.val[2] + dir.val[2] * 2;
	param->x_fact = get_random_number((time(NULL) * 0xcacacaca) << 16) * 2.5;
	param->y_fact = get_random_number((time(NULL) * 0xabcdef99) << 4) * 2.5;
	param->z_fact = get_random_number((time(NULL) * 0xff3672ff) << 3) * 2.5;
	param->surface = get_random_number(time(NULL)) > 0.5 ? -1 : 1;

	//param->normal = assign_3vecf(get_random_number((time(NULL) * 0xcacacaca) << 16) - 0.5, get_random_number((time(NULL) * 0xfeabcdef) << 8) - 0.5, get_random_number((time(NULL) * 0x1056ffe) << 4) - 0.5);
	// normalize_3vecf(&param->normal);
/*	while (!is_null(dot_product_3vecf(param->normal, param->x2d_axis)))
	{
		param->x2d_axis = assign_3vecf(get_random_number(rd * 0xcacacaca << 16) - 0.5, get_random_number(rd * 0xfeabcdef << 8) - 0.5, get_random_number(rd * 0x1056ffe << 4) - 0.5);
		rd *= time(NULL);
		normalize_3vecf(&param->x2d_axis);
		printf("asad\n");
	} */
	hyperboloid->obj_param = param;
	hyperboloid->obj_type = OBJ_HYPERBOLOID;
	hyperboloid->check_inside = &check_inside_hyperboloid;
	hyperboloid->ray_intersect = &ray_intersect_hyperboloid;
	hyperboloid->get_normal_inter = &get_normal_intersect_hyperboloid;
	hyperboloid->get_origin = &get_origin_hyperboloid;
	hyperboloid->move = &move_hyperboloid;
	hyperboloid->rotate = NULL;
	hyperboloid->get_text_coordinate = &get_text_coordinate_hyperboloid;
	hyperboloid->get_text_color = &get_uni_color;
	hyperboloid->text = generate_random_texture();
	set_bump_own(hyperboloid);
	add_object(hyperboloid, data);
	data->new_obj = 1;
}

/*int		parse_hyperboloid(char *line, t_data *data)
  {
  int			i;
  t_obj		*hyperboloid;
  t_hyperboloid	*hyperboloid_param;

  if (!(hyperboloid = malloc(sizeof(t_obj))) || !(hyperboloid_param = malloc(sizeof(t_hyperboloid))))
  return (0);
  i = 6;
  while (ft_isspace(line[i]))
  ++i;
  if (line[i] != '(' || (i = parse_3vecf(line, i, &hyperboloid_param->origin)) == -1)
  {
  ft_printf("Syntax error: hyperboloid syntax: hyperboloid(origin)(radius)(color)(reflection)(refraction)\n");
  return (0);
  }
  while (ft_isspace(line[i]))
  ++i;
  if (line[i] != '(' || (i = parse_double(line, i, &hyperboloid_param->radius)) == -1)
  {
  ft_printf("Syntax error: hyperboloid syntax: hyperboloid(origin)(radius)(color)(reflection)(refraction)\n");
  return (0);
  }
  while (ft_isspace(line[i]))
  ++i;
  if (line[i] != '(' || (i = parse_texture(line, i, hyperboloid)) == -1)
  {
  ft_printf("Syntax error: hyperboloid syntax: hyperboloid(origin)(radius)(color)(reflection)(refraction)\n");
  return (0);
  }
  while (ft_isspace(line[i]))
  ++i;
  if (line[i] != '(' || (i = parse_double(line, i, &hyperboloid->reflection)) == -1)
  {
  ft_printf("Syntax error: hyperboloid syntax: hyperboloid(origin)(radius)(color)(reflection)(refraction)\n");
  return (0);
  }
  while (ft_isspace(line[i]))
  ++i;
  if (line[i] != '(' || (i = parse_double(line, i, &hyperboloid->refraction)) == -1)
  {
  ft_printf("Syntax error: hyperboloid syntax: hyperboloid(origin)(radius)(color)(reflection)(refraction)\n");
  return (0);
  }

//printf("hyperboloid : %f %f %f && %f && %f %f %f\n", hyperboloid_param->origin.val[0], hyperboloid_param->origin.val[1], hyperboloid_param->origin.val[2], hyperboloid_param->radius, hyperboloid->color.val[0], hyperboloid->color.val[1], hyperboloid->color.val[2]);
hyperboloid->obj_param = hyperboloid_param;
hyperboloid->obj_type = OBJ_hyperboloid;
hyperboloid->ray_intersect = &ray_intersect_hyperboloid;
hyperboloid->get_normal_inter = &get_normal_intersect_hyperboloid;
hyperboloid->get_text_coordinate = &get_text_coordinate_hyperboloid;
if (data->objs)
{
hyperboloid->next = data->objs;
}
else
hyperboloid->next = NULL;
data->objs = hyperboloid;
return (1);
}*/
