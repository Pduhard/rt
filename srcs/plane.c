/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   plane.c                                          .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: aplat <aplat@student.le-101.fr>            +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2019/12/30 17:05:21 by pduhard-     #+#   ##    ##    #+#       */
/*   Updated: 2020/03/12 19:51:43 by pduhard-         ###   ########lyon.fr   */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "rt.h"

int		check_inside_plane(t_3vecf inter_point, t_obj *plane)
{
	t_plane *param;
	t_3vecf	inter_orig;

	param = (t_plane *)plane->obj_param;
	inter_orig = sub_3vecf(inter_point, param->origin);
	if (is_null_3vecf(inter_orig))
		return (1);
	if (is_null(dot_product_3vecf(inter_orig, param->normal)))
		return (1);
	return (0);
}

t_2vecf	get_text_coordinate_plane(t_3vecf inter_point, t_3vecf normal_inter, t_obj *plane)
{
	t_2vecf	text_coord;
	t_3vecf	u_axis;
	t_3vecf	v_axis;
	t_plane	*param;
	t_3vecf	origin_inter;

	param = (t_plane *)plane->obj_param;
	if (param->x2d_axis.val[0] * param->x2d_axis.val[0] + param->x2d_axis.val[1] * param->x2d_axis.val[1] + param->x2d_axis.val[2] * param->x2d_axis.val[2] == 0 || !is_null(dot_product_3vecf(param->x2d_axis, param->normal)))
	{
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
		normalize_3vecf(&u_axis);
		param->x2d_axis = u_axis;
		v_axis = product_3vecf(u_axis, normal_inter);
		normalize_3vecf(&v_axis);
	}
	else
	{
		v_axis = param->x2d_axis;
		normalize_3vecf(&v_axis);
		u_axis = product_3vecf(v_axis, normal_inter);
		normalize_3vecf(&u_axis);
	}
	origin_inter = sub_3vecf(inter_point, param->origin);
	//	printf("%f %f %f , %f %f %f, %f %f %f\n", param->normal.val[0], param->normal.val[1], param->normal.val[2], u_axis.val[0], u_axis.val[1], u_axis.val[2], v_axis.val[0], v_axis.val[1], v_axis.val[2]);
	text_coord.val[0] = dot_product_3vecf(origin_inter, u_axis) / 2;
	text_coord.val[1] = dot_product_3vecf(origin_inter, v_axis) / 2;
	return (text_coord);
}

void	move_plane(t_obj *plane, t_3vecf dir, double fact)
{
	t_plane	*param;
	t_cut	*cuts;

	param = (t_plane *)plane->obj_param;
	param->origin.val[0] += dir.val[0] * fact;
	param->origin.val[1] += dir.val[1] * fact;
	param->origin.val[2] += dir.val[2] * fact;
	cuts = plane->cuts;
	while (cuts)
	{
		if (cuts->move && cuts->cut_type != CUT_STATIC)
			cuts->move(cuts, dir, fact);
		cuts = cuts->next;
	}
}

void	rotate_plane(t_obj *plane, t_3vecf orig, t_33matf rot_mat[2])
{
	t_plane	*param;
	t_cut	*cuts;

	param = (t_plane *)plane->obj_param;
	param->origin = sub_3vecf(param->origin, orig);
	param->origin = mult_3vecf_33matf(param->origin, rot_mat[1]);
	param->origin = mult_3vecf_33matf(param->origin, rot_mat[0]);
	param->normal = mult_3vecf_33matf(param->normal, rot_mat[1]);
	param->normal = mult_3vecf_33matf(param->normal, rot_mat[0]);
	param->x2d_axis = mult_3vecf_33matf(param->x2d_axis, rot_mat[1]);
	param->x2d_axis = mult_3vecf_33matf(param->x2d_axis, rot_mat[0]);
	param->origin = add_3vecf(param->origin, orig);
	cuts = plane->cuts;
	while (cuts)
	{
		if (cuts->rotate && cuts->cut_type != CUT_STATIC)
			cuts->rotate(cuts, orig, rot_mat);
		cuts = cuts->next;
	}
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

int	ray_intersect_plane(t_leq l, t_obj *plane, t_dist dist, int sp_id)
{
	t_plane	*plane_param;
	double	div;
	double	inter_dist;
	t_3vecf	plane_origin;

	plane_param = (t_plane *)plane->obj_param;
	plane_origin = sp_id ? move_3vecf(plane_param->origin, plane->motions, sp_id) : plane_param->origin;
	div = dot_product_3vecf(l.dir, plane_param->normal);
	if (div == 0)//> -0.00000001 && div < 0.00000001)
		return (0);
	inter_dist = dot_product_3vecf(sub_3vecf(plane_origin, l.orig), plane_param->normal) / div;
	if (inter_dist < *(dist.dist) && inter_dist > dist.min_dist && inter_dist < dist.max_dist)
	{
		*(dist.dist) = inter_dist;
		return (1);
	}
	return (0);
}

void  assign_plane_function(t_obj *plane)
{
	plane->obj_type = OBJ_PLANE;
	plane->check_inside = &check_inside_plane;
	plane->ray_intersect = &ray_intersect_plane;
	plane->get_normal_inter = &get_normal_intersect_plane;
	plane->get_origin = &get_origin_plane;
	plane->move = &move_plane;
	plane->rotate = &rotate_plane;
	plane->get_text_coordinate = &get_text_coordinate_plane;
}

void	generate_new_plane(t_data *data)
{
	t_obj		*plane;
	t_plane	*param;
	t_3vecf		dir;

	dir = mult_3vecf_33matf(mult_3vecf_33matf(window_to_view(0, 0, data->size.val[0], data->size.val[1]), data->rot_mat[1]), data->rot_mat[0]);
	normalize_3vecf(&dir);
	if (!(plane = ft_memalloc(sizeof(t_obj))))
		return ;
	if (!(param = ft_memalloc(sizeof(t_plane))))
		return ;
	param->origin.val[0] = data->camera->origin.val[0] + dir.val[0] * 2;
	param->origin.val[1] = data->camera->origin.val[1] + dir.val[1] * 2;
	param->origin.val[2] = data->camera->origin.val[2] + dir.val[2] * 2;
	param->normal = assign_3vecf(get_random_number((time(NULL) * 0xcacacaca) << 16) - 0.5, get_random_number((time(NULL) * 0xfeabcdef) << 8) - 0.5, get_random_number((time(NULL) * 0x1056ffe) << 4) - 0.5);
	normalize_3vecf(&param->normal);
	plane->obj_param = param;

	//int		rd;

//	rd = time(NULL);
/*	while (!is_null(dot_product_3vecf(param->normal, param->x2d_axis)))
	{
		param->x2d_axis = assign_3vecf(get_random_number(rd * 0xcacacaca << 16) - 0.5, get_random_number(rd * 0xfeabcdef << 8) - 0.5, get_random_number(rd * 0x1056ffe << 4) - 0.5);
		rd *= time(NULL);
		normalize_3vecf(&param->x2d_axis);
		printf("asad\n");
	} */
	assign_plane_function(plane);
	plane->text = generate_random_texture(plane);
	set_bump_own(plane);
	add_object(plane, data);
	data->new_obj = 1;
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
