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
/*
void	generate_new_cylinder(t_data *data)
{
	t_obj		*cylinder;
	t_cylinder		*param;
	t_3vecf		dir;

	dir = mult_3vecf_33matf(mult_3vecf_33matf(window_to_view(0, 0, data->size.val[0], data->size.val[1]), data->rot_mat[1]), data->rot_mat[0]);
	normalize_3vecf(&dir);
	if (!(cylinder = ft_memalloc(sizeof(t_obj))))
		return ;
	if (!(param = ft_memalloc(sizeof(t_cylinder))))
		return ;
	param->radius = get_random_number(time(NULL)) * 1.5;
	param->tip.val[0] = data->camera->origin.val[0] + dir.val[0] * 2;
	param->tip.val[1] = data->camera->origin.val[1] + dir.val[1] * 2;
	param->tip.val[2] = data->camera->origin.val[2] + dir.val[2] * 2;
	param->center = add_3vecf(assign_3vecf(0, 1, 0), param->tip);
	cylinder->obj_param = param;
	cylinder->obj_type = OBJ_CYLINDER;
	cylinder->check_inside = &check_inside_cylinder;
	cylinder->ray_intersect = &ray_intersect_cylinder;
	cylinder->get_normal_inter = &get_normal_intersect_cylinder;
	cylinder->get_origin = &get_origin_cylinder;
	cylinder->move = &move_cylinder;
	cylinder->rotate = &rotate_cylinder;
	cylinder->get_text_coordinate = &get_text_coordinate_cylinder;
	cylinder->get_text_color = &get_uni_color;
	cylinder->text = generate_random_texture();
	set_bump_own(cylinder);
	add_object(cylinder, data);
	data->new_obj = 1;
}
*/
t_2vecf	get_text_coordinate_triangle(t_3vecf inter_point, t_3vecf normal_inter, t_obj *triangle)
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
	param->a.val[0] += dir.val[0] * fact;
	param->a.val[1] += dir.val[1] * fact;
	param->a.val[2] += dir.val[2] * fact;
	param->b.val[0] += dir.val[0] * fact;
	param->b.val[1] += dir.val[1] * fact;
	param->b.val[2] += dir.val[2] * fact;
	param->c.val[0] += dir.val[0] * fact;
	param->c.val[1] += dir.val[1] * fact;
	param->c.val[2] += dir.val[2] * fact;
	param->origin.val[0] += dir.val[0] * fact;
	param->origin.val[1] += dir.val[1] * fact;
	param->origin.val[2] += dir.val[2] * fact;

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

t_3vecf	get_origin_triangle(t_obj *triangle)
{
	// t_triangle *param;
	// t_3vecf			origin;
	//
	// param = (t_triangle *)triangle->obj_param;
	// origin.val[0] = (param->a.val[0] + param->b.val[0] + param->c.val[0]) / 3;
	// origin.val[1] = (param->a.val[1] + param->b.val[1] + param->c.val[1]) / 3;
	// origin.val[2] = (param->a.val[2] + param->b.val[2] + param->c.val[2]) / 3;
	// return (origin);
	return (((t_triangle *)triangle->obj_param)->origin);
}

t_3vecf	get_normal_intersect_triangle(t_3vecf inter_point, t_obj *triangle, int sp_id)
{
	t_triangle	*param;
	t_3vecf		normal;

	param = (t_triangle *)triangle->obj_param;
	normal = product_3vecf(sub_3vecf(param->b, param->a), sub_3vecf(param->c, param->a));
	normalize_3vecf(&normal);
	return (normal);
	(void)inter_point;
	(void)sp_id;
}

