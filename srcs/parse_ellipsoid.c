/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_ellipsoid.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aplat <aplat@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/15 20:26:32 by aplat             #+#    #+#             */
/*   Updated: 2020/05/15 20:27:34 by aplat            ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

static int	parse_ellipsoid_element(char **line, int *ret,
	t_ellipsoid *ellipsoid_param)
{
	char stripe;

	*ret = 1;
	stripe = 0;
	while (stripe != '>' && *ret != 0)
	{
		stripe = goto_next_element(line);
		if (!ft_strncmp_case(*line, "origin", 6))
			*ret = parse_origin(line, &ellipsoid_param->origin, 6);
		else if (!ft_strncmp_case(*line, "x_axis", 6))
			*ret = parse_double(line, 6, &ellipsoid_param->x_fact);
		else if (!ft_strncmp_case(*line, "y_axis", 6))
			*ret = parse_double(line, 6, &ellipsoid_param->y_fact);
		else if (!ft_strncmp_case(*line, "z_axis", 6))
			*ret = parse_double(line, 6, &ellipsoid_param->z_fact);
		else if (stripe == '<')
			return (0);
	}
	return (1);
}

static int	check_ellipsoid_param(t_ellipsoid *ellipsoid_param, int ret)
{
	if (is_null(ellipsoid_param->x_fact) || ellipsoid_param->x_fact <= 0)
		ft_fdprintf(2,
		"Parse error: Ellipsoid: x axis factor must be greater than 0\n");
	else if (is_null(ellipsoid_param->y_fact) || ellipsoid_param->y_fact <= 0)
		ft_fdprintf(2,
		"Parse error: Ellipsoid: y axis factor must be greater than 0\n");
	else if (is_null(ellipsoid_param->z_fact) || ellipsoid_param->z_fact <= 0)
		ft_fdprintf(2,
		"Parse error: Ellipsoid: z axis factor must be greater than 0\n");
	else if (ret != 0)
		return (1);
	return (0);
}

int			parse_ellipsoid(char **line, t_obj *ellipsoid)
{
	int			ret;
	t_ellipsoid	*ellipsoid_param;

	if (ellipsoid->obj_param)
		return (error(ALREADYOBJ, NULL));
	if (!(ellipsoid_param = ft_memalloc(sizeof(t_ellipsoid))))
		return (0);
	if (!parse_ellipsoid_element(line, &ret, ellipsoid_param) ||
			!check_ellipsoid_param(ellipsoid_param, ret))
		return (syn_error(SERROR, ELLIPSE, AXIS, NULL));
	ellipsoid->obj_param = ellipsoid_param;
	assign_ellipsoid_function(ellipsoid);
	return (ret);
}
