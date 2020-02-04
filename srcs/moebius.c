/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   moebius.c                                        .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: pduhard- <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2020/01/31 18:29:04 by pduhard-     #+#   ##    ##    #+#       */
/*   Updated: 2020/02/04 07:49:43 by pduhard-    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "rt.h"

t_2vecf	get_text_coordinate_moebius(t_3vecf inter_point, t_3vecf normal_inter, t_obj *moebius)
{
	t_2vecf	text_coord;
/* FALSE need to get from parametric equation*/

	text_coord.val[1] = (1 - fmod((atan2(normal_inter.val[0], normal_inter.val[2]) / (2 * M_PI) + 0.5), 1));
	text_coord.val[0] = (normal_inter.val[1] * 0.5 + 0.5);
	//printf("%f %f\n", text_coord.val[0], text_coord.val[1]);
	return (text_coord);
	(void)inter_point;
	(void)moebius;
}

void	move_moebius(t_obj *moebius, t_3vecf dir, double fact)
{
	t_moebius	*param;

	param = (t_moebius *)moebius->obj_param;
	param->origin.val[0] += dir.val[0] * fact;
	param->origin.val[1] += dir.val[1] * fact;
	param->origin.val[2] += dir.val[2] * fact;
}

t_3vecf	get_origin_moebius(t_obj *moebius)
{
	return (((t_moebius *)moebius->obj_param)->origin);
}

t_3vecf	get_normal_intersect_moebius(t_3vecf inter_point, t_obj *moebius, int sp_id)
{
	t_moebius	*param;
	t_3vecf		normal_inter;
	double		x;
	double		y;
	double		z;
	t_3vecf		moebius_origin;

	param = (t_moebius *)moebius->obj_param;
	moebius_origin = sp_id ? move_3vecf(param->origin, moebius->motions, sp_id) : param->origin;
	x = inter_point.val[0] - moebius_origin.val[0];
	y = inter_point.val[1] - moebius_origin.val[1];
	z = inter_point.val[2] - moebius_origin.val[2];
	normal_inter.val[0] = 2 * x * y - 2 * param->radius * z - 4 * x * z;
	normal_inter.val[1] = -param->radius * param->radius + x * x + 3 * y * y - 4 * y * z + z * z;
	normal_inter.val[2] = -2 * param->radius * x - 2 * x * x - 2 * y * y + 2 * y * z;
	normalize_3vecf(&normal_inter);
	return (normal_inter);
}

