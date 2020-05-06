/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monkey_saddle.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pduhard- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/26 02:40:58 by pduhard-          #+#    #+#             */
/*   Updated: 2020/02/28 23:51:52 by pduhard-         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

int		check_inside_monkey_saddle(t_3vecf point, t_obj *monkey_saddle)
{
	(void)monkey_saddle;
//	t_monkey_saddle	*param;
	double	v;
	double	u;
	double	sin_v_2;

	return (0);
//	param = (t_monkey_saddle *)monkey_saddle->obj_param;
	v = atan2(point.val[1], point.val[0]);
	if (!is_null((sin_v_2 = sin(v / 2))))
		u = point.val[2] / sin_v_2;
	u = (point.val[0] - 1);
	point.val[0] -= (1 + u * cos(v / 2)) * cos(v);
	point.val[1] -= (1 + u * cos(v / 2)) * sin(v);
	point.val[2] -= u * sin_v_2;

	if (is_null(point.val[0] * point.val[0] + point.val[1] * point.val[1] + point.val[2] * point.val[2]) && u < 1 && u > 1)
		return (1);
	return (0);
}

t_2vecf	get_text_coordinate_monkey_saddle(t_3vecf inter_point, t_3vecf normal_inter, t_obj *monkey_saddle)
{
	(void)monkey_saddle;
	t_2vecf	text_coord;
	double	sin_v_2;
//	t_monkey_saddle	*param;

//	param = (t_monkey_saddle *)monkey_saddle->obj_param;
	text_coord.val[1] = atan2(inter_point.val[1], inter_point.val[0]);
	if (!is_null((sin_v_2 = sin(text_coord.val[1] / 2))))
		text_coord.val[0] = inter_point.val[2] / sin_v_2;
	else if (!is_null(text_coord.val[1]))
		text_coord.val[0] = (inter_point.val[0] / cos(text_coord.val[1]) - 1) / cos(text_coord.val[1] / 2);
	else
		text_coord.val[0] = (inter_point.val[0]);

/* FALSE need to get from parametric equation*/

	//printf("%f %f\n", text_coord.val[0], text_coord.val[1]);
	text_coord.val[0] = (text_coord.val[0]) / (2);
	text_coord.val[1] /= M_PI * 2;
	return (text_coord);
	(void)normal_inter;
}

void	move_monkey_saddle(t_obj *monkey_saddle, t_3vecf dir, double fact)
{
	t_monkey_saddle	*param;
	t_cut			*cuts;

	param = (t_monkey_saddle *)monkey_saddle->obj_param;
	param->origin.val[0] += dir.val[0] * fact;
	param->origin.val[1] += dir.val[1] * fact;
	param->origin.val[2] += dir.val[2] * fact;
	cuts = monkey_saddle->cuts;
	while (cuts)
	{
		if (cuts->move && cuts->cut_type != CUT_STATIC)
			cuts->move(cuts, dir, fact);
		cuts = cuts->next;
	}
}

t_3vecf	get_origin_monkey_saddle(t_obj *monkey_saddle)
{
	return (((t_monkey_saddle *)monkey_saddle->obj_param)->origin);
}

t_3vecf	get_normal_intersect_monkey_saddle(t_3vecf inter_point, t_obj *monkey_saddle, int sp_id)
{
	t_monkey_saddle	*param;
	t_3vecf		normal_inter;
	double		x;
//	double		y;
	double		z;
	t_3vecf		monkey_saddle_origin;

	param = (t_monkey_saddle *)monkey_saddle->obj_param;
	monkey_saddle_origin = sp_id ? move_3vecf(param->origin, monkey_saddle->motions, sp_id) : param->origin;
	x = inter_point.val[0] - monkey_saddle_origin.val[0];
//	y = inter_point.val[1] - monkey_saddle_origin.val[1];
	z = inter_point.val[2] - monkey_saddle_origin.val[2];
	normal_inter.val[0] = 3 * x * x - 3 * z * z;
	normal_inter.val[1] = -1;
	normal_inter.val[2] = -6 * x * z;
	normalize_3vecf(&normal_inter);
	return (normal_inter);
}

