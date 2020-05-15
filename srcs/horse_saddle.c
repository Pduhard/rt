/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   horse_saddle.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aplat <aplat@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/15 18:52:21 by aplat             #+#    #+#             */
/*   Updated: 2020/05/15 18:57:06 by aplat            ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

int		check_inside_horse_saddle(t_3vecf point, t_obj *horse_saddle)
{
	t_horse_saddle	*param;

	param = (t_horse_saddle *)horse_saddle->obj_param;
	point = sub_3vecf(point, param->origin);
	if (point.val[0] < -param->x_fact || point.val[0] > param->x_fact
		|| point.val[1] < -param->y_fact || point.val[1] > param->y_fact)
		return (0);
	point.val[0] = (point.val[0] * point.val[0])
		/ (param->x_fact * param->x_fact);
	point.val[1] = (point.val[1] * point.val[1])
		/ (param->y_fact * param->y_fact);
	if (!is_null(point.val[0] + point.val[1] + point.val[2]))
		return (0);
	return (1);
}

t_2vecf	get_text_coordinate_horse_saddle(t_3vecf inter_point,
	t_3vecf normal_inter, t_obj *horse_saddle)
{
	return ((t_2vecf){{0, 0}});
	(void)inter_point;
	(void)horse_saddle;
	(void)normal_inter;
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

t_3vecf	get_origin_horse_saddle(t_obj *horse_saddle) // atej
{
	return (((t_horse_saddle *)horse_saddle->obj_param)->origin);
}

t_3vecf	get_horse_saddle_origin(t_obj *horse_saddle, t_horse_saddle *param,
	int sp_id)
{
	if (sp_id)
		return (move_3vecf(param->origin, horse_saddle->motions, sp_id));
	return (param->origin);
}

t_3vecf	get_normal_intersect_horse_saddle(t_3vecf inter_point,
	t_obj *horse_saddle, int sp_id)
{
	t_horse_saddle	*param;
	t_3vecf			normal_inter;
	double			x;
	double			z;
	t_3vecf			horse_saddle_origin;

	param = (t_horse_saddle *)horse_saddle->obj_param;
	horse_saddle_origin = get_horse_saddle_origin(horse_saddle, param, sp_id);
	x = inter_point.val[0] - horse_saddle_origin.val[0];
	z = inter_point.val[2] - horse_saddle_origin.val[2];
	normal_inter.val[0] = (2 * x) / (param->x_fact * param->x_fact);
	normal_inter.val[1] = -1;
	normal_inter.val[2] = (-2 * z) / (param->y_fact * param->y_fact);
	normalize_3vecf(&normal_inter);
	return (normal_inter);
}

t_3vecf	get_horse_saddle_quadratic_cst(t_horse_saddle *param,
	t_3vecf horse_saddle_origin, t_leq l)
{
	t_3vecf		cst;
	double		o[3];
	double		d[3];
	double		a;
	double		b;

	o[0] = l.orig.val[0] - horse_saddle_origin.val[0];
	o[1] = l.orig.val[1] - horse_saddle_origin.val[1];
	o[2] = l.orig.val[2] - horse_saddle_origin.val[2];
	d[0] = l.dir.val[0];
	d[1] = l.dir.val[1];
	d[2] = l.dir.val[2];
	a = param->x_fact;
	b = param->y_fact;
	cst.val[2] = (o[0] * o[0]) / (a * a) - (o[2] * o[2]) / (b * b) - o[1];
	cst.val[1] = (2 * o[0] * d[0]) / (a * a) - (2 * o[2] * d[2])
		/ (b * b) - d[1];
	cst.val[0] = (d[0] * d[0]) / (a * a) - (d[2] * d[2]) / (b * b);
	return (cst);
}

int		check_horse_saddle_bnd(t_leq l, t_3vecf horse_saddle_origin,
	double root, t_horse_saddle *param)
{
	double	x;
	double	y;
	double	z;

	x = l.orig.val[0] - horse_saddle_origin.val[0] + l.dir.val[0] * root;
	y = l.orig.val[1] - horse_saddle_origin.val[1] + l.dir.val[1] * root;
	z = l.orig.val[2] - horse_saddle_origin.val[2] + l.dir.val[2] * root;
	if (x > -param->x_fact && x < param->x_fact && z > -param->y_fact
		&& z < param->y_fact)
		return (1);
	return (0);
}

int		ray_intersect_horse_saddle(t_leq l, t_obj *horse_saddle,
	t_dist dist, int sp_id)
{
	t_horse_saddle	*param;
	t_3vecf			cst;
	t_2vecf			roots;
	int				check;
	t_3vecf			horse_saddle_origin;

	check = 0;
	param = (t_horse_saddle *)horse_saddle->obj_param;
	horse_saddle_origin = get_horse_saddle_origin(horse_saddle, param, sp_id);
	cst = get_horse_saddle_quadratic_cst(param, horse_saddle_origin, l);
	roots = solve_quadratic(cst.val[0], cst.val[1], cst.val[2]);
	if (check_horse_saddle_bnd(l, horse_saddle_origin, roots.val[0], param))
		check |= is_closest_intersect(dist, roots.val[0]);
	if (check_horse_saddle_bnd(l, horse_saddle_origin, roots.val[1], param))
		check |= is_closest_intersect(dist, roots.val[1]);
	return (check);
}

void	assign_horse_saddle_function(t_obj *horse_saddle)
{
	horse_saddle->obj_type = OBJ_HORSE_SADDLE;
	horse_saddle->check_inside = &check_inside_horse_saddle;
	horse_saddle->ray_intersect = &ray_intersect_horse_saddle;
	horse_saddle->get_normal_inter = &get_normal_intersect_horse_saddle;
	horse_saddle->get_origin = &get_origin_horse_saddle;
	horse_saddle->move = &move_horse_saddle;
	horse_saddle->rotate = NULL;
	horse_saddle->get_text_coordinate = &get_text_coordinate_horse_saddle;
}

void	generate_new_horse_saddle(t_data *data)
{
	t_obj			*horse_saddle;
	t_horse_saddle	*param;
	t_3vecf			dir;

	dir = mult_3vecf_33matf(mult_3vecf_33matf(window_to_view(0, 0,
		data->size.val[0], data->size.val[1]), data->rot_mat[1]),
			data->rot_mat[0]);
	normalize_3vecf(&dir);
	if (!(horse_saddle = ft_memalloc(sizeof(t_obj))))
		return ;
	if (!(param = ft_memalloc(sizeof(t_horse_saddle))))
		return ;
	param->origin = add_3vecf(data->camera->origin, product_c3vecf(dir, 2));
	param->x_fact = get_random_number((time(NULL) * 0xcacacaca) << 16) * 2.5;
	param->y_fact = get_random_number((time(NULL) * 0xabcdef99) << 4) * 2.5;
	horse_saddle->obj_param = param;
	assign_horse_saddle_function(horse_saddle);
	horse_saddle->text = generate_random_texture(horse_saddle);
	set_bump_own(horse_saddle);
	add_object(horse_saddle, data);
	data->new_obj = 1;
}
