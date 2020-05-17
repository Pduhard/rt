/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_fermat.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aplat <aplat@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/15 20:27:55 by aplat             #+#    #+#             */
/*   Updated: 2020/05/17 17:44:01 by aplat            ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

static int	parse_fermat_element(char **line, int *ret, t_fermat *fermat_param)
{
	char	stripe;

	*ret = 1;
	stripe = 0;
	while (stripe != '>' && *ret != 0)
	{
		stripe = goto_next_element(line);
		if (!ft_strncmp_case(*line, "origin", 6))
			*ret = parse_origin(line, &fermat_param->origin, 6);
		else if (stripe == '<')
			return (0);
	}
	return (1);
}

int			parse_fermat(char **line, t_obj *fermat)
{
	int			ret;
	t_fermat	*fermat_param;

	if (fermat->obj_param)
		return (error(ALREADYOBJ, NULL));
	if (!(fermat_param = ft_memalloc(sizeof(t_fermat))))
		return (0);
	if (!parse_fermat_element(line, &ret, fermat_param) || ret == 0)
	{
		free(fermat_param);
		return (syn_error(SERROR, FERMAT, ORIGIN, NULL));
	}
	fermat->obj_param = fermat_param;
	assign_fermat_function(fermat);
	return (ret);
}
