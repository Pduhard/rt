/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   plane.c                                          .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: aplat <aplat@student.le-101.fr>            +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2019/12/30 17:05:21 by pduhard-     #+#   ##    ##    #+#       */
/*   Updated: 2020/02/04 07:20:16 by pduhard-    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "rt.h"

t_2vecf	get_text_coordinate_plane(t_3vecf inter_point, t_3vecf normal_inter, t_obj *plane)
{
	t_2vecf	text_coord;
	t_3vecf	u_axis;
	t_3vecf	v_axis;
	t_plane	*param;
	t_3vecf	origin_inter;

	param = (t_plane *)plane->obj_param;
	if (normal_inter.val[0] != 0)
	{
		u_axis = assign_3vecf(0, 1, 1);
		u_axis.val[0] = (-normal_inter.val[1] - normal_inter.val[2]) / normal_inter.val[0];
	}
	else if (normal_inter.val[1] != 0)
	{
		u_axis = assign_3vecf(1, 0, 1);
		u_axis.val[1] = (-normal_inter.val[0] - normal_inter.val[2]) / normal_inter.val[1];
	}
	else if (normal_inter.val[2] != 0)
	{
		u_axis = assign_3vecf(1, 1, 0);
		u_axis.val[2] = (-normal_inter.val[0] - normal_inter.val[1]) / normal_inter.val[2];
	}
	else
	{
		//	ft_printf("plane normal == [0, 0, 0] => exit\n");
		//	exit(0);
	}
	normalize_3vecf(&u_axis);
	v_axis = product_3vecf(u_axis, normal_inter);
	origin_inter = sub_3vecf(inter_point, param->origin);
	//	printf("%f %f %f , %f %f %f, %f %f %f\n", param->normal.val[0], param->normal.val[1], param->normal.val[2], u_axis.val[0], u_axis.val[1], u_axis.val[2], v_axis.val[0], v_axis.val[1], v_axis.val[2]);
	text_coord.val[0] = dot_product_3vecf(origin_inter, u_axis) / 2;
	text_coord.val[1] = -dot_product_3vecf(origin_inter, v_axis) / 2;
	return (text_coord);
}

void	move_plane(t_obj *plane, t_3vecf dir, double fact)
{
	t_plane	*param;

	param = (t_plane *)plane->obj_param;
	param->origin.val[0] += dir.val[0] * fact;
	param->origin.val[1] += dir.val[1] * fact;
	param->origin.val[2] += dir.val[2] * fact;
}

t_3vecf	get_origin_plane(t_obj *plane)
{
	return (((t_plane *)plane->obj_param)->origin);
}

t_3vecf	get_normal_intersect_plane(t_3vecf inter_point, t_obj *plane, int sp_id)
{
	return (((t_plane *)plane->obj_param)->normal);
	(void)inter_point;
	(void)sp_id;
}

int	ray_intersect_plane(t_3vecf orig, t_3vecf dir, t_obj *plane, double *dist, double min_dist, double max_dist, int sp_id)
{
	t_plane	*plane_param;
	double	div;
	double	inter_dist;
	t_3vecf	plane_origin;

	plane_param = (t_plane *)plane->obj_param;
	plane_origin = sp_id ? move_3vecf(plane_param->origin, plane->motions, sp_id) : plane_param->origin;
	div = dot_product_3vecf(dir, plane_param->normal);
	if (div == 0)//> -0.00000001 && div < 0.00000001)
	{
		return (0);
		//		return (1);
	}
	inter_dist = dot_product_3vecf(sub_3vecf(plane_origin, orig), plane_param->normal) / div;
	if (inter_dist < *dist && inter_dist > min_dist && inter_dist < max_dist)
	{
		*dist = inter_dist;
		return (1);
	}
	return (0);
}

/*int		parse_plane(char *line, t_data *data)
  {
  int			i;
  t_obj		*plane;
  t_plane		*plane_param;

  if (!(plane = malloc(sizeof(t_obj))) || !(plane_param = malloc(sizeof(t_plane))))
  return (0);
  i = 5;
  while (ft_isspace(line[i]))
  ++i;
  if (line[i] != '(' || (i = parse_3vecf(line, i, &plane_param->origin)) == -1)
  {
  ft_printf("Syntax error: plane syntax: plane(origin)(normal)(color)(reflection)\n");
  return (0);
  }
  while (ft_isspace(line[i]))
  ++i;
  if (line[i] != '(' || (i = parse_3vecf(line, i, &plane_param->normal)) == -1)
  {
  ft_printf("Syntax error: plane syntax: plane(origin)(normal)(color)(reflection)\n");
  return (0);
  }
  while (ft_isspace(line[i]))
  ++i;
  if (line[i] != '(' || (i = parse_texture(line, i, plane)) == -1)
  {
  ft_printf("Syntax error: plane syntax: plane(origin)(normal)(color)(reflection)\n");
  return (0);
  }
  while (ft_isspace(line[i]))
  ++i;
  if (line[i] != '(' || (i = parse_double(line, i, &plane->reflection)) == -1)
  {
  ft_printf("Syntax error: plane syntax: plane(origin)(normal)(color)(reflection)\n");
  return (0);
  }
  while (ft_isspace(line[i]))
  ++i;
  if (line[i] != '(' || (i = parse_double(line, i, &plane->refraction)) == -1)
  {
  ft_printf("Syntax error: plane syntax: plane(origin)(normal)(color)(reflection)\n");
  return (0);
  }
  normalize_3vecf(&plane_param->normal);
//	printf("plane : %f %f %f && %f %f %f && %f %f %f\n", plane_param->origin.val[0], plane_param->origin.val[1], plane_param->origin.val[2], plane_param->normal.val[0], plane_param->normal.val[1], plane_param->normal.val[2] , plane->color.val[0], plane->color.val[1], plane->color.val[2]);
plane->obj_param = plane_param;
plane->obj_type = OBJ_PLANE;
plane->ray_intersect = &ray_intersect_plane;
plane->get_normal_inter = &get_normal_intersect_plane;
plane->get_text_coordinate = &get_text_coordinate_plane;
if (data->objs)
{
plane->next = data->objs;
}
else
plane->next = NULL;
data->objs = plane;
return (1);
}
*/
