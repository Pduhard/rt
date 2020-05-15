/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sphere.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aplat <aplat@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/15 20:58:25 by aplat             #+#    #+#             */
/*   Updated: 2020/05/15 21:00:31 by aplat            ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

int		check_inside_sphere(t_3vecf point, t_obj *sphere)
{
	t_sphere	*param;

	param = (t_sphere *)sphere->obj_param;
	if (get_length_3vecf(sub_3vecf(point, param->origin)) > param->radius)
		return (0);
	return (1);
}

t_2vecf	get_text_coordinate_sphere(t_3vecf inter_point,
	t_3vecf normal_inter, t_obj *sphere)
{
	t_2vecf	text_coord;

	text_coord.val[1] = (1 - fmod((atan2(normal_inter.val[0],
		normal_inter.val[2]) / (2 * M_PI) + 0.5), 1));
	text_coord.val[0] = (normal_inter.val[1] * 0.5 + 0.5);
	return (text_coord);
	(void)inter_point;
	(void)sphere;
}

void	move_sphere(t_obj *sphere, t_3vecf dir, double fact)
{
	t_sphere	*param;
	t_cut		*cuts;

	param = (t_sphere *)sphere->obj_param;
	param->origin.val[0] += dir.val[0] * fact;
	param->origin.val[1] += dir.val[1] * fact;
	param->origin.val[2] += dir.val[2] * fact;
	cuts = sphere->cuts;
	while (cuts)
	{
		if (cuts->move && cuts->cut_type != CUT_STATIC)
			cuts->move(cuts, dir, fact);
		cuts = cuts->next;
	}
}

void	rotate_sphere(t_obj *sphere, t_3vecf orig, t_33matf rot_mat[2])
{
	t_sphere	*param;
	t_cut		*cuts;

	param = (t_sphere *)sphere->obj_param;
	param->origin = sub_3vecf(param->origin, orig);
	param->origin = mult_3vecf_33matf(param->origin, rot_mat[1]);
	param->origin = mult_3vecf_33matf(param->origin, rot_mat[0]);
	param->origin = add_3vecf(param->origin, orig);
	cuts = sphere->cuts;
	while (cuts)
	{
		if (cuts->rotate && cuts->cut_type != CUT_STATIC)
			cuts->rotate(cuts, orig, rot_mat);
		cuts = cuts->next;
	}
}

t_3vecf	get_origin_sphere(t_obj *sphere)
{
	return (((t_sphere *)sphere->obj_param)->origin);
}

t_3vecf	get_sphere_origin(t_obj *sphere, t_sphere *sphere_param, int sp_id)
{
	if (sp_id)
		return (move_3vecf(sphere_param->origin, sphere->motions, sp_id));
	return (sphere_param->origin);
}

t_3vecf	get_normal_intersect_sphere(t_3vecf inter_point,
	t_obj *sphere, int sp_id)
{
	t_sphere	*param;
	t_3vecf		sphere_origin;
	t_3vecf		normal;

	param = (t_sphere *)sphere->obj_param;
	sphere_origin = get_sphere_origin(sphere, param, sp_id);
	normal = sub_3vecf(inter_point, sphere_origin);
	normalize_3vecf(&normal);
	return (normal);
}

t_3vecf	get_sphere_quadratic_cst(t_sphere *sphere_param, t_3vecf sphere_origin,
	t_leq l)
{
	t_3vecf	dist_vec;
	t_3vecf cst;

	dist_vec = sub_3vecf(l.orig, sphere_origin);
	cst.val[0] = dot_product_3vecf(l.dir, l.dir);
	cst.val[1] = 2.f * dot_product_3vecf(dist_vec, l.dir);
	cst.val[2] = dot_product_3vecf(dist_vec, dist_vec)
		- sphere_param->radius * sphere_param->radius;
	return (cst);
}

int		ray_intersect_sphere(t_leq l, t_obj *sphere, t_dist dist, int sp_id)
{
	t_2vecf		roots;
	t_3vecf		cst;
	t_sphere	*sphere_param;
	int			check;
	t_3vecf		sphere_origin;

	check = 0;
	sphere_param = (t_sphere *)sphere->obj_param;
	sphere_origin = get_sphere_origin(sphere, sphere_param, sp_id);
	cst = get_sphere_quadratic_cst(sphere_param, sphere_origin, l);
	roots = solve_quadratic(cst.val[0], cst.val[1], cst.val[2]);
	check |= is_closest_intersect(dist, roots.val[0]);
	check |= is_closest_intersect(dist, roots.val[1]);
	return (check);
}

void	assign_sphere_function(t_obj *sphere)
{
	sphere->obj_type = OBJ_SPHERE;
	sphere->check_inside = &check_inside_sphere;
	sphere->ray_intersect = &ray_intersect_sphere;
	sphere->get_normal_inter = &get_normal_intersect_sphere;
	sphere->get_origin = &get_origin_sphere;
	sphere->move = &move_sphere;
	sphere->rotate = &rotate_sphere;
	sphere->get_text_coordinate = &get_text_coordinate_sphere;
}

void	generate_new_sphere(t_data *data)
{
	t_obj		*sphere;
	t_sphere	*param;
	t_3vecf		dir;

	dir = mult_3vecf_33matf(mult_3vecf_33matf(window_to_view(0, 0,
		data->size.val[0], data->size.val[1]), data->rot_mat[1]),
			data->rot_mat[0]);
	normalize_3vecf(&dir);
	if (!(sphere = ft_memalloc(sizeof(t_obj))))
		return ;
	if (!(param = ft_memalloc(sizeof(t_sphere))))
		return ;
	param->radius = get_random_number(time(NULL)) * 2.5;
	param->origin = add_3vecf(data->camera->origin,
		product_c3vecf(dir, param->radius * 2));
	sphere->obj_param = param;
	assign_sphere_function(sphere);
	sphere->text = generate_random_texture(sphere);
	set_bump_own(sphere);
	add_object(sphere, data);
	data->new_obj = 1;
}
