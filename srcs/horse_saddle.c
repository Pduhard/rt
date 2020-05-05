/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   horse_saddle.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pduhard- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/25 18:28:43 by pduhard-          #+#    #+#             */
/*   Updated: 2020/02/28 23:50:59 by pduhard-         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

int		check_inside_horse_saddle(t_3vecf point, t_obj *horse_saddle)
{
	t_horse_saddle	*param;

	param = (t_horse_saddle *)horse_saddle->obj_param;
	point = sub_3vecf(point, param->origin);
	if (point.val[0] < -param->x_fact || point.val[0] > param->x_fact || point.val[1] < -param->y_fact || point.val[1] > param->y_fact)
		return (0);
	point.val[0] = (point.val[0] * point.val[0]) / (param->x_fact * param->x_fact);
	point.val[1] = (point.val[1] * point.val[1]) / (param->y_fact * param->y_fact);
	if (!is_null(point.val[0] + point.val[1] + point.val[2]))
		return (0);
	return (1);
}

t_2vecf	get_text_coordinate_horse_saddle(t_3vecf inter_point, t_3vecf normal_inter, t_obj *horse_saddle)
{
	t_2vecf	text_coord;

	text_coord.val[1] = (1 - fmod((atan2(normal_inter.val[0], normal_inter.val[2]) / (2 * M_PI) + 0.5), 1));
	text_coord.val[0] = (normal_inter.val[1] * 0.5 + 0.5);
	return (text_coord);
	(void)inter_point;
	(void)horse_saddle;
}

void	move_horse_saddle(t_obj *horse_saddle, t_3vecf dir, double fact)
{
	t_horse_saddle	*param;
	t_cut			*cuts;

	param = (t_horse_saddle *)horse_saddle->obj_param;
	param->origin.val[0] += dir.val[0] * fact;
	param->origin.val[1] += dir.val[1] * fact;
	param->origin.val[2] += dir.val[2] * fact;
	cuts = horse_saddle->cuts;
	while (cuts)
	{
		if (cuts->move && cuts->cut_type != CUT_STATIC)
			cuts->move(cuts, dir, fact);
		cuts = cuts->next;
	}
}

t_3vecf	get_origin_horse_saddle(t_obj *horse_saddle)
{
	return (((t_horse_saddle *)horse_saddle->obj_param)->origin);
}

t_3vecf	get_normal_intersect_horse_saddle(t_3vecf inter_point, t_obj *horse_saddle, int sp_id)
{
	t_horse_saddle	*param;
	t_3vecf		normal_inter;
	double		x;
//	double		y;
	double		z;
	t_3vecf		horse_saddle_origin;

	param = (t_horse_saddle *)horse_saddle->obj_param;
	horse_saddle_origin = sp_id ? move_3vecf(param->origin, horse_saddle->motions, sp_id) : param->origin;
	x = inter_point.val[0] - horse_saddle_origin.val[0];
//	y = inter_point.val[1] - horse_saddle_origin.val[1];
	z = inter_point.val[2] - horse_saddle_origin.val[2];
	normal_inter.val[0] = (2 * x) / (param->x_fact * param->x_fact);
	normal_inter.val[1] = -1;// / (param->y_fact * param->y_fact);
	normal_inter.val[2] = (-2 * z) / (param->y_fact * param->y_fact);
	normalize_3vecf(&normal_inter);
	return (normal_inter);
}

int	ray_intersect_horse_saddle(t_leq l, t_obj *horse_saddle, t_dist dist, int sp_id)
{
	t_horse_saddle	*param;
	double		ox;
	double		oy;
	double		oz;
	double		dx;
	double		dy;
	double		dz;
	t_3vecf		t_fact;
	t_2vecf		roots;
	int			check;
	t_3vecf		horse_saddle_origin;

	(void)sp_id;
	check = 0;
	param = (t_horse_saddle *)horse_saddle->obj_param;
	horse_saddle_origin = sp_id ? move_3vecf(param->origin, horse_saddle->motions, sp_id) : param->origin;
	ox = l.orig.val[0] - horse_saddle_origin.val[0];
	oy = l.orig.val[1] - horse_saddle_origin.val[1];
	oz = l.orig.val[2] - horse_saddle_origin.val[2];
//	if (ox < -1 || ox > 1 || oy < -1 || oy > 1)
//		return (0);
	dx = l.dir.val[0];
	dy = l.dir.val[1];
	dz = l.dir.val[2];
	double	a;
	double	b;

	a = param->x_fact;
	b = param->y_fact;
	t_fact.val[2] = (ox * ox) / (a * a) - (oz * oz) / (b * b) - oy;//c
	t_fact.val[1] = (2 * ox * dx) / (a * a) - (2 * oz * dz) / (b * b) - dy;//b
	t_fact.val[0] = (dx * dx) / (a * a) - (dz * dz) / (b * b);//a

	if (is_null(t_fact.val[0]))
	{
		roots.val[0] = -t_fact.val[2] / t_fact.val[1];
		roots.val[1] = roots.val[0];
	}
	else
	{
	//		t_fact.val[0] = 0.01;
		double delta = t_fact.val[1] * t_fact.val[1] - 4. * t_fact.val[0] * t_fact.val[2];

		if (delta < 0)
			return (0);
		roots.val[0] = (-t_fact.val[1] + sqrtf(delta)) / (2 * t_fact.val[0]);
		roots.val[1] = (-t_fact.val[1] - sqrtf(delta)) / (2 * t_fact.val[0]);
	}
	if (roots.val[0] < *(dist.dist) && roots.val[0] > dist.min_dist && roots.val[0] < dist.max_dist)
	{
		double x, z;
		x = ox + dx * roots.val[0];
		z = oz + dz * roots.val[0];
		if (x > -param->x_fact && x < param->x_fact && z > -param->y_fact && z < param->y_fact)
		//if (x > -1 && x < 1 && z > -1 && z < 1)
		{
			check = 1;
			*(dist.dist) = roots.val[0];
		}
	}
	if (roots.val[1] < *(dist.dist) && roots.val[1] > dist.min_dist && roots.val[1] < dist.max_dist)
	{
		double x, z;
		x = ox + dx * roots.val[1];
		z = oz + dz * roots.val[1];
		if (x > -param->x_fact && x < param->x_fact && z > -param->y_fact && z < param->y_fact)
		{
			check = 1;
			*(dist.dist) = roots.val[1];
		}
	}
	return (check);
}

void	generate_new_horse_saddle(t_data *data)
{
	t_obj		*horse_saddle;
	t_horse_saddle	*param;
	t_3vecf		dir;

	dir = mult_3vecf_33matf(mult_3vecf_33matf(window_to_view(0, 0, data->size.val[0], data->size.val[1]), data->rot_mat[1]), data->rot_mat[0]);
	normalize_3vecf(&dir);
	if (!(horse_saddle = ft_memalloc(sizeof(t_obj))))
		return ;
	if (!(param = ft_memalloc(sizeof(t_horse_saddle))))
		return ;
	param->origin.val[0] = data->camera->origin.val[0] + dir.val[0] * 2;
	param->origin.val[1] = data->camera->origin.val[1] + dir.val[1] * 2;
	param->origin.val[2] = data->camera->origin.val[2] + dir.val[2] * 2;
	param->x_fact = get_random_number((time(NULL) * 0xcacacaca) << 16) * 2.5;
	param->y_fact = get_random_number((time(NULL) * 0xabcdef99) << 4) * 2.5;

	//param->normal = assign_3vecf(get_random_number((time(NULL) * 0xcacacaca) << 16) - 0.5, get_random_number((time(NULL) * 0xfeabcdef) << 8) - 0.5, get_random_number((time(NULL) * 0x1056ffe) << 4) - 0.5);
	// normalize_3vecf(&param->normal);
/*	while (!is_null(dot_product_3vecf(param->normal, param->x2d_axis)))
	{
		param->x2d_axis = assign_3vecf(get_random_number(rd * 0xcacacaca << 16) - 0.5, get_random_number(rd * 0xfeabcdef << 8) - 0.5, get_random_number(rd * 0x1056ffe << 4) - 0.5);
		rd *= time(NULL);
		normalize_3vecf(&param->x2d_axis);
		printf("asad\n");
	} */
	horse_saddle->obj_param = param;
	horse_saddle->obj_type = OBJ_HORSE_SADDLE;
	horse_saddle->check_inside = &check_inside_horse_saddle;
	horse_saddle->ray_intersect = &ray_intersect_horse_saddle;
	horse_saddle->get_normal_inter = &get_normal_intersect_horse_saddle;
	horse_saddle->get_origin = &get_origin_horse_saddle;
	horse_saddle->move = &move_horse_saddle;
	horse_saddle->rotate = NULL;
	horse_saddle->get_text_coordinate = &get_text_coordinate_horse_saddle;
	horse_saddle->get_text_color = &get_uni_color;
	horse_saddle->text = generate_random_texture();
	set_bump_own(horse_saddle);
	add_object(horse_saddle, data);
	data->new_obj = 1;
}
/*int		parse_horse_saddle(char *line, t_data *data)
  {
  int			i;
  t_obj		*horse_saddle;
  t_horse_saddle	*horse_saddle_param;

  if (!(horse_saddle = malloc(sizeof(t_obj))) || !(horse_saddle_param = malloc(sizeof(t_horse_saddle))))
  return (0);
  i = 6;
  while (ft_isspace(line[i]))
  ++i;
  if (line[i] != '(' || (i = parse_3vecf(line, i, &horse_saddle_param->origin)) == -1)
  {
  ft_printf("Syntax error: horse_saddle syntax: horse_saddle(origin)(radius)(color)(reflection)(refraction)\n");
  return (0);
  }
  while (ft_isspace(line[i]))
  ++i;
  if (line[i] != '(' || (i = parse_double(line, i, &horse_saddle_param->radius)) == -1)
  {
  ft_printf("Syntax error: horse_saddle syntax: horse_saddle(origin)(radius)(color)(reflection)(refraction)\n");
  return (0);
  }
  while (ft_isspace(line[i]))
  ++i;
  if (line[i] != '(' || (i = parse_texture(line, i, horse_saddle)) == -1)
  {
  ft_printf("Syntax error: horse_saddle syntax: horse_saddle(origin)(radius)(color)(reflection)(refraction)\n");
  return (0);
  }
  while (ft_isspace(line[i]))
  ++i;
  if (line[i] != '(' || (i = parse_double(line, i, &horse_saddle->reflection)) == -1)
  {
  ft_printf("Syntax error: horse_saddle syntax: horse_saddle(origin)(radius)(color)(reflection)(refraction)\n");
  return (0);
  }
  while (ft_isspace(line[i]))
  ++i;
  if (line[i] != '(' || (i = parse_double(line, i, &horse_saddle->refraction)) == -1)
  {
  ft_printf("Syntax error: horse_saddle syntax: horse_saddle(origin)(radius)(color)(reflection)(refraction)\n");
  return (0);
  }

//printf("horse_saddle : %f %f %f && %f && %f %f %f\n", horse_saddle_param->origin.val[0], horse_saddle_param->origin.val[1], horse_saddle_param->origin.val[2], horse_saddle_param->radius, horse_saddle->color.val[0], horse_saddle->color.val[1], horse_saddle->color.val[2]);
horse_saddle->obj_param = horse_saddle_param;
horse_saddle->obj_type = OBJ_horse_saddle;
horse_saddle->ray_intersect = &ray_intersect_horse_saddle;
horse_saddle->get_normal_inter = &get_normal_intersect_horse_saddle;
horse_saddle->get_text_coordinate = &get_text_coordinate_horse_saddle;
if (data->objs)
{
horse_saddle->next = data->objs;
}
else
horse_saddle->next = NULL;
data->objs = horse_saddle;
return (1);
}*/
