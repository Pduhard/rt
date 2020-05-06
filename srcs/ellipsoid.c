/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ellipsoid.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pduhard- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/25 18:28:43 by pduhard-          #+#    #+#             */
/*   Updated: 2020/02/29 01:47:58 by pduhard-         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

int		check_inside_ellipsoid(t_3vecf point, t_obj *ellipsoid)
{
	t_ellipsoid	*param;

	param = (t_ellipsoid *)ellipsoid->obj_param;
	point = sub_3vecf(point, param->origin);
	point.val[0] = (point.val[0] * point.val[0]) / (param->x_fact * param->x_fact);
	point.val[1] = (point.val[1] * point.val[1]) / (param->y_fact * param->y_fact);
	point.val[2] = (point.val[2] * point.val[2]) / (param->z_fact * param->z_fact);
	if (point.val[0] + point.val[1] + point.val[2] > 1)
		return (0);
	return (1);
}

t_2vecf	get_text_coordinate_ellipsoid(t_3vecf inter_point, t_3vecf normal_inter, t_obj *ellipsoid)
{
	t_2vecf	text_coord;

	text_coord.val[1] = (1 - fmod((atan2(normal_inter.val[0], normal_inter.val[2]) / (2 * M_PI) + 0.5), 1));
	text_coord.val[0] = (normal_inter.val[1] * 0.5 + 0.5);
	return (text_coord);
	(void)inter_point;
	(void)ellipsoid;
}

void	move_ellipsoid(t_obj *ellipsoid, t_3vecf dir, double fact)
{
	t_ellipsoid	*param;
	t_cut		*cuts;

	param = (t_ellipsoid *)ellipsoid->obj_param;
	param->origin.val[0] += dir.val[0] * fact;
	param->origin.val[1] += dir.val[1] * fact;
	param->origin.val[2] += dir.val[2] * fact;
	cuts = ellipsoid->cuts;
	while (cuts)
	{
		if (cuts->move && cuts->cut_type != CUT_STATIC)
			cuts->move(cuts, dir, fact);
		cuts = cuts->next;
	}
}

t_3vecf	get_origin_ellipsoid(t_obj *ellipsoid)
{
	return (((t_ellipsoid *)ellipsoid->obj_param)->origin);
}

t_3vecf	get_normal_intersect_ellipsoid(t_3vecf inter_point, t_obj *ellipsoid, int sp_id)
{
	t_ellipsoid	*param;
	t_3vecf		normal_inter;
	double		x;
	double		y;
	double		z;
	t_3vecf		ellipsoid_origin;

	param = (t_ellipsoid *)ellipsoid->obj_param;
	ellipsoid_origin = sp_id ? move_3vecf(param->origin, ellipsoid->motions, sp_id) : param->origin;
	x = inter_point.val[0] - ellipsoid_origin.val[0];
	y = inter_point.val[1] - ellipsoid_origin.val[1];
	z = inter_point.val[2] - ellipsoid_origin.val[2];
	normal_inter.val[0] = (2 * x) / (param->x_fact * param->x_fact);
	normal_inter.val[1] = (2 * y) / (param->y_fact * param->y_fact);
	normal_inter.val[2] = (2 * z) / (param->z_fact * param->z_fact);
	normalize_3vecf(&normal_inter);
	return (normal_inter);
}

int	ray_intersect_ellipsoid(t_leq l, t_obj *ellipsoid, t_dist dist, int sp_id)
{
	t_ellipsoid	*param;
	double		ox;
	double		oy;
	double		oz;
	double		dx;
	double		dy;
	double		dz;
	t_3vecf		t_fact;
	t_2vecf		roots;
	int			check;
	t_3vecf		ellipsoid_origin;

	(void)sp_id;
	check = 0;
	param = (t_ellipsoid *)ellipsoid->obj_param;
	ellipsoid_origin = sp_id ? move_3vecf(param->origin, ellipsoid->motions, sp_id) : param->origin;
	ox = l.orig.val[0] - ellipsoid_origin.val[0];
	oy = l.orig.val[1] - ellipsoid_origin.val[1];
	oz = l.orig.val[2] - ellipsoid_origin.val[2];
	dx = l.dir.val[0];
	dy = l.dir.val[1];
	dz = l.dir.val[2];
	double	a;
	double	b;
	double	c;

	a = param->x_fact;
	b = param->y_fact;
	c = param->z_fact;
	t_fact.val[2] = (ox * ox) / (a * a) + (oy * oy) / (b * b) + (oz * oz) / (c * c) - 1;//c
	t_fact.val[1] = (2 * ox * dx) / (a * a) + (2 * oy * dy) / (b * b) + (2 * oz * dz) / (c * c);//b
	t_fact.val[0] = (dx * dx) / (a * a) + (dy * dy) / (b * b) + (dz * dz) / (c * c);//a

	double delta = t_fact.val[1] * t_fact.val[1] - 4. * t_fact.val[0] * t_fact.val[2];

	if (delta < 0)
		return (0);
	roots.val[0] = (-t_fact.val[1] + sqrtf(delta)) / (2 * t_fact.val[0]);
	roots.val[1] = (-t_fact.val[1] - sqrtf(delta)) / (2 * t_fact.val[0]);
	if (roots.val[0] < *(dist.dist) && roots.val[0] > dist.min_dist && roots.val[0] < dist.max_dist)
	{
		check = 1;
		*(dist.dist) = roots.val[0];
	}
	if (roots.val[1] < *(dist.dist) && roots.val[1] > dist.min_dist && roots.val[1] < dist.max_dist)
	{
		check = 1;
		*(dist.dist) = roots.val[1];
	}
	return (check);
}

void	generate_new_ellipsoid(t_data *data)
{
	t_obj		*ellipsoid;
	t_ellipsoid	*param;
	t_3vecf		dir;

	dir = mult_3vecf_33matf(mult_3vecf_33matf(window_to_view(0, 0, data->size.val[0], data->size.val[1]), data->rot_mat[1]), data->rot_mat[0]);
	normalize_3vecf(&dir);
	if (!(ellipsoid = ft_memalloc(sizeof(t_obj))))
		return ;
	if (!(param = ft_memalloc(sizeof(t_ellipsoid))))
		return ;
	param->origin.val[0] = data->camera->origin.val[0] + dir.val[0] * 2;
	param->origin.val[1] = data->camera->origin.val[1] + dir.val[1] * 2;
	param->origin.val[2] = data->camera->origin.val[2] + dir.val[2] * 2;
	param->x_fact = get_random_number((time(NULL) * 0xcacacaca) << 16) * 2.5;
	param->y_fact = get_random_number((time(NULL) * 0xabcdef99) << 4) * 2.5;
	param->z_fact = get_random_number((time(NULL) * 0xff3672ff) << 3) * 2.5;

	//param->normal = assign_3vecf(get_random_number((time(NULL) * 0xcacacaca) << 16) - 0.5, get_random_number((time(NULL) * 0xfeabcdef) << 8) - 0.5, get_random_number((time(NULL) * 0x1056ffe) << 4) - 0.5);
	// normalize_3vecf(&param->normal);

/*	while (!is_null(dot_product_3vecf(param->normal, param->x2d_axis)))
	{
		param->x2d_axis = assign_3vecf(get_random_number(rd * 0xcacacaca << 16) - 0.5, get_random_number(rd * 0xfeabcdef << 8) - 0.5, get_random_number(rd * 0x1056ffe << 4) - 0.5);
		rd *= time(NULL);
		normalize_3vecf(&param->x2d_axis);
		printf("asad\n");
	} */
	ellipsoid->obj_param = param;
	ellipsoid->obj_type = OBJ_ELLIPSOID;
	ellipsoid->check_inside = &check_inside_ellipsoid;
	ellipsoid->ray_intersect = &ray_intersect_ellipsoid;
	ellipsoid->get_normal_inter = &get_normal_intersect_ellipsoid;
	ellipsoid->get_origin = &get_origin_ellipsoid;
	ellipsoid->move = &move_ellipsoid;
	ellipsoid->rotate = NULL;
	ellipsoid->get_text_coordinate = &get_text_coordinate_ellipsoid;
	ellipsoid->get_text_color = &get_uni_color;
	ellipsoid->text = generate_random_texture(ellipsoid);
	set_bump_own(ellipsoid);
	add_object(ellipsoid, data);
	data->new_obj = 1;
}

