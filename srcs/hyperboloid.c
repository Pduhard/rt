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

t_2vecf	get_text_coordinate_hyperboloid(t_3vecf inter_point,
	t_3vecf normal_inter, t_obj *hyperboloid)
{
	return ((t_2vecf){{0, 0}});
	(void)inter_point;
	(void)hyperboloid;
	(void)normal_inter;
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

t_3vecf	get_origin_hyperboloid(t_obj *hyperboloid) // bye
{
	return (((t_hyperboloid *)hyperboloid->obj_param)->origin);
}

t_3vecf get_hyperboloid_origin(t_obj *hyperboloid,
	t_hyperboloid *hyperboloid_param, int sp_id)
{
	if (sp_id)
		return (move_3vecf(hyperboloid_param->origin, hyperboloid->motions, sp_id));
	return (hyperboloid_param->origin);
}

t_3vecf	get_normal_intersect_hyperboloid(t_3vecf inter_point,
	t_obj *hyperboloid, int sp_id)
{
	t_hyperboloid	*param;
	t_3vecf		normal_inter;
	t_3vecf		hyperboloid_origin;
	t_3vecf		cst;

	param = (t_hyperboloid *)hyperboloid->obj_param;
	hyperboloid_origin = get_hyperboloid_origin(hyperboloid, param, sp_id);
	cst = sub_3vecf(inter_point, hyperboloid_origin);
	normal_inter.val[0] = (2 * cst.val[0]) / (param->x_fact * param->x_fact);
	normal_inter.val[1] = (-2 * cst.val[1]) / (param->y_fact * param->y_fact);
	normal_inter.val[2] = (2 * cst.val[2]) / (param->z_fact * param->z_fact);
	normalize_3vecf(&normal_inter);
	return (normal_inter);
}

t_3vecf get_hyperboloid_quadratic_cst(t_hyperboloid *param,
	t_3vecf hyperboloid_origin, t_leq l)
{
	double		f[3];
	double		o[3];
	double		d[3];
	t_3vecf		cst;

	o[0] = l.orig.val[0] - hyperboloid_origin.val[0];
	o[1] = l.orig.val[1] - hyperboloid_origin.val[1];
	o[2] = l.orig.val[2] - hyperboloid_origin.val[2];
	d[0] = l.dir.val[0];
	d[1] = l.dir.val[1];
	d[2] = l.dir.val[2];
	f[0] = param->x_fact;
	f[1] = param->y_fact;
	f[2] = param->z_fact;
	cst.val[2] = (o[0] * o[0]) / (f[0] * f[0]) - (o[1] * o[1]) / (f[1] * f[1])
						 + (o[2] * o[2]) / (f[2] * f[2]) + param->surface;//f[2]
	cst.val[1] = (2 * o[0] * d[0]) / (f[0] * f[0]) - (2 * o[1] * d[1])
						 / (f[1] * f[1]) + (2 * o[2] * d[2]) / (f[2] * f[2]);//f[1]
	cst.val[0] = (d[0] * d[0]) / (f[0] * f[0]) - (d[1] * d[1]) / (f[1] * f[1])
						 + (d[2] * d[2]) / (f[2] * f[2]);//f[0]
	return (cst);
}

int	ray_intersect_hyperboloid(t_leq l, t_obj *hyperboloid, t_dist dist, int sp_id)
{
	t_hyperboloid	*param;
	t_2vecf				roots;
	int						check;
	t_3vecf				hyperboloid_origin;
	t_3vecf		cst;

	check = 0;
	param = (t_hyperboloid *)hyperboloid->obj_param;
	hyperboloid_origin = get_hyperboloid_origin(hyperboloid, param, sp_id);
	cst = get_hyperboloid_quadratic_cst(param, hyperboloid_origin, l);
	roots = solve_quadratic(cst.val[0], cst.val[1], cst.val[2]);
	check |= is_closest_intersect(dist, roots.val[0]);
	check |= is_closest_intersect(dist, roots.val[1]);
	return (check);
}

void  assign_hyperboloid_function(t_obj *hyperboloid)
{
	hyperboloid->obj_type = OBJ_HYPERBOLOID;
	hyperboloid->check_inside = &check_inside_hyperboloid;
	hyperboloid->ray_intersect = &ray_intersect_hyperboloid;
	hyperboloid->get_normal_inter = &get_normal_intersect_hyperboloid;
	hyperboloid->get_origin = &get_origin_hyperboloid;
	hyperboloid->move = &move_hyperboloid;
	hyperboloid->rotate = NULL;
	hyperboloid->get_text_coordinate = &get_text_coordinate_hyperboloid;
}

void	generate_new_hyperboloid(t_data *data)
{
	t_obj		*hyperboloid;
	t_hyperboloid	*param;
	t_3vecf		dir;

	dir = mult_3vecf_33matf(mult_3vecf_33matf(window_to_view(0, 0,
		data->size.val[0], data->size.val[1]), data->rot_mat[1]), data->rot_mat[0]);
	normalize_3vecf(&dir);
	if (!(hyperboloid = ft_memalloc(sizeof(t_obj))))
		return ;
	if (!(param = ft_memalloc(sizeof(t_hyperboloid))))
		return ;
	param->origin = add_3vecf(data->camera->origin, product_c3vecf(dir, 2));
	param->x_fact = get_random_number((time(NULL) * 0xcacacaca) << 16) * 2.5;
	param->y_fact = get_random_number((time(NULL) * 0xabcdef99) << 4) * 2.5;
	param->z_fact = get_random_number((time(NULL) * 0xff3672ff) << 3) * 2.5;
	param->surface = get_random_number(time(NULL)) > 0.5 ? -1 : 1;
	hyperboloid->obj_param = param;
	assign_hyperboloid_function(hyperboloid);
	hyperboloid->text = generate_random_texture(hyperboloid);
	set_bump_own(hyperboloid);
	add_object(hyperboloid, data);
	data->new_obj = 1;
}
