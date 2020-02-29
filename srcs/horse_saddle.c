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
	double		y;
	double		z;
	t_3vecf		horse_saddle_origin;

	param = (t_horse_saddle *)horse_saddle->obj_param;
	horse_saddle_origin = sp_id ? move_3vecf(param->origin, horse_saddle->motions, sp_id) : param->origin;
	x = inter_point.val[0] - horse_saddle_origin.val[0];
	y = inter_point.val[1] - horse_saddle_origin.val[1];
	z = inter_point.val[2] - horse_saddle_origin.val[2];
	normal_inter.val[0] = (2 * x) / (param->x_fact * param->x_fact);
	normal_inter.val[1] = -1;// / (param->y_fact * param->y_fact);
	normal_inter.val[2] = (-2 * z) / (param->y_fact * param->y_fact);
	normalize_3vecf(&normal_inter);
	return (normal_inter);	
}

int	ray_intersect_horse_saddle(t_3vecf orig, t_3vecf dir, t_obj *horse_saddle, double *dist, double min_dist, double max_dist, int sp_id)
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
	ox = orig.val[0] - horse_saddle_origin.val[0];
	oy = orig.val[1] - horse_saddle_origin.val[1];
	oz = orig.val[2] - horse_saddle_origin.val[2];
//	if (ox < -1 || ox > 1 || oy < -1 || oy > 1)
//		return (0);
	dx = dir.val[0];
	dy = dir.val[1];
	dz = dir.val[2];
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
	if (roots.val[0] < *dist && roots.val[0] > min_dist && roots.val[0] < max_dist)
	{
		double x, z;
		x = ox + dx * roots.val[0];
		z = oz + dz * roots.val[0];
		if (x > -param->x_fact && x < param->x_fact && z > -param->y_fact && z < param->y_fact)
		//if (x > -1 && x < 1 && z > -1 && z < 1)
		{
			check = 1;
			*dist = roots.val[0];
		}
	}
	if (roots.val[1] < *dist && roots.val[1] > min_dist && roots.val[1] < max_dist)
	{
		double x, z;
		x = ox + dx * roots.val[1];
		z = oz + dz * roots.val[1];
		if (x > -param->x_fact && x < param->x_fact && z > -param->y_fact && z < param->y_fact)
		{
			check = 1;
			*dist = roots.val[1];
		}
	}
	return (check);
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