int	ray_intersect_triangle(t_leq l, t_obj *triangle, t_dist dist, int sp_id)
{
	t_3vecf	ab;
	t_3vecf	ac;
	t_3vecf	dir_ac;
	t_3vecf	t_vec;
	t_3vecf	q_vec;
	double delta;
	double inv_delta;
	double u, v;
	double tdist;
	t_triangle	*param;

	t_3vecf	a, b, c;

	param = (t_triangle *)triangle->obj_param;
	a = sp_id ? move_3vecf(param->a, triangle->motions, sp_id) : param->a;
	b = sp_id ? move_3vecf(param->b, triangle->motions, sp_id) : param->b;
	c = sp_id ? move_3vecf(param->c, triangle->motions, sp_id) : param->c;
	ab = sub_3vecf(b, a);
	ac = sub_3vecf(c, a);
	dir_ac = product_3vecf(l.dir, ac);
	delta = dot_product_3vecf(ab, dir_ac);
	if (is_null(delta))
		return (0);
	inv_delta = 1. / delta;
	t_vec = sub_3vecf(l.orig, a);
	u = dot_product_3vecf(t_vec, dir_ac) * inv_delta;
	if (u < 0 || u > 1)
		return (0);
	q_vec = product_3vecf(t_vec, ab);
	v = dot_product_3vecf(l.dir, q_vec) * inv_delta;
	if (v < 0 || u + v > 1)
		return (0);
	tdist = dot_product_3vecf(ac, q_vec) * inv_delta;
	if (tdist < *(dist.dist) && tdist > dist.min_dist && tdist < dist.max_dist)
	{
		*(dist.dist) = tdist;
		return (1);
	}
	return (0);
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

void	generate_new_triangle(t_data *data)
{
	t_obj		*triangle;
	t_triangle	*param;
	t_3vecf		dir;

	dir = mult_3vecf_33matf(mult_3vecf_33matf(window_to_view(0, 0, data->size.val[0], data->size.val[1]), data->rot_mat[1]), data->rot_mat[0]);
	normalize_3vecf(&dir);
	if (!(triangle = ft_memalloc(sizeof(t_obj))))
		return ;
	if (!(param = ft_memalloc(sizeof(t_triangle))))
		return ;
//	param->radius = get_random_number(time(NULL)) * 2.5;
	param->origin.val[0] = data->camera->origin.val[0] + dir.val[0] * 2;
	param->origin.val[1] = data->camera->origin.val[1] + dir.val[1] * 2;
	param->origin.val[2] = data->camera->origin.val[2] + dir.val[2] * 2;
	dir = assign_3vecf(get_random_number((time(NULL) * 0xcacacaca) << 16) - 0.5, get_random_number((time(NULL) * 0xfeabcdef) << 8) - 0.5, get_random_number((time(NULL) * 0x1056ffe) << 12) - 0.5);
	normalize_3vecf(&dir);
	param->a = add_3vecf(dir, param->origin);
	dir = assign_3vecf(get_random_number((time(NULL) * 0xcacacaca) << 13) - 0.5, get_random_number((time(NULL) * 0xfeabcdef) << 9) - 0.5, get_random_number((time(NULL) * 0x1056ffe) << 3) - 0.5);
	normalize_3vecf(&dir);
	param->b = add_3vecf(dir, param->origin);
	dir = assign_3vecf(get_random_number((time(NULL) * 0xcacacaca) << 12) - 0.5, get_random_number((time(NULL) * 0xfeabcdef) << 6) - 0.5, get_random_number((time(NULL) * 0x1056ffe) << 0) - 0.5);
	normalize_3vecf(&dir);
	param->c = add_3vecf(dir, param->origin);
	param->origin.val[0] = (param->a.val[0] + param->b.val[0] + param->c.val[0]) / 3.;
	param->origin.val[1] = (param->a.val[1] + param->b.val[1] + param->c.val[1]) / 3;
	param->origin.val[2] = (param->a.val[2] + param->b.val[2] + param->c.val[2]) / 3;
	// param->origin.val[1] = data->camera->origin.val[1] + dir.val[1] * 2;
	// param->origin.val[2] = data->camera->origin.val[2] + dir.val[2] * 2;

	triangle->obj_param = param;
	assign_triangle_function(triangle);
	triangle->text = generate_random_texture(triangle);
	//triangle->get_bump_mapping = NULL;
	set_bump_own(triangle);
	//texture needed
	add_object(triangle, data);
	data->new_obj = 1;
}
/*int		parse_triangle(char *line, t_data *data)
  {
  int			i;
  t_obj		*triangle;
  t_triangle	*triangle_param;

  if (!(triangle = malloc(sizeof(t_obj))) || !(triangle_param = malloc(sizeof(t_triangle))))
  return (0);
  i = 6;
  while (ft_isspace(line[i]))
  ++i;
  if (line[i] != '(' || (i = parse_3vecf(line, i, &triangle_param->origin)) == -1)
  {
  ft_printf("Syntax error: triangle syntax: triangle(origin)(radius)(color)(reflection)(refraction)\n");
  return (0);
  }
  while (ft_isspace(line[i]))
  ++i;
  if (line[i] != '(' || (i = parse_double(line, i, &triangle_param->radius)) == -1)
  {
  ft_printf("Syntax error: triangle syntax: triangle(origin)(radius)(color)(reflection)(refraction)\n");
  return (0);
  }
  while (ft_isspace(line[i]))
  ++i;
  if (line[i] != '(' || (i = parse_texture(line, i, triangle)) == -1)
  {
  ft_printf("Syntax error: triangle syntax: triangle(origin)(radius)(color)(reflection)(refraction)\n");
  return (0);
  }
  while (ft_isspace(line[i]))
  ++i;
  if (line[i] != '(' || (i = parse_double(line, i, &triangle->reflection)) == -1)
  {
  ft_printf("Syntax error: triangle syntax: triangle(origin)(radius)(color)(reflection)(refraction)\n");
  return (0);
  }
  while (ft_isspace(line[i]))
  ++i;
  if (line[i] != '(' || (i = parse_double(line, i, &triangle->refraction)) == -1)
  {
  ft_printf("Syntax error: triangle syntax: triangle(origin)(radius)(color)(reflection)(refraction)\n");
  return (0);
  }

//printf("triangle : %f %f %f && %f && %f %f %f\n", triangle_param->origin.val[0], triangle_param->origin.val[1], triangle_param->origin.val[2], triangle_param->radius, triangle->color.val[0], triangle->color.val[1], triangle->color.val[2]);
triangle->obj_param = triangle_param;
triangle->obj_type = OBJ_triangle;
triangle->ray_intersect = &ray_intersect_triangle;
triangle->get_normal_inter = &get_normal_intersect_triangle;
triangle->get_text_coordinate = &get_text_coordinate_triangle;
if (data->objs)
{
triangle->next = data->objs;
}
else
triangle->next = NULL;
data->objs = triangle;
return (1);
}*/
