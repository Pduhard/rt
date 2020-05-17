/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_plane.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aplat <aplat@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/15 20:33:53 by aplat             #+#    #+#             */
/*   Updated: 2020/05/16 21:58:36 by aplat            ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

static int	check_plane_param(t_plane *plane_param, int ret)
{
	if (is_null(get_length_3vecf(plane_param->normal)))
		ft_fdprintf(2, "Parse error: Plane: normal vector must not be null\n");
	else if (!is_null(dot_product_3vecf(plane_param->x2d_axis,
		plane_param->normal)))
		ft_fdprintf(2,
		"Parse error: Plane: specified x2d axis vector must be orthogonal\n");
	else if (ret != 0)
		return (1);
	return (0);
}

static int	parse_plane_element(char **line, int *ret, t_plane *plane_param)
{
	char stripe;

	*ret = 1;
	stripe = 0;
	while (stripe != '>' && *ret != 0)
	{
		stripe = goto_next_element(line);
		if (!ft_strncmp_case(*line, "origin", 6))
			*ret = parse_origin(line, &plane_param->origin, 6);
		else if (!ft_strncmp_case(*line, "normal", 6))
			*ret = parse_origin(line, &plane_param->normal, 6);
		else if (!ft_strncmp_case(*line, "xaxis", 5))
			*ret = parse_origin(line, &plane_param->x2d_axis, 5);
		else if (stripe == '<')
			return (0);
	}
	return (1);
}

int			parse_plane(char **line, t_obj *plane)
{
	int		ret;
	t_plane	*plane_param;

	if (plane->obj_param)
		return (error(ALREADYOBJ, NULL));
	if (!(plane_param = ft_memalloc(sizeof(t_plane))))
		return (0);
	if (!parse_plane_element(line, &ret, plane_param) ||
			!check_plane_param(plane_param, ret))
	{
		free(plane_param);
		return (syn_error(SERROR, PLANE, NORMAL, XAXIS));
	}
	if (!is_null_3vecf(plane_param->x2d_axis))
		normalize_3vecf(&plane_param->x2d_axis);
	plane->obj_param = plane_param;
	assign_plane_function(plane);
	return (ret);
}