int	ray_intersect_monkey_saddle(t_leq l, t_obj *monkey_saddle, t_dist dist, int sp_id)
{
	t_monkey_saddle	*param;
	double		ox;
	double		oy;
	double		oz;
	double		dx;
	double		dy;
	double		dz;
	t_4vecf		t_fact;
	t_3vecf		roots;
	int			check;
	t_3vecf		monkey_saddle_origin;

	(void)sp_id;
	check = 0;
	param = (t_monkey_saddle *)monkey_saddle->obj_param;
	monkey_saddle_origin = sp_id ? move_3vecf(param->origin, monkey_saddle->motions, sp_id) : param->origin;
	ox = l.orig.val[0] - monkey_saddle_origin.val[0];
	oy = l.orig.val[1] - monkey_saddle_origin.val[1];
	oz = l.orig.val[2] - monkey_saddle_origin.val[2];
	dx = l.dir.val[0];
	dy = l.dir.val[1];
	dz = l.dir.val[2];

	t_fact.val[0] = (-3 * dx * dz * dz) + (dx * dx * dx);//a
	t_fact.val[1] = (-3 * ox * dz * dz) - (6 * oz * dz * dx) + (dx * dx * ox) + (2 * ox * dx * dx);//b
	t_fact.val[2] = (-6 * ox * oz * dz) - (3 * oz * oz * dx) - dy + (2 * ox * ox * dx) + (ox * ox * dx);//c
	t_fact.val[3] = (-3 * oz * oz * ox) + (ox * ox * ox) - oy;//d

	roots = solve_cubic(t_fact.val[0], t_fact.val[1], t_fact.val[2], t_fact.val[3]);
	int		i = -1;
//	printf("%f %f %f\n", roots.val[0], roots.val[1], roots.val[2]);
	while (++i < 3)
	{
		//t_3vecf	coord;
	//	coord.val[0] = orig.val[0] + dir.val[0] * roots.val[i] - monkey_saddle_origin.val[0];
	//	coord.val[1] = orig.val[1] + dir.val[1] * roots.val[i] - monkey_saddle_origin.val[1];
	//	coord.val[2] = orig.val[2] + dir.val[2] * roots.val[i] - monkey_saddle_origin.val[2];
		double	x, y, z;
		x = ox + dx * roots.val[i];
		y = oy + dy * roots.val[i];
		z = oz + dz * roots.val[i];
		if (x > -1 && x < 1 && y > -1 && y < 1 && z > -1 && z < 1 && roots.val[i] < *(dist.dist) && roots.val[i] > dist.min_dist && roots.val[i] < dist.max_dist)
		{

				check = 1;
				*(dist.dist) = roots.val[i];

	/*		double	v;
			double	u;
			double	sin_v_2;

			v = atan2(coord.val[1], coord.val[0]);
			if (!is_null((sin_v_2 = sin(v / 2))))
				u = coord.val[2] / sin_v_2;
			else if (!is_null(v))
				u = (coord.val[0] / cos(v) - param->radius) / cos(v / 2);
			else
				u = (coord.val[0] - param->radius);
		//	}
	//		double	x = coord.val[0];
	//		double	y = coord.val[1];
	//		double	z = coord.val[2];
	//		printf("%f %f \n", -param->radius * param->radius * y + x * x * y + y * y * y - 2 * param->radius * x * z - 2 * x * x * z - 2 * y * y * z + y * z * z , u);
	//		if (-param->radius * param->radius * y + x * x * y + y * y * y - 2 * param->radius * x * z - 2 * x * x * z - 2 * y * y * z + y * z * z < 0.00001 && -param->radius * param->radius * y + x * x * y + y * y * y - 2 * param->radius * x * z - 2 * x * x * z - 2 * y * y * z + y * z * z > -0.00001)
	//			;
			//else
	//		{
			coord.val[0] -= (param->radius + u * cos(v / 2)) * cos(v);
			coord.val[1] -= (param->radius + u * cos(v / 2)) * sin(v);
			coord.val[2] -= u * sin_v_2;

//			u = (coord.val[2] / 2) / (sin(v));
*/		/*	if (coord.val[0] < -1.99 && coord.val[0] > -2.01 && coord.val[1] > -0.01 && coord.val[1] < 0.01 && coord.val[2] > -1 && coord.val[2] < -0.5)
				//printf("wefwef\n");
			{
				printf("type A coord %f %f %f y / x : %f u %f v %f\n", coord.val[0], coord.val[1], coord.val[2], coord.val[1] / coord.val[0],  u, v);
			//	check = 1;
			//	*dist = roots.val[i];
			}
		*///	if (coord.val[0] > 0 && u > -1 && u < 1)// && coord.val[0] < 2.01 && coord.val[1] > 0.5 && coord.val[1] < 1 && coord.val[2] < 1 && coord.val[2] > 0.5)
		//	{
			//	printf("type B coord %f %f %f u %f v %f\n", coord.val[0], coord.val[1], coord.val[2], u, v);
			//	check = 1;
			//	*dist = roots.val[i];
		//	}
				//			if (v > 0 && v < 2 * M_PI)
		//	printf("%f\n",coord.val[0] * coord.val[0] + coord.val[1] * coord.val[1] + coord.val[2] * coord.val[2] );
		//	if (monkey_saddle->check_inside(coord, monkey_saddle))
		//	if (is_null(coord.val[0] * coord.val[0] + coord.val[1] * coord.val[1] + coord.val[2] * coord.val[2]) && u < param->half_width && u > -param->half_width)
		//	{

		//		check = 1;
		//		*dist = roots.val[i];
		//	}
		}
	}
	return (check);
}

