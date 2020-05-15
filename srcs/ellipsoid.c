/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ellipsoid.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aplat <aplat@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/15 17:58:29 by aplat             #+#    #+#             */
/*   Updated: 2020/05/15 18:04:20 by aplat            ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

int		check_inside_ellipsoid(t_3vecf point, t_obj *ellipsoid)
{
	t_ellipsoid	*param;

	param = (t_ellipsoid *)ellipsoid->obj_param;
	point = sub_3vecf(point, param->origin);
	point.val[0] = (point.val[0] * point.val[0])
		/ (param->x_fact * param->x_fact);
	point.val[1] = (point.val[1] * point.val[1])
		/ (param->y_fact * param->y_fact);
	point.val[2] = (point.val[2] * point.val[2])
		/ (param->z_fact * param->z_fact);
	if (point.val[0] + point.val[1] + point.val[2] > 1)
		return (0);
	return (1);
}

t_2vecf	get_text_coordinate_ellipsoid(t_3vecf inter_point,
	t_3vecf normal_inter, t_obj *ellipsoid)
{
	t_2vecf	text_coord;

	text_coord.val[1] = (1 - fmod((atan2(normal_inter.val[0],
		normal_inter.val[2])
		/ (2 * M_PI) + 0.5), 1));
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

t_3vecf	get_ellipsoid_origin(t_obj *ellipsoid, t_ellipsoid *ellipsoid_param,
	int sp_id)
{
	if (sp_id)
		return (move_3vecf(ellipsoid_param->origin, ellipsoid->motions, sp_id));
	return (ellipsoid_param->origin);
}

t_3vecf	get_normal_intersect_ellipsoid(t_3vecf inter_point,
	t_obj *ellipsoid, int sp_id)
{
	t_ellipsoid	*param;
	t_3vecf		normal_inter;
	t_3vecf		cst;
	t_3vecf		ellipsoid_origin;

	param = (t_ellipsoid *)ellipsoid->obj_param;
	ellipsoid_origin = get_ellipsoid_origin(ellipsoid, param, sp_id);
	cst = sub_3vecf(inter_point, ellipsoid_origin);
	normal_inter.val[0] = (2 * cst.val[0]) / (param->x_fact * param->x_fact);
	normal_inter.val[1] = (2 * cst.val[1]) / (param->y_fact * param->y_fact);
	normal_inter.val[2] = (2 * cst.val[2]) / (param->z_fact * param->z_fact);
	normalize_3vecf(&normal_inter);
	return (normal_inter);
}

t_3vecf	get_ellipsoid_quadratic_cst(t_ellipsoid *param,
	t_3vecf ellipsoid_origin, t_leq l)
{
	double		o[3];
	double		d[3];
	double		f[3];
	t_3vecf		cst;

	o[0] = l.orig.val[0] - ellipsoid_origin.val[0];
	o[1] = l.orig.val[1] - ellipsoid_origin.val[1];
	o[2] = l.orig.val[2] - ellipsoid_origin.val[2];
	d[0] = l.dir.val[0];
	d[1] = l.dir.val[1];
	d[2] = l.dir.val[2];
	f[0] = param->x_fact;
	f[1] = param->y_fact;
	f[2] = param->z_fact;
	cst.val[2] = (o[0] * o[0]) / (f[0] * f[0]) + (o[1] * o[1]) / (f[1] * f[1])
		+ (o[2] * o[2]) / (f[2] * f[2]) - 1;
	cst.val[1] = (2 * o[0] * d[0]) / (f[0] * f[0])
		+ (2 * o[1] * d[1]) / (f[1] * f[1]) + (2 * o[2] * d[2])
			/ (f[2] * f[2]);
	cst.val[0] = (d[0] * d[0]) / (f[0] * f[0]) + (d[1] * d[1]) / (f[1] * f[1])
		+ (d[2] * d[2]) / (f[2] * f[2]);
	return (cst);
}

int		ray_intersect_ellipsoid(t_leq l, t_obj *ellipsoid,
	t_dist dist, int sp_id)
{
	t_ellipsoid	*param;
	t_3vecf		cst;
	t_2vecf		roots;
	int			check;
	t_3vecf		ellipsoid_origin;

	check = 0;
	param = (t_ellipsoid *)ellipsoid->obj_param;
	ellipsoid_origin = get_ellipsoid_origin(ellipsoid, param, sp_id);
	cst = get_ellipsoid_quadratic_cst(param, ellipsoid_origin, l);
	roots = solve_quadratic(cst.val[0], cst.val[1], cst.val[2]);
	check |= is_closest_intersect(dist, roots.val[0]);
	check |= is_closest_intersect(dist, roots.val[1]);
	return (check);
}

void	assign_ellipsoid_function(t_obj *ellipsoid)
{
	ellipsoid->obj_type = OBJ_ELLIPSOID;
	ellipsoid->check_inside = &check_inside_ellipsoid;
	ellipsoid->ray_intersect = &ray_intersect_ellipsoid;
	ellipsoid->get_normal_inter = &get_normal_intersect_ellipsoid;
	ellipsoid->get_origin = &get_origin_ellipsoid;
	ellipsoid->move = &move_ellipsoid;
	ellipsoid->rotate = NULL;
	ellipsoid->get_text_coordinate = &get_text_coordinate_ellipsoid;
}

void	generate_new_ellipsoid(t_data *data)
{
	t_obj		*ellipsoid;
	t_ellipsoid	*param;
	t_3vecf		dir;

	dir = mult_3vecf_33matf(mult_3vecf_33matf(window_to_view(0, 0,
		data->size.val[0], data->size.val[1]), data->rot_mat[1]),
			data->rot_mat[0]);
	normalize_3vecf(&dir);
	if (!(ellipsoid = ft_memalloc(sizeof(t_obj))))
		return ;
	if (!(param = ft_memalloc(sizeof(t_ellipsoid))))
		return ;
	param->origin = add_3vecf(data->camera->origin, product_c3vecf(dir, 2));
	param->x_fact = get_random_number((time(NULL) * 0xcacacaca) << 16) * 2.5;
	param->y_fact = get_random_number((time(NULL) * 0xabcdef99) << 4) * 2.5;
	param->z_fact = get_random_number((time(NULL) * 0xff3672ff) << 3) * 2.5;
	ellipsoid->obj_param = param;
	assign_ellipsoid_function(ellipsoid);
	ellipsoid->text = generate_random_texture(ellipsoid);
	set_bump_own(ellipsoid);
	add_object(ellipsoid, data);
	data->new_obj = 1;
}
