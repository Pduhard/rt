/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   rect.c                                         .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: aplat <aplat@student.le-101.fr>            +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2019/12/30 16:52:54 by pduhard-     #+#   ##    ##    #+#       */
/*   Updated: 2020/02/20 19:13:51 by pduhard-         ###   ########lyon.fr   */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "rt.h"

int		check_inside_rect(t_3vecf point, t_obj *rect)
{
	t_rect	*param;

	param = (t_rect *)rect->obj_param;
	(void)param;
	(void)point;
/*	if (point.val[0] < param->x_range.val[0] || point.val[0] > param->x_range.val[1]
		|| point.val[1] < param->y_range.val[0] || point.val[1] > param->y_range.val[1]
			||point.val[2] < param->z_range.val[0] || point.val[2] > param->z_range.val[1])
*/		return (0);
	return (1);
}

t_2vecf	get_text_coordinate_rect(t_3vecf inter_point, t_3vecf normal_inter, t_obj *rect)
{
	t_2vecf	text_coord;

	text_coord.val[1] = (1 - fmod((atan2(normal_inter.val[0], normal_inter.val[2]) / (2 * M_PI) + 0.5), 1));
	text_coord.val[0] = (normal_inter.val[1] * 0.5 + 0.5);
	//printf("%f %f\n", text_coord.val[0], text_coord.val[1]);
	return (text_coord);
	(void)inter_point;
	(void)rect;
}

void	move_rect(t_obj *rect, t_3vecf dir, double fact)
{
	t_rect	*param;

	param = (t_rect *)rect->obj_param;
	param->origin.val[1] += dir.val[1] * fact;
	param->origin.val[2] += dir.val[2] * fact;
}

t_3vecf	get_origin_rect(t_obj *rect)
{
	return (((t_rect *)rect->obj_param)->origin);
}

t_3vecf	get_rect_side_normal(int i, t_rect *param)
{
	if (i == 0)
		return (param->x_axis);
	if (i == 1)
		return (param->y_axis);
	if (i == 2)
		return (param->z_axis);
	if (i == 3)
		return (assign_3vecf(-param->x_axis.val[0], -param->x_axis.val[1], -param->x_axis.val[2]));
	if (i == 4)
		return (assign_3vecf(-param->y_axis.val[0], -param->y_axis.val[1], -param->y_axis.val[2]));
	else // i == 5
		return (assign_3vecf(-param->z_axis.val[0], -param->z_axis.val[1], -param->z_axis.val[2]));
}

t_3vecf	get_normal_intersect_rect(t_3vecf inter_point, t_obj *rect, int sp_id)
{
	t_rect	*param;
	t_3vecf		sph_orig;

	param = (t_rect *)rect->obj_param;
	sph_orig = sp_id ? move_3vecf(param->origin, rect->motions, sp_id) : param->origin;
	return (assign_3vecf(0, -1, 0));
	return (sub_3vecf(inter_point, sph_orig));
}


