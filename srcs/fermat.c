/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fermat.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pduhard- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/26 05:56:10 by pduhard-          #+#    #+#             */
/*   Updated: 2020/02/28 23:54:28 by pduhard-         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

int		check_inside_fermat(t_3vecf point, t_obj *fermat)
{
	t_fermat	*param;
	double	v;
	double	u;
//	double	sin_v_2;

	return (0);
	/* ne need to do*/
	param = (t_fermat *)fermat->obj_param;
	v = atan2(point.val[1], point.val[0]);
	u = (point.val[0]);
	point.val[2] -= u + v;

	if (is_null(point.val[0] * point.val[0] + point.val[1] * point.val[1] + point.val[2] * point.val[2]))
		return (1);
	return (0);
}

t_2vecf	get_text_coordinate_fermat(t_3vecf inter_point, t_3vecf normal_inter, t_obj *fermat)
{
	t_2vecf	text_coord;
//	double	sin_v_2;
	t_fermat	*param;

	param = (t_fermat *)fermat->obj_param;
	text_coord.val[1] = atan2(inter_point.val[1], inter_point.val[0]);
/*	if (!is_null((sin_v_2 = sin(text_coord.val[1] / 2))))
		text_coord.val[0] = inter_point.val[2] / sin_v_2;
	else if (!is_null(text_coord.val[1]))
		text_coord.val[0] = (inter_point.val[0] / cos(text_coord.val[1]) - param->radius) / cos(text_coord.val[1] / 2);
	else
		text_coord.val[0] = (inter_point.val[0] - param->radius);
*/
/* FALSE need to get from parametric equation*/

	//printf("%f %f\n", text_coord.val[0], text_coord.val[1]);
	text_coord.val[0] = 0 * (text_coord.val[0]+  1) / (2 * 1);
	text_coord.val[1] = 0 * M_PI * 2;
	return (text_coord);
	(void)normal_inter;
}

void	move_fermat(t_obj *fermat, t_3vecf dir, double fact)
{
	t_fermat	*param;
	t_cut		*cuts;

	param = (t_fermat *)fermat->obj_param;
	param->origin.val[0] += dir.val[0] * fact;
	param->origin.val[1] += dir.val[1] * fact;
	param->origin.val[2] += dir.val[2] * fact;
	cuts = fermat->cuts;
	while (cuts)
	{
		if (cuts->move && cuts->cut_type != CUT_STATIC)
			cuts->move(cuts, dir, fact);
		cuts = cuts->next;
	}
}

t_3vecf	get_origin_fermat(t_obj *fermat)
{
	return (((t_fermat *)fermat->obj_param)->origin);
}

t_3vecf	get_normal_intersect_fermat(t_3vecf inter_point, t_obj *fermat, int sp_id)
{
	t_fermat	*param;
	t_3vecf		normal_inter;
	double		x;
	double		y;
	double		z;
	t_3vecf		fermat_origin;

	param = (t_fermat *)fermat->obj_param;
	fermat_origin = sp_id ? move_3vecf(param->origin, fermat->motions, sp_id) : param->origin;
	x = inter_point.val[0] - fermat_origin.val[0];
	y = inter_point.val[1] - fermat_origin.val[1];
	z = inter_point.val[2] - fermat_origin.val[2];
	normal_inter.val[0] = 3 * x * x;
	normal_inter.val[1] = 3 * y * y;
	normal_inter.val[2] = 3 * z * z;
	normalize_3vecf(&normal_inter);
	return (normal_inter);
}