/*int		parse_ellipsoid(char *line, t_data *data)
  {
  int			i;
  t_obj		*ellipsoid;
  t_ellipsoid	*ellipsoid_param;

  if (!(ellipsoid = malloc(sizeof(t_obj))) || !(ellipsoid_param = malloc(sizeof(t_ellipsoid))))
  return (0);
  i = 6;
  while (ft_isspace(line[i]))
  ++i;
  if (line[i] != '(' || (i = parse_3vecf(line, i, &ellipsoid_param->origin)) == -1)
  {
  ft_printf("Syntax error: ellipsoid syntax: ellipsoid(origin)(radius)(color)(reflection)(refraction)\n");
  return (0);
  }
  while (ft_isspace(line[i]))
  ++i;
  if (line[i] != '(' || (i = parse_double(line, i, &ellipsoid_param->radius)) == -1)
  {
  ft_printf("Syntax error: ellipsoid syntax: ellipsoid(origin)(radius)(color)(reflection)(refraction)\n");
  return (0);
  }
  while (ft_isspace(line[i]))
  ++i;
  if (line[i] != '(' || (i = parse_texture(line, i, ellipsoid)) == -1)
  {
  ft_printf("Syntax error: ellipsoid syntax: ellipsoid(origin)(radius)(color)(reflection)(refraction)\n");
  return (0);
  }
  while (ft_isspace(line[i]))
  ++i;
  if (line[i] != '(' || (i = parse_double(line, i, &ellipsoid->reflection)) == -1)
  {
  ft_printf("Syntax error: ellipsoid syntax: ellipsoid(origin)(radius)(color)(reflection)(refraction)\n");
  return (0);
  }
  while (ft_isspace(line[i]))
  ++i;
  if (line[i] != '(' || (i = parse_double(line, i, &ellipsoid->refraction)) == -1)
  {
  ft_printf("Syntax error: ellipsoid syntax: ellipsoid(origin)(radius)(color)(reflection)(refraction)\n");
  return (0);
  }

//printf("ellipsoid : %f %f %f && %f && %f %f %f\n", ellipsoid_param->origin.val[0], ellipsoid_param->origin.val[1], ellipsoid_param->origin.val[2], ellipsoid_param->radius, ellipsoid->color.val[0], ellipsoid->color.val[1], ellipsoid->color.val[2]);
ellipsoid->obj_param = ellipsoid_param;
ellipsoid->obj_type = OBJ_ellipsoid;
ellipsoid->ray_intersect = &ray_intersect_ellipsoid;
ellipsoid->get_normal_inter = &get_normal_intersect_ellipsoid;
ellipsoid->get_text_coordinate = &get_text_coordinate_ellipsoid;
if (data->objs)
{
ellipsoid->next = data->objs;
}
else
ellipsoid->next = NULL;
data->objs = ellipsoid;
return (1);
}*/
