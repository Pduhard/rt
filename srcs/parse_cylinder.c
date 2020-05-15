/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_cylinder.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aplat <aplat@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/15 20:24:19 by aplat             #+#    #+#             */
/*   Updated: 2020/05/15 20:25:50 by aplat            ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

static int	check_cylinder_param(t_cylinder *cylinder_param, int ret)
{
	if (cylinder_param->radius <= 0. || is_null(cylinder_param->radius))
		ft_fdprintf(2,
		"Parse error: Cylinder: radius must be greater than 0\n");
	else if (is_null_3vecf(sub_3vecf(cylinder_param->center,
		cylinder_param->tip)))
		ft_fdprintf(2,
		"Parse error: Cylinder: center and tip must be different\n");
	else if (ret != 0)
		return (1);
	return (0);
}

static int	parse_cylinder_element(char **line, int *ret,
	t_cylinder *cylinder_param)
{
	char stripe;

	stripe = 0;
	*ret = 1;
	while (stripe != '>' && *ret != 0)
	{
		stripe = goto_next_element(line);
		if (!ft_strncmp_case(*line, "origin", 6))
			*ret = parse_origin(line, &cylinder_param->center, 6);
		else if (!ft_strncmp_case(*line, "tip", 3))
			*ret = parse_origin(line, &cylinder_param->tip, 3);
		else if (!ft_strncmp_case(*line, "radius", 6))
			*ret = parse_double(line, 6, &cylinder_param->radius);
		else if (stripe == '<')
			return (0);
	}
	return (1);
}

int			parse_cylinder(char **line, t_obj *cylinder)
{
	int			ret;
	t_cylinder	*cylinder_param;

	if (cylinder->obj_param)
		return (error(ALREADYOBJ, NULL));
	if (!(cylinder_param = ft_memalloc(sizeof(t_cylinder))))
		return (ft_memalloc_error(0, sizeof(t_cylinder)));
	if (!parse_cylinder_element(line, &ret, cylinder_param) ||
			!check_cylinder_param(cylinder_param, ret))
		return (syn_error(SERROR, CYLINDER, TIP, RADIUS));
	cylinder->obj_param = cylinder_param;
	assign_cylinder_function(cylinder);
	return (ret);
}