int	ray_intersect_fermat(t_3vecf orig, t_3vecf dir, t_obj *fermat, double *dist, double min_dist, double max_dist, int sp_id)
{
	t_fermat	*param;
	double		ox;
	double		oy;
	double		oz;
	double		dx;
	double		dy;
	double		dz;
	t_4vecf		t_fact;
	t_3vecf		roots;
	int			check;
	t_3vecf		fermat_origin;

	(void)sp_id;
	check = 0;
	param = (t_fermat *)fermat->obj_param;
	fermat_origin = sp_id ? move_3vecf(param->origin, fermat->motions, sp_id) : param->origin;
	ox = orig.val[0] - fermat_origin.val[0];
	oy = orig.val[1] - fermat_origin.val[1];
	oz = orig.val[2] - fermat_origin.val[2];
	dx = dir.val[0];
	dy = dir.val[1];
	dz = dir.val[2];

	t_fact.val[0] = dz * dz * dz + dx * dx * dx + dy * dy * dy;//a
	t_fact.val[1] = 3 * oz * dz * dz + 3 * ox * dx * dx + 3 * oy * dy * dy;//b
	t_fact.val[2] = 3 * oz * oz * dz + 3 * oy * oy * dy + 3 * ox * ox *dx;//c
	t_fact.val[3] = oz * oz * oz + ox * ox * ox + oy * oy * oy - 1;//d

	roots = solve_cubic(t_fact.val[0], t_fact.val[1], t_fact.val[2], t_fact.val[3]);
	int		i = -1;
//	printf("%f %f %f\n", roots.val[0], roots.val[1], roots.val[2]);
	while (++i < 3)
	{
		t_3vecf	coord;
	//	t_3vecf	coord;
		coord.val[0] = orig.val[0] + dir.val[0] * roots.val[i] - fermat_origin.val[0];
		coord.val[1] = orig.val[1] + dir.val[1] * roots.val[i] - fermat_origin.val[1];
		coord.val[2] = orig.val[2] + dir.val[2] * roots.val[i] - fermat_origin.val[2];
		double	x, y, z;
		x = ox + dx * roots.val[i];
		y = oy + dy * roots.val[i];
		z = oz + dz * roots.val[i];
		if (x > -3 && x < 3 && y > -3 && y < 3 && z > -3 && z < 3 && roots.val[i] < *dist && roots.val[i] > min_dist && roots.val[i] < max_dist)
		{
			coord.val[0] = orig.val[0] + dir.val[0] * roots.val[i] - fermat_origin.val[0];
			coord.val[1] = orig.val[1] + dir.val[1] * roots.val[i] - fermat_origin.val[1];
			coord.val[2] = orig.val[2] + dir.val[2] * roots.val[i] - fermat_origin.val[2];
			check = 1;
			*dist = roots.val[i];
		}
	}
	return (check);
}

/*int		parse_fermat(char *line, t_data *data)
{
	int			i;
	t_obj		*fermat;
	t_fermat	*fermat_param;

	if (!(fermat = malloc(sizeof(t_obj))) || !(fermat_param = malloc(sizeof(t_fermat))))
		return (0);
	i = 6;
	while (ft_isspace(line[i]))
		++i;
	if (line[i] != '(' || (i = parse_3vecf(line, i, &fermat_param->origin)) == -1)
	{
		ft_printf("Syntax error: fermat syntax: fermat(origin)(radius)(color)(reflection)(refraction)\n");
		return (0);
	}
	while (ft_isspace(line[i]))
		++i;
	if (line[i] != '(' || (i = parse_double(line, i, &fermat_param->radius)) == -1)
	{
		ft_printf("Syntax error: fermat syntax: fermat(origin)(radius)(color)(reflection)(refraction)\n");
		return (0);
	}
	while (ft_isspace(line[i]))
		++i;
	if (line[i] != '(' || (i = parse_texture(line, i, fermat)) == -1)
	{
		ft_printf("Syntax error: fermat syntax: fermat(origin)(radius)(color)(reflection)(refraction)\n");
		return (0);
	}
	while (ft_isspace(line[i]))
		++i;
	if (line[i] != '(' || (i = parse_double(line, i, &fermat->reflection)) == -1)
	{
		ft_printf("Syntax error: fermat syntax: fermat(origin)(radius)(color)(reflection)(refraction)\n");
		return (0);
	}
	while (ft_isspace(line[i]))
		++i;
	if (line[i] != '(' || (i = parse_double(line, i, &fermat->refraction)) == -1)
	{
		ft_printf("Syntax error: fermat syntax: fermat(origin)(radius)(color)(reflection)(refraction)\n");
		return (0);
	}

	//printf("fermat : %f %f %f && %f && %f %f %f\n", fermat_param->origin.val[0], fermat_param->origin.val[1], fermat_param->origin.val[2], fermat_param->radius, fermat->color.val[0], fermat->color.val[1], fermat->color.val[2]);
	fermat->obj_param = fermat_param;
	fermat->obj_type = OBJ_fermat;
	fermat->ray_intersect = &ray_intersect_fermat;
	fermat->get_normal_inter = &get_normal_intersect_fermat;
	fermat->get_text_coordinate = &get_text_coordinate_fermat;
	if (data->objs)
	{
		fermat->next = data->objs;
	}
	else
		fermat->next = NULL;
	data->objs = fermat;
	return (1);
}*/
