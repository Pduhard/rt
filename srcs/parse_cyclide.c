/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_cyclide.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aplat <aplat@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/15 20:23:43 by aplat             #+#    #+#             */
/*   Updated: 2020/05/17 17:41:40 by aplat            ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

static int	parse_cyclide_element(char **line, int *ret,
	t_cyclide *cyclide_param)
{
	char stripe;

	*ret = 1;
	stripe = 0;
	while (stripe != '>' && *ret != 0)
	{
		stripe = goto_next_element(line);
		if (!ft_strncmp_case(*line, "origin", 6))
			*ret = parse_origin(line, &cyclide_param->origin, 6);
		else if (!ft_strncmp_case(*line, "param", 5))
			*ret = parse_double(line, 5, &cyclide_param->param);
		else if (stripe == '<')
			return (0);
	}
	return (1);
}

int			parse_cyclide(char **line, t_obj *cyclide)
{
	int			ret;
	t_cyclide	*cyclide_param;

	if (cyclide->obj_param)
		return (error(ALREADYOBJ, NULL));
	if (!(cyclide_param = ft_memalloc(sizeof(t_cyclide))))
		return (0);
	if (!parse_cyclide_element(line, &ret, cyclide_param) || ret == 0)
	{
		free(cyclide_param);
		return (syn_error(SERROR, CYCLIDE, ORIGIN, PARAM));
	}
	cyclide->obj_param = cyclide_param;
	assign_cyclide_function(cyclide);
	return (ret);
}