int	ray_intersect_moebius(t_3vecf orig, t_3vecf dir, t_obj *moebius, double *dist, double min_dist, double max_dist, int sp_id)
{
	t_moebius	*param;
	double		ox;
	double		oy;
	double		oz;
	double		vx;
	double		vy;
	double		vz;
	t_4vecf		t_fact;
	t_3vecf		roots;
	int			check;
	t_3vecf		moebius_origin;

	(void)sp_id;
	check = 0;
	param = (t_moebius *)moebius->obj_param;
	moebius_origin = sp_id ? move_3vecf(param->origin, moebius->motions, sp_id) : param->origin;
	ox = orig.val[0] - moebius_origin.val[0];
	oy = orig.val[1] - moebius_origin.val[1];
	oz = orig.val[2] - moebius_origin.val[2];
	vx = dir.val[0];
	vy = dir.val[1];
	vz = dir.val[2];

	t_fact.val[3] = (-param->radius * param->radius * oy) + (ox * ox * oy) + (oy * oy * oy) - (2 * param->radius * ox * oz) - (2 * ox * ox * oz) - (2 * oy * oy * oz) + (oy * oz * oz);//d
	t_fact.val[2] = (-param->radius * param->radius * vy) + (ox * ox * vy) + (2 * ox * oy * vx) + (oy * oy * vy) + (2 * oy * oy * vy)  - (2 * param->radius * ox * vz) - (2 * param->radius * oz * vx) - (2 * ox * ox * vz) - (4 * ox * oz * vx) - (2 * oy * oy * vz) - (4 * oy * oz * vy) + (2 * oy * oz * vz) + (oz * oz * vy);//c
	t_fact.val[1] = (2 * ox * vx * vy) + (oy * vx * vx) + (2 * oy * vy * vy) + (oy * vy * vy) - (2 * param->radius * vx * vz) - (4 * ox * vx * vz) - (2 * oz * vx * vx) - (4 * oy * vy * vz) - (2 * oz * vy * vy) + (oy * vz * vz) + (2 * oz * vy * vz);//b
	t_fact.val[0] = (vx * vx * vy) + (vy * vy * vy) - (2 * vx * vx * vz) - (2 * vy * vy * vz) + (vy * vz * vz);//a

	roots = solve_cubic(t_fact.val[0], t_fact.val[1], t_fact.val[2], t_fact.val[3]);
	int		i = -1;
//	printf("%f %f %f\n", roots.val[0], roots.val[1], roots.val[2]);
	while (++i < 3)
	{
		t_3vecf	coord;
		if (roots.val[i] < *dist && roots.val[i] > min_dist && roots.val[i] < max_dist)
		{
			coord.val[0] = orig.val[0] + dir.val[0] * roots.val[i] - moebius_origin.val[0];
			coord.val[1] = orig.val[1] + dir.val[1] * roots.val[i] - moebius_origin.val[1];
			coord.val[2] = orig.val[2] + dir.val[2] * roots.val[i] - moebius_origin.val[2];

			double	v;
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
		/*	if (coord.val[0] < -1.99 && coord.val[0] > -2.01 && coord.val[1] > -0.01 && coord.val[1] < 0.01 && coord.val[2] > -1 && coord.val[2] < -0.5)
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
			if (is_null(coord.val[0] * coord.val[0] + coord.val[1] * coord.val[1] + coord.val[2] * coord.val[2]) && u < param->half_width && u > -param->half_width)
			{
				
				check = 1;
				*dist = roots.val[i];
			}
		}
	}
	return (check);
}

/*int		parse_moebius(char *line, t_data *data)
{
	int			i;
	t_obj		*moebius;
	t_moebius	*moebius_param;

	if (!(moebius = malloc(sizeof(t_obj))) || !(moebius_param = malloc(sizeof(t_moebius))))
		return (0);
	i = 6;
	while (ft_isspace(line[i]))
		++i;
	if (line[i] != '(' || (i = parse_3vecf(line, i, &moebius_param->origin)) == -1)
	{
		ft_printf("Syntax error: moebius syntax: moebius(origin)(radius)(color)(reflection)(refraction)\n");
		return (0);
	}
	while (ft_isspace(line[i]))
		++i;
	if (line[i] != '(' || (i = parse_double(line, i, &moebius_param->radius)) == -1)
	{
		ft_printf("Syntax error: moebius syntax: moebius(origin)(radius)(color)(reflection)(refraction)\n");
		return (0);
	}
	while (ft_isspace(line[i]))
		++i;
	if (line[i] != '(' || (i = parse_texture(line, i, moebius)) == -1)
	{
		ft_printf("Syntax error: moebius syntax: moebius(origin)(radius)(color)(reflection)(refraction)\n");
		return (0);
	}
	while (ft_isspace(line[i]))
		++i;
	if (line[i] != '(' || (i = parse_double(line, i, &moebius->reflection)) == -1)
	{
		ft_printf("Syntax error: moebius syntax: moebius(origin)(radius)(color)(reflection)(refraction)\n");
		return (0);
	}
	while (ft_isspace(line[i]))
		++i;
	if (line[i] != '(' || (i = parse_double(line, i, &moebius->refraction)) == -1)
	{
		ft_printf("Syntax error: moebius syntax: moebius(origin)(radius)(color)(reflection)(refraction)\n");
		return (0);
	}

	//printf("moebius : %f %f %f && %f && %f %f %f\n", moebius_param->origin.val[0], moebius_param->origin.val[1], moebius_param->origin.val[2], moebius_param->radius, moebius->color.val[0], moebius->color.val[1], moebius->color.val[2]);
	moebius->obj_param = moebius_param;
	moebius->obj_type = OBJ_moebius;
	moebius->ray_intersect = &ray_intersect_moebius;
	moebius->get_normal_inter = &get_normal_intersect_moebius;
	moebius->get_text_coordinate = &get_text_coordinate_moebius;
	if (data->objs)
	{
		moebius->next = data->objs;
	}
	else
		moebius->next = NULL;
	data->objs = moebius;
	return (1);
}*/
