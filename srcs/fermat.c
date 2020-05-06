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
	if (param)
		param++;
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
	if (param)
		param++;
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

int	ray_intersect_fermat(t_leq l, t_obj *fermat, t_dist dist, int sp_id)
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
	ox = l.orig.val[0] - fermat_origin.val[0];
	oy = l.orig.val[1] - fermat_origin.val[1];
	oz = l.orig.val[2] - fermat_origin.val[2];
	dx = l.dir.val[0];
	dy = l.dir.val[1];
	dz = l.dir.val[2];

	t_fact.val[0] = dz * dz * dz + dx * dx * dx + dy * dy * dy;//a
	t_fact.val[1] = 3 * oz * dz * dz + 3 * ox * dx * dx + 3 * oy * dy * dy;//b
	t_fact.val[2] = 3 * oz * oz * dz + 3 * oy * oy * dy + 3 * ox * ox *dx;//c
	t_fact.val[3] = oz * oz * oz + ox * ox * ox + oy * oy * oy - 1;//d

	roots = solve_cubic(t_fact.val[0], t_fact.val[1], t_fact.val[2], t_fact.val[3]);
	int		i = -1;
//	printf("%f %f %f\n", roots.val[0], roots.val[1], roots.val[2]);
	while (++i < 3)
	{
	//	t_3vecf	coord;
	//	t_3vecf	coord;
	//	coord.val[0] = orig.val[0] + dir.val[0] * roots.val[i] - fermat_origin.val[0];
	//	coord.val[1] = orig.val[1] + dir.val[1] * roots.val[i] - fermat_origin.val[1];
	//	coord.val[2] = orig.val[2] + dir.val[2] * roots.val[i] - fermat_origin.val[2];
		double	x, y, z;
		x = ox + dx * roots.val[i];
		y = oy + dy * roots.val[i];
		z = oz + dz * roots.val[i];
		if (x > -3 && x < 3 && y > -3 && y < 3 && z > -3 && z < 3 && roots.val[i] < *(dist.dist) && roots.val[i] > dist.min_dist && roots.val[i] < dist.max_dist)
		{
		//	coord.val[0] = orig.val[0] + dir.val[0] * roots.val[i] - fermat_origin.val[0];
		//	coord.val[1] = orig.val[1] + dir.val[1] * roots.val[i] - fermat_origin.val[1];
		//	coord.val[2] = orig.val[2] + dir.val[2] * roots.val[i] - fermat_origin.val[2];
			check = 1;
			*(dist.dist) = roots.val[i];
		}
	}
	return (check);
}

void	generate_new_fermat(t_data *data)
{
	t_obj		*fermat;
	t_fermat	*param;
	t_3vecf		dir;

	dir = mult_3vecf_33matf(mult_3vecf_33matf(window_to_view(0, 0, data->size.val[0], data->size.val[1]), data->rot_mat[1]), data->rot_mat[0]);
	normalize_3vecf(&dir);
	if (!(fermat = ft_memalloc(sizeof(t_obj))))
		return ;
	if (!(param = ft_memalloc(sizeof(t_fermat))))
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
	fermat->obj_param = param;
	fermat->obj_type = OBJ_FERMAT;
	fermat->check_inside = &check_inside_fermat;
	fermat->ray_intersect = &ray_intersect_fermat;
	fermat->get_normal_inter = &get_normal_intersect_fermat;
	fermat->get_origin = &get_origin_fermat;
	fermat->move = &move_fermat;
	fermat->rotate = NULL;
	fermat->get_text_coordinate = &get_text_coordinate_fermat;
	fermat->get_text_color = &get_uni_color;
	fermat->text = generate_random_texture(fermat);
	set_bump_own(fermat);
	add_object(fermat, data);
	data->new_obj = 1;
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
