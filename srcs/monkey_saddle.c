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
	return (0);
	(void)point;
	(void)monkey_saddle;
}

t_2vecf	get_text_coordinate_monkey_saddle(t_3vecf inter_point, t_3vecf normal_inter, t_obj *monkey_saddle)
{
	return ((t_2vecf){{0, 0}});
	(void)normal_inter;
	(void)inter_point;
	(void)monkey_saddle;
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

t_3vecf get_monkey_saddle_origin(t_obj *monkey_saddle, t_monkey_saddle *param, int sp_id)
{
		if (sp_id)
			return (move_3vecf(param->origin, monkey_saddle->motions, sp_id));
		return (param->origin);
}

t_3vecf	get_normal_intersect_monkey_saddle(t_3vecf inter_point, t_obj *monkey_saddle, int sp_id)
{
	t_monkey_saddle	*param;
	t_3vecf		normal_inter;
	double		x;
	double		z;
	t_3vecf		monkey_saddle_origin;

	param = (t_monkey_saddle *)monkey_saddle->obj_param;
	monkey_saddle_origin = get_monkey_saddle_origin(monkey_saddle, param, sp_id);
	x = inter_point.val[0] - monkey_saddle_origin.val[0];
	z = inter_point.val[2] - monkey_saddle_origin.val[2];
	normal_inter.val[0] = 3 * x * x - 3 * z * z;
	normal_inter.val[1] = -1;
	normal_inter.val[2] = -6 * x * z;
	normalize_3vecf(&normal_inter);
	return (normal_inter);
}

t_4vecf get_monkey_saddle_cubis_cst(t_3vecf monkey_saddle_origin, t_leq l)
{
	double		o[3];
	double		d[3];
	t_4vecf		cst;

	o[0] = l.orig.val[0] - monkey_saddle_origin.val[0];
	o[1] = l.orig.val[1] - monkey_saddle_origin.val[1];
	o[2] = l.orig.val[2] - monkey_saddle_origin.val[2];
	d[0] = l.dir.val[0];
	d[1] = l.dir.val[1];
	d[2] = l.dir.val[2];
	cst.val[0] = (-3 * d[0] * d[2] * d[2]) + (d[0] * d[0] * d[0]);//a
	cst.val[1] = (-3 * o[0] * d[2] * d[2]) - (6 * o[2] * d[2] * d[0])
						 + (d[0] * d[0] * o[0]) + (2 * o[0] * d[0] * d[0]);//b
	cst.val[2] = (-6 * o[0] * o[2] * d[2]) - (3 * o[2] * o[2] * d[0]) - d[1]
						 + (2 * o[0] * o[0] * d[0]) + (o[0] * o[0] * d[0]);//c
	cst.val[3] = (-3 * o[2] * o[2] * o[0]) + (o[0] * o[0] * o[0]) - o[1];//d
	return (cst);
}

int check_monkey_saddle_bnd(t_leq l, t_3vecf monkey_saddle_origin, double root)
{
	double	x;
	double	y;
	double	z;

	x = l.orig.val[0] - monkey_saddle_origin.val[0] + l.dir.val[0] * root;
	y = l.orig.val[1] - monkey_saddle_origin.val[1] + l.dir.val[1] * root;
	z = l.orig.val[2] - monkey_saddle_origin.val[2] + l.dir.val[2] * root;
	if (x > -1 && x < 1 && y > -1 && y < 1 && z > -1 && z < 1)
		return (1);
	return (0);
}

int	ray_intersect_monkey_saddle(t_leq l, t_obj *monkey_saddle,
	t_dist dist, int sp_id)
{
	t_4vecf	cst;
	t_3vecf	roots;
	int			check;
	t_3vecf	monkey_saddle_origin;
	int			i;

 	i = -1;
	check = 0;
	monkey_saddle_origin = get_monkey_saddle_origin(monkey_saddle,
		(t_monkey_saddle *)monkey_saddle->obj_param, sp_id);
	cst = get_monkey_saddle_cubis_cst(monkey_saddle_origin, l);
	roots = solve_cubic(cst.val[0], cst.val[1], cst.val[2], cst.val[3]);
	while (++i < 3)
		if (check_monkey_saddle_bnd(l, monkey_saddle_origin, roots.val[i]))
			check |= is_closest_intersect(dist, roots.val[i]);
	return (check);
}

void  assign_monkey_saddle_function(t_obj *monkey_saddle)
{
	monkey_saddle->obj_type = OBJ_MONKEY_SADDLE;
	monkey_saddle->check_inside = &check_inside_monkey_saddle;
	monkey_saddle->ray_intersect = &ray_intersect_monkey_saddle;
	monkey_saddle->get_normal_inter = &get_normal_intersect_monkey_saddle;
	monkey_saddle->get_origin = &get_origin_monkey_saddle;
	monkey_saddle->move = &move_monkey_saddle;
	monkey_saddle->rotate = NULL;
	monkey_saddle->get_text_coordinate = &get_text_coordinate_monkey_saddle;
}

void	generate_new_monkey_saddle(t_data *data)
{
	t_obj		*monkey_saddle;
	t_monkey_saddle	*param;
	t_3vecf		dir;

	dir = mult_3vecf_33matf(mult_3vecf_33matf(window_to_view(0, 0,
		data->size.val[0], data->size.val[1]), data->rot_mat[1]), data->rot_mat[0]);
	normalize_3vecf(&dir);
	if (!(monkey_saddle = ft_memalloc(sizeof(t_obj))))
		return ;
	if (!(param = ft_memalloc(sizeof(t_monkey_saddle))))
		return ;
	param->origin = add_3vecf(data->camera->origin, product_c3vecf(dir, 2));
	monkey_saddle->obj_param = param;
	assign_monkey_saddle_function(monkey_saddle);
	monkey_saddle->text = generate_random_texture(monkey_saddle);
	set_bump_own(monkey_saddle);
	add_object(monkey_saddle, data);
	data->new_obj = 1;
}
