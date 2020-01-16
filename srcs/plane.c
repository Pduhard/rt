/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   plane.c                                          .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: pduhard- <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2019/12/30 17:05:21 by pduhard-     #+#   ##    ##    #+#       */
/*   Updated: 2020/01/16 02:26:27 by pduhard-    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "rt.h"

t_3vecf	get_normal_intersect_plane(t_3vecf inter_point, t_obj *plane)
{
	t_plane *param;

	param = (t_plane *)plane->obj_param;
//	if (dot_product_3vecf(inter_point, param->normal) > 0)
//		return (assign_3vecf(-param->normal.val[0], -param->normal.val[1], -param->normal.val[2]));
	return (param->normal);
	(void)inter_point;
}

int	ray_intersect_plane(t_3vecf orig, t_3vecf dir, t_obj *plane, double *dist, double min_dist, double max_dist)
{
	t_plane	*plane_param;
	double	div;
	double	inter_dist;

	plane_param = (t_plane *)plane->obj_param;
	div = dot_product_3vecf(dir, plane_param->normal);
	if (div == 0)//> -0.00000001 && div < 0.00000001)
	{
		div = 0.00000001;
//		return (1);
	}
	inter_dist = dot_product_3vecf(sub_3vecf(plane_param->origin, orig), plane_param->normal) / div;
	if (inter_dist < *dist && inter_dist > min_dist && inter_dist < max_dist)
	{
		*dist = inter_dist;
		return (1);
	}
	return (0);
}

int		parse_plane(char *line, t_data *data)
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
	if (line[i] != '(' || (i = parse_3vecf(line, i, &plane->color)) == -1)
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

	printf("plane : %f %f %f && %f %f %f && %f %f %f\n", plane_param->origin.val[0], plane_param->origin.val[1], plane_param->origin.val[2], plane_param->normal.val[0], plane_param->normal.val[1], plane_param->normal.val[2] , plane->color.val[0], plane->color.val[1], plane->color.val[2]);
	plane->obj_param = plane_param;
	plane->obj_type = OBJ_PLANE;
	plane->ray_intersect = &ray_intersect_plane;
	plane->get_normal_inter = &get_normal_intersect_plane;
	if (data->objs)
	{
		plane->next = data->objs;
	}
	else
		plane->next = NULL;
	data->objs = plane;
	return (1);
}