int	ray_intersect_rect(t_3vecf orig, t_3vecf dir, t_obj *rect, double *dist, double min_dist, double max_dist, int sp_id)
{
	t_rect	*rect_param;
	double	div;
	double	inter_dist;
	t_3vecf	rect_origin;
	int		i;
	int		check;

	check = 0;
	i = 0;
	rect_param = (t_rect *)rect->obj_param;
	rect_origin = sp_id ? move_3vecf(rect_param->origin, rect->motions, sp_id) : rect_param->origin;
	while (i < 6)
	{
		t_3vecf	side_normal;

		side_normal = get_rect_side_normal(i, rect_param);
		t_3vecf	other_side[2];
		other_side[0] = get_rect_side_normal((i + 1) % 6, rect_param);
		other_side[1] = get_rect_side_normal((i + 2) % 6, rect_param);
		div = dot_product_3vecf(dir, side_normal);
		if (!is_null(div))
		{
			t_3vecf	side_angles[4];
			t_3vecf	side_origin;

			side_origin.val[0] = rect_origin.val[0] + side_normal.val[0];
			side_origin.val[1] = rect_origin.val[1] + side_normal.val[1];
			side_origin.val[2] = rect_origin.val[2] + side_normal.val[2];
			side_angles[0].val[0] = side_origin.val[0] + other_side[0].val[0] + other_side[1].val[0];
			side_angles[0].val[1] = side_origin.val[1] + other_side[0].val[1] + other_side[1].val[1];
			side_angles[0].val[2] = side_origin.val[2] + other_side[0].val[2] + other_side[1].val[2];

			side_angles[1].val[0] = side_origin.val[0] + other_side[0].val[0] - other_side[1].val[0];
			side_angles[1].val[1] = side_origin.val[1] + other_side[0].val[1] - other_side[1].val[1];
			side_angles[1].val[2] = side_origin.val[2] + other_side[0].val[2] - other_side[1].val[2];

			side_angles[2].val[0] = side_origin.val[0] - other_side[0].val[0] + other_side[1].val[0];
			side_angles[2].val[1] = side_origin.val[1] - other_side[0].val[1] + other_side[1].val[1];
			side_angles[2].val[2] = side_origin.val[2] - other_side[0].val[2] + other_side[1].val[2];

			inter_dist = dot_product_3vecf(sub_3vecf(side_origin, orig), side_normal) / div;
			t_3vecf	u_axis = sub_3vecf(side_angles[1], side_angles[0]);
			t_3vecf	v_axis = sub_3vecf(side_angles[2], side_angles[0]);
			t_3vecf	inter_point;
			inter_point.val[0] = orig.val[0] + dir.val[0] * inter_dist;
			inter_point.val[1] = orig.val[1] + dir.val[1] * inter_dist;
			inter_point.val[2] = orig.val[2] + dir.val[2] * inter_dist;
			if (dot_product_3vecf(sub_3vecf(inter_point, side_angles[0]), u_axis) > 0 && dot_product_3vecf(sub_3vecf(inter_point, side_angles[1]), u_axis) < 0)
				if (dot_product_3vecf(sub_3vecf(inter_point, side_angles[0]), v_axis) > 0 && dot_product_3vecf(sub_3vecf(inter_point, side_angles[2]), v_axis) < 0)
			if (inter_dist < *dist && inter_dist > min_dist && inter_dist < max_dist)
			{
				*dist = inter_dist;
				check++;
				if (check == 2)
					return (1);
			}		
		}
		i++;
	}
	return (check);
}
/*
int	ray_intersect_rect(t_3vecf orig, t_3vecf dir, t_obj *rect, double *dist, double min_dist, double max_dist, int sp_id)
{

	t_3vecf	dist_vec;
	double	a, b, c;
	double	delta;
	t_2vecf	hit_point;
	t_rect	*rect_param;
	int		check = 0;
	t_3vecf	sph_origin;

	rect_param = (t_rect *)rect->obj_param;
	sph_origin = sp_id ? move_3vecf(rect_param->origin, rect->motions, sp_id) : rect_param->origin;
	dist_vec = sub_3vecf(orig, sph_origin);
	a = dot_product_3vecf(dir, dir);
	b = 2.f * dot_product_3vecf(dist_vec, dir);
	c = dot_product_3vecf(dist_vec, dist_vec) - rect_param->radius * rect_param->radius;
	delta = b * b - 4.f * a * c;
	if (delta < 0)
		return (0);
	hit_point.val[0] = (-b + sqrtf(delta)) / (2 * a);
	hit_point.val[1] = (-b - sqrtf(delta)) / (2 * a);
	if (hit_point.val[0] < *dist && hit_point.val[0] > min_dist && hit_point.val[0] < max_dist)
	{
		check = 1;
		*dist = hit_point.val[0];
	}
	if (hit_point.val[1] < *dist && hit_point.val[1] > min_dist && hit_point.val[1] < max_dist)
	{
		check = 1;
		*dist = hit_point.val[1];
	}
	return (check);
}
*/
/*int		parse_rect(char *line, t_data *data)
  {
  int			i;
  t_obj		*rect;
  t_rect	*rect_param;

  if (!(rect = malloc(sizeof(t_obj))) || !(rect_param = malloc(sizeof(t_rect))))
  return (0);
  i = 6;
  while (ft_isspace(line[i]))
  ++i;
  if (line[i] != '(' || (i = parse_3vecf(line, i, &rect_param->origin)) == -1)
  {
  ft_printf("Syntax error: rect syntax: rect(origin)(radius)(color)(reflection)(refraction)\n");
  return (0);
  }
  while (ft_isspace(line[i]))
  ++i;
  if (line[i] != '(' || (i = parse_double(line, i, &rect_param->radius)) == -1)
  {
  ft_printf("Syntax error: rect syntax: rect(origin)(radius)(color)(reflection)(refraction)\n");
  return (0);
  }
  while (ft_isspace(line[i]))
  ++i;
  if (line[i] != '(' || (i = parse_texture(line, i, rect)) == -1)
  {
  ft_printf("Syntax error: rect syntax: rect(origin)(radius)(color)(reflection)(refraction)\n");
  return (0);
  }
  while (ft_isspace(line[i]))
  ++i;
  if (line[i] != '(' || (i = parse_double(line, i, &rect->reflection)) == -1)
  {
  ft_printf("Syntax error: rect syntax: rect(origin)(radius)(color)(reflection)(refraction)\n");
  return (0);
  }
  while (ft_isspace(line[i]))
  ++i;
  if (line[i] != '(' || (i = parse_double(line, i, &rect->refraction)) == -1)
  {
  ft_printf("Syntax error: rect syntax: rect(origin)(radius)(color)(reflection)(refraction)\n");
  return (0);
  }

//printf("rect : %f %f %f && %f && %f %f %f\n", rect_param->origin.val[0], rect_param->origin.val[1], rect_param->origin.val[2], rect_param->radius, rect->color.val[0], rect->color.val[1], rect->color.val[2]);
rect->obj_param = rect_param;
rect->obj_type = OBJ_rect;
rect->ray_intersect = &ray_intersect_rect;
rect->get_normal_inter = &get_normal_intersect_rect;
rect->get_text_coordinate = &get_text_coordinate_rect;
if (data->objs)
{
rect->next = data->objs;
}
else
rect->next = NULL;
data->objs = rect;
return (1);
}*/