void	generate_new_monkey_saddle(t_data *data)
{
	t_obj		*monkey_saddle;
	t_monkey_saddle	*param;
	t_3vecf		dir;

	dir = mult_3vecf_33matf(mult_3vecf_33matf(window_to_view(0, 0, data->size.val[0], data->size.val[1]), data->rot_mat[1]), data->rot_mat[0]);
	normalize_3vecf(&dir);
	if (!(monkey_saddle = ft_memalloc(sizeof(t_obj))))
		return ;
	if (!(param = ft_memalloc(sizeof(t_monkey_saddle))))
		return ;
	param->origin.val[0] = data->camera->origin.val[0] + dir.val[0] * 2;
	param->origin.val[1] = data->camera->origin.val[1] + dir.val[1] * 2;
	param->origin.val[2] = data->camera->origin.val[2] + dir.val[2] * 2;

	//param->normal = assign_3vecf(get_random_number((time(NULL) * 0xcacacaca) << 16) - 0.5, get_random_number((time(NULL) * 0xfeabcdef) << 8) - 0.5, get_random_number((time(NULL) * 0x1056ffe) << 4) - 0.5);
	// normalize_3vecf(&param->normal);
/*	while (!is_null(dot_product_3vecf(param->normal, param->x2d_axis)))
	{
		param->x2d_axis = assign_3vecf(get_random_number(rd * 0xcacacaca << 16) - 0.5, get_random_number(rd * 0xfeabcdef << 8) - 0.5, get_random_number(rd * 0x1056ffe << 4) - 0.5);
		rd *= time(NULL);
		normalize_3vecf(&param->x2d_axis);
		printf("asad\n");
	} */
	monkey_saddle->obj_param = param;
	monkey_saddle->obj_type = OBJ_MONKEY_SADDLE;
	monkey_saddle->check_inside = &check_inside_monkey_saddle;
	monkey_saddle->ray_intersect = &ray_intersect_monkey_saddle;
	monkey_saddle->get_normal_inter = &get_normal_intersect_monkey_saddle;
	monkey_saddle->get_origin = &get_origin_monkey_saddle;
	monkey_saddle->move = &move_monkey_saddle;
	monkey_saddle->rotate = NULL;
	monkey_saddle->get_text_coordinate = &get_text_coordinate_monkey_saddle;
	monkey_saddle->get_text_color = &get_uni_color;
	monkey_saddle->text = generate_random_texture(monkey_saddle);
	set_bump_own(monkey_saddle);
	add_object(monkey_saddle, data);
	data->new_obj = 1;
}
/*int		parse_monkey_saddle(char *line, t_data *data)
{
	int			i;
	t_obj		*monkey_saddle;
	t_monkey_saddle	*monkey_saddle_param;

	if (!(monkey_saddle = malloc(sizeof(:%t_obj))) || !(monkey_saddle_param = malloc(sizeof(t_monkey_saddle))))
		return (0);
	i = 6;
	while (ft_isspace(line[i]))
		++i;
	if (line[i] != '(' || (i = parse_3vecf(line, i, &monkey_saddle_param->origin)) == -1)
	{
		ft_printf("Syntax error: monkey_saddle syntax: monkey_saddle(origin)(radius)(color)(reflection)(refraction)\n");
		return (0);
	}
	while (ft_isspace(line[i]))
		++i;
	if (line[i] != '(' || (i = parse_double(line, i, &monkey_saddle_param->radius)) == -1)
	{
		ft_printf("Syntax error: monkey_saddle syntax: monkey_saddle(origin)(radius)(color)(reflection)(refraction)\n");
		return (0);
	}
	while (ft_isspace(line[i]))
		++i;
	if (line[i] != '(' || (i = parse_texture(line, i, monkey_saddle)) == -1)
	{
		ft_printf("Syntax error: monkey_saddle syntax: monkey_saddle(origin)(radius)(color)(reflection)(refraction)\n");
		return (0);
	}
	while (ft_isspace(line[i]))
		++i;
	if (line[i] != '(' || (i = parse_double(line, i, &monkey_saddle->reflection)) == -1)
	{
		ft_printf("Syntax error: monkey_saddle syntax: monkey_saddle(origin)(radius)(color)(reflection)(refraction)\n");
		return (0);
	}
	while (ft_isspace(line[i]))
		++i;
	if (line[i] != '(' || (i = parse_double(line, i, &monkey_saddle->refraction)) == -1)
	{
		ft_printf("Syntax error: monkey_saddle syntax: monkey_saddle(origin)(radius)(color)(reflection)(refraction)\n");
		return (0);
	}

	//printf("monkey_saddle : %f %f %f && %f && %f %f %f\n", monkey_saddle_param->origin.val[0], monkey_saddle_param->origin.val[1], monkey_saddle_param->origin.val[2], monkey_saddle_param->radius, monkey_saddle->color.val[0], monkey_saddle->color.val[1], monkey_saddle->color.val[2]);
	monkey_saddle->obj_param = monkey_saddle_param;
	monkey_saddle->obj_type = OBJ_monkey_saddle;
	monkey_saddle->ray_intersect = &ray_intersect_monkey_saddle;
	monkey_saddle->get_normal_inter = &get_normal_intersect_monkey_saddle;
	monkey_saddle->get_text_coordinate = &get_text_coordinate_monkey_saddle;
	if (data->objs)
	{
		monkey_saddle->next = data->objs;
	}
	else
		monkey_saddle->next = NULL;
	data->objs = monkey_saddle;
	return (1);
}*/
