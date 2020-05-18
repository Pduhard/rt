/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   triangle_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aplat <aplat@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/18 05:50:04 by aplat             #+#    #+#             */
/*   Updated: 2020/05/18 05:52:09 by aplat            ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

int		check_inside_triangle(t_3vecf point, t_obj *triangle)
{
	return (0);
	(void)point;
	(void)triangle;
}

void	move_triangle(t_obj *triangle, t_3vecf dir, double fact)
{
	t_triangle	*param;
	t_cut		*cuts;

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
	t_cut		*cuts;

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

void	assign_triangle_function(t_obj *triangle)
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

void	get_random_corner(t_triangle *param)
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
		data->size.val[0], data->size.val[1]), data->rot_mat[1]),
			data->rot_mat[0]);
	normalize_3vecf(&dir);
	if (!(triangle = ft_memalloc(sizeof(t_obj)))
		|| !(param = ft_memalloc(sizeof(t_triangle))))
		return ;
	param->origin = add_3vecf(data->camera->origin, product_c3vecf(dir, 2));
	get_random_corner(param);
	param->origin.val[0] = (param->a.val[0] + param->b.val[0]
		+ param->c.val[0]) / 3.;
	param->origin.val[1] = (param->a.val[1] + param->b.val[1]
		+ param->c.val[1]) / 3;
	param->origin.val[2] = (param->a.val[2] + param->b.val[2]
		+ param->c.val[2]) / 3;
	triangle->obj_param = param;
	assign_triangle_function(triangle);
	triangle->text = generate_random_texture(triangle);
	set_bump_own(triangle);
	add_object(triangle, data);
	data->new_obj = 1;
}
