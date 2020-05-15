/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_triangle.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aplat <aplat@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/15 20:38:40 by aplat             #+#    #+#             */
/*   Updated: 2020/05/15 20:48:44 by aplat            ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

static int	check_triangle_param(t_triangle *triangle_param, int ret)
{
	if (is_null_3vecf(sub_3vecf(triangle_param->a, triangle_param->b)) ||
		is_null_3vecf(sub_3vecf(triangle_param->b, triangle_param->c)) ||
		is_null_3vecf(sub_3vecf(triangle_param->c, triangle_param->a)))
		ft_fdprintf(2,
			"Parse error: Triangle: at least 2 corners must be different\n");
	else if (ret != 0)
		return (1);
	return (0);
}

static int	parse_triangle_element(char **line, int *ret,
	t_triangle *triangle_param)
{
	char stripe;

	*ret = 1;
	stripe = 0;
	while (stripe != '>' && *ret != 0)
	{
		stripe = goto_next_element(line);
		if (!ft_strncmp_case(*line, "a", 1))
			*ret = parse_origin(line, &triangle_param->a, 1);
		else if (!ft_strncmp_case(*line, "b", 1))
			*ret = parse_origin(line, &triangle_param->b, 1);
		else if (!ft_strncmp_case(*line, "c", 1))
			*ret = parse_origin(line, &triangle_param->c, 1);
		else if (stripe == '<')
			return (0);
	}
	return (1);
}

int			parse_triangle(char **line, t_obj *triangle)
{
	int			ret;
	t_triangle	*triangle_param;

	if (triangle->obj_param)
		return (error(ALREADYOBJ, NULL));
	if (!(triangle_param = ft_memalloc(sizeof(t_triangle))))
		return (0);
	if (!parse_triangle_element(line, &ret, triangle_param) ||
			!check_triangle_param(triangle_param, ret))
		return (syn_error(SERROR, TRIANGLE, TRIBC, NULL));
	triangle_param->origin = product_c3vecf(add_3vecf(triangle_param->a,
		add_3vecf(triangle_param->b, triangle_param->c)), 1. / 3.);
	triangle->obj_param = triangle_param;
	assign_triangle_function(triangle);
	return (ret);
}
