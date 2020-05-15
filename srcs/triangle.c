/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   triangle.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pduhard- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/28 03:46:18 by pduhard-          #+#    #+#             */
/*   Updated: 2020/03/12 20:55:45 by pduhard-         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

int		check_inside_triangle(t_3vecf point, t_obj *triangle)
{
	return (0);
	(void)point;
	(void)triangle;
}

t_2vecf	get_text_coordinate_triangle(t_3vecf inter_point,
	t_3vecf normal_inter, t_obj *triangle)
{
	t_2vecf	text_coord;

	text_coord.val[1] = 0;
	text_coord.val[0] = 0;
	return (text_coord);
	(void)inter_point;
	(void)triangle;
	(void)normal_inter;
}

void	move_triangle(t_obj *triangle, t_3vecf dir, double fact)
{
	t_triangle	*param;
	t_cut	*cuts;

	param = (t_triangle *)triangle->obj_param;
	param->a = add_3vecf(param->a, product_c3vecf(dir, fact));
	param->b = add_3vecf(param->b, product_c3vecf(dir, fact));
	param->c = add_3vecf(param->c, product_c3vecf(dir, fact));
	param->origin = add_3vecf(param->origin, product_c3vecf(dir, fact));
	cuts = triangle->cuts;
	while (cuts)
	{
		if (cuts->move && cuts->cut_type != CUT_STATIC)
			cuts->move(cuts, dir, fact);
		cuts = cuts->next;
	}
}

void	rotate_triangle(t_obj *triangle, t_3vecf orig, t_33matf rot_mat[2])
{
	t_triangle	*param;
	t_cut	*cuts;

	param = (t_triangle *)triangle->obj_param;
	param->a = sub_3vecf(param->a, orig);
	param->b = sub_3vecf(param->b, orig);
	param->c = sub_3vecf(param->c, orig);
	param->a = mult_3vecf_33matf(param->a, rot_mat[1]);
	param->b = mult_3vecf_33matf(param->b, rot_mat[1]);
	param->c = mult_3vecf_33matf(param->c, rot_mat[1]);
	param->a = mult_3vecf_33matf(param->a, rot_mat[0]);
	param->b = mult_3vecf_33matf(param->b, rot_mat[0]);
	param->c = mult_3vecf_33matf(param->c, rot_mat[0]);
	param->a = add_3vecf(orig, param->a);
	param->b = add_3vecf(orig, param->b);
	param->c = add_3vecf(orig, param->c);
	cuts = triangle->cuts;
	while (cuts)
	{
		if (cuts->rotate && cuts->cut_type != CUT_STATIC)
			cuts->rotate(cuts, orig, rot_mat);
		cuts = cuts->next;
	}
}

t_3vecf	get_origin_triangle(t_obj *triangle) // a tej
{
	return (((t_triangle *)triangle->obj_param)->origin);
}

t_3vecf get_triangle_origin(t_obj *triangle,
	t_triangle *triangle_param, int sp_id)
{
	if (sp_id)
		return (move_3vecf(triangle_param->origin, triangle->motions, sp_id));
	return (triangle_param->origin);
}
t_3vecf	get_normal_intersect_triangle(t_3vecf inter_point,
	t_obj *triangle, int sp_id)
{
	t_triangle	*param;
	t_3vecf		normal;

	param = (t_triangle *)triangle->obj_param;
	normal = product_3vecf(sub_3vecf(param->b, param->a),
		sub_3vecf(param->c, param->a));
	normalize_3vecf(&normal);
	return (normal);
	(void)inter_point;
	(void)sp_id;
}

void  get_triangle_corner(t_3vecf corner[3], t_obj *triangle, int sp_id)
{
	t_triangle *param;

	param = (t_triangle *)triangle->obj_param;
	corner[0] = sp_id ? move_3vecf(param->a, triangle->motions, sp_id) : param->a;
	corner[1] = sp_id ? move_3vecf(param->b, triangle->motions, sp_id) : param->b;
	corner[2] = sp_id ? move_3vecf(param->c, triangle->motions, sp_id) : param->c;
}

double get_inverse_delta(t_3vecf a_vec, t_3vecf dir_ac)
{
	double delta;

	delta = dot_product_3vecf(a_vec, dir_ac);
	if (is_null(delta))
		return (0);
	return (1. / delta);
}

int check_triangle_dist(double u, double v, t_dist dist, double root)
{
	if (v < 0 || u + v > 1)
		return (0);
	if (root < *(dist.dist) && root > dist.min_dist && root < dist.max_dist)
	{
		*(dist.dist) = root;
		return (1);
	}
	return (0);
}

int	ray_intersect_triangle(t_leq l, t_obj *triangle, t_dist dist, int sp_id)
{
	t_3vecf	a_vec[2];
	t_3vecf corner[3];
	t_3vecf	dir_ac;
	t_3vecf tq_vec[2];
	double  tmp[3];

	get_triangle_corner(corner, triangle, sp_id);
	a_vec[0] = sub_3vecf(corner[1], corner[0]);
	a_vec[1] = sub_3vecf(corner[2], corner[0]);
	dir_ac = product_3vecf(l.dir, a_vec[1]);
	if (is_null((tmp[0] = get_inverse_delta(a_vec[0], dir_ac))))
		return (0);
	tq_vec[0] = sub_3vecf(l.orig, corner[0]);
	tmp[1] = dot_product_3vecf(tq_vec[0], dir_ac) * tmp[0];
	if (tmp[1] < 0 || tmp[1] > 1)
		return (0);
	tq_vec[1] = product_3vecf(tq_vec[0], a_vec[0]);
	tmp[2] = dot_product_3vecf(l.dir, tq_vec[1]) * tmp[0];
	return (check_triangle_dist(tmp[1], tmp[2], dist,
		dot_product_3vecf(a_vec[1], tq_vec[1]) * tmp[0]));
}

void  assign_triangle_function(t_obj *triangle)
{
	triangle->obj_type = OBJ_TRIANGLE;
	triangle->check_inside = &check_inside_triangle;
	triangle->ray_intersect = &ray_intersect_triangle;
	triangle->get_normal_inter = &get_normal_intersect_triangle;
	triangle->get_origin = &get_origin_triangle;
	triangle->move = &move_triangle;
	triangle->rotate = &rotate_triangle;
	triangle->get_text_coordinate = &get_text_coordinate_triangle;
}

void  get_random_corner(t_triangle *param)
{
	t_3vecf		dir;

	dir = assign_3vecf(
		get_random_number((time(NULL) * 0xcacacaca) << 16) - 0.5,
		get_random_number((time(NULL) * 0xfeacdef) << 8) - 0.5,
		get_random_number((time(NULL) * 0x1056ffe) << 12) - 0.5);
	normalize_3vecf(&dir);
	param->a = add_3vecf(dir, param->origin);
	dir = assign_3vecf(
		get_random_number((time(NULL) * 0xcacacaca) << 13) - 0.5,
		get_random_number((time(NULL) * 0xfeacdef) << 9) - 0.5,
		get_random_number((time(NULL) * 0x1056ffe) << 3) - 0.5);
	normalize_3vecf(&dir);
	param->b = add_3vecf(dir, param->origin);
	dir = assign_3vecf(
		get_random_number((time(NULL) * 0xcacacaca) << 12) - 0.5,
		get_random_number((time(NULL) * 0xfeacdef) << 6) - 0.5,
		get_random_number((time(NULL) * 0x1056ffe) << 0) - 0.5);
	normalize_3vecf(&dir);
	param->c = add_3vecf(dir, param->origin);
}

void	generate_new_triangle(t_data *data)
{
	t_obj		*triangle;
	t_triangle	*param;
	t_3vecf		dir;

	dir = mult_3vecf_33matf(mult_3vecf_33matf(window_to_view(0, 0,
		data->size.val[0], data->size.val[1]), data->rot_mat[1]), data->rot_mat[0]);
	normalize_3vecf(&dir);
	if (!(triangle = ft_memalloc(sizeof(t_obj))))
		return ;
	if (!(param = ft_memalloc(sizeof(t_triangle))))
		return ;
	param->origin = add_3vecf(data->camera->origin, product_c3vecf(dir, 2));
	get_random_corner(param);
	param->origin.val[0] = (param->a.val[0] + param->b.val[0] + param->c.val[0]) / 3.;
	param->origin.val[1] = (param->a.val[1] + param->b.val[1] + param->c.val[1]) / 3;
	param->origin.val[2] = (param->a.val[2] + param->b.val[2] + param->c.val[2]) / 3;
	triangle->obj_param = param;
	assign_triangle_function(triangle);
	triangle->text = generate_random_texture(triangle);
	set_bump_own(triangle);
	add_object(triangle, data);
	data->new_obj = 1;
}
