/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_cut_static_real.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aplat <aplat@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/23 19:36:02 by aplat             #+#    #+#             */
/*   Updated: 2020/05/23 19:44:10 by aplat            ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

static int	parse_cut_static_real_element(char **line, t_cut *cut,
	t_cut_classic *param, int *ret)
{
	char stripe;

	*ret = 1;
	stripe = 0;
	while (stripe != '>' && *ret != 0)
	{
		stripe = goto_next_element(line);
		if (!(ft_strncmp_case(*line, "origin", 6)))
			*ret = parse_origin(line, &param->origin, 6);
		else if (!(ft_strncmp_case(*line, "normal", 6)))
			*ret = parse_origin(line, &param->normal, 6);
		else if (!(cut->cut_param) && stripe == '<')
			return (syn_error(SERROR, SYNCUT, STATICCUT, ""));
	}
	return (*ret);
}

static int	check_cut_static_real_param(t_cut_classic *param)
{
	if (is_null(get_length_3vecf(param->normal)))
		return (0);
	return (1);
}

int			parse_cut_static_real(char **line, t_cut *cut,
	t_cut_type cut_type)
{
	char			stripe;
	int				ret;
	t_cut_classic	*param;

	stripe = 0;
	cut->cut_type = cut_type;
	if (cut->cut_param)
		return (error(ALREADYCUT, NULL));
	if (!(param = ft_memalloc(sizeof(t_cut_classic))))
		return (0);
	if (!parse_cut_static_real_element(line, cut, param, &ret) ||
			!check_cut_static_real_param(param))
	{
		free(param);
		return (0);
	}
	cut->cut_param = param;
	cut->move = &move_cut_plane;
	cut->rotate = &rotate_cut_plane;
	return (ret ? ret : syn_error(SERROR, SYNCUT, STATICCUT, ""));
}
