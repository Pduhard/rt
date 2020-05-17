/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_cone.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aplat <aplat@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/15 20:21:31 by aplat             #+#    #+#             */
/*   Updated: 2020/05/16 21:50:16 by aplat            ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

static int	parse_cone_element(char **line, int *ret, t_cone *cone_param)
{
	char stripe;

	*ret = 1;
	stripe = 0;
	while (stripe != '>' && *ret != 0)
	{
		stripe = goto_next_element(line);
		if (!ft_strncmp_case(*line, "origin", 6))
			*ret = parse_origin(line, &cone_param->center, 6);
		else if (!ft_strncmp_case(*line, "tip", 3))
			*ret = parse_origin(line, &cone_param->tip, 3);
		else if (!ft_strncmp_case(*line, "radius", 6))
			*ret = parse_double(line, 6, &cone_param->radius);
		else if (stripe == '<')
			return (0);
	}
	return (1);
}

static int	check_cone_param(t_cone *cone_param, int ret)
{
	if (cone_param->radius <= 0. || is_null(cone_param->radius))
		ft_fdprintf(2, "Parse error: Cone: radius must be greater than 0\n");
	else if (is_null_3vecf(sub_3vecf(cone_param->center, cone_param->tip)))
		ft_fdprintf(2,
		"Parse error: Cone: center and tip are confused(must be different)\n");
	else if (ret != 0)
		return (1);
	return (0);
}

int			parse_cone(char **line, t_obj *cone)
{
	int		ret;
	t_cone	*cone_param;

	if (cone->obj_param)
		return (error(ALREADYOBJ, NULL));
	if (!(cone_param = ft_memalloc(sizeof(t_cone))))
		return (0);
	if (!parse_cone_element(line, &ret, cone_param) ||
			!check_cone_param(cone_param, ret))
	{
		free(cone_param);
		return (syn_error(SERROR, CONE, TIP, RADIUS));
	}
	cone->obj_param = cone_param;
	assign_cone_function(cone);
	return (ret);
}
