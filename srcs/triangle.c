/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   triangle.c                                         .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: aplat <aplat@student.le-101.fr>            +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2019/12/30 16:52:54 by pduhard-     #+#   ##    ##    #+#       */
/*   Updated: 2020/02/26 07:29:41 by pduhard-         ###   ########lyon.fr   */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "rt.h"

int		check_inside_triangle(t_3vecf point, t_obj *triangle)
{
	return (0);
	(void)point;
	(void)triangle;
}

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
}

t_3vecf	get_origin_triangle(t_obj *triangle)
{
	//FALLLLLLLLSE
	return (((t_triangle *)triangle->obj_param)->a);
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

int	ray_intersect_triangle(t_3vecf orig, t_3vecf dir, t_obj *triangle, double *dist, double min_dist, double max_dist, int sp_id)
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
	dir_ac = product_3vecf(dir, ac);
	delta = dot_product_3vecf(ab, dir_ac);
	if (is_null(delta))
		return (0);
	inv_delta = 1. / delta;
	t_vec = sub_3vecf(orig, a);
	u = dot_product_3vecf(t_vec, dir_ac) * inv_delta;
	if (u < 0 || u > 1)
		return (0);
	q_vec = product_3vecf(t_vec, ab);
	v = dot_product_3vecf(dir, q_vec) * inv_delta;
	if (v < 0 || u + v > 1)
		return (0);
	tdist = dot_product_3vecf(ac, q_vec) * inv_delta;
	if (tdist < *dist && tdist > min_dist && tdist < max_dist)
	{
		*dist = tdist;
		return (1);
	}
	return (0);
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
