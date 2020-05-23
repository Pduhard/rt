/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_cut_uv.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aplat <aplat@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/23 19:44:24 by aplat             #+#    #+#             */
/*   Updated: 2020/05/23 19:47:59 by aplat            ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

static int	check_cut_uv_param(t_cut_uv *param)
{
	if (param->u_range.val[0] == param->u_range.val[1] ||
		param->v_range.val[0] == param->v_range.val[1])
	{
		ft_fdprintf(2, "cut uv range must be greater than 0\n");
		return (0);
	}
	return (1);
}

static int	parse_cut_uv_element(char **line, t_cut_uv *param, int *ret)
{
	char	stripe;

	stripe = 0;
	*ret = 1;
	while (stripe != '>' && *ret != 0)
	{
		stripe = goto_next_element(line);
		if (!(ft_strncmp_case(*line, "u_range", 7)))
			*ret = parse_rotation(line, &param->u_range, 7);
		else if (!(ft_strncmp_case(*line, "v_range", 7)))
			*ret = parse_rotation(line, &param->v_range, 7);
		else if (stripe == '<' || *ret == 0)
			return (0);
	}
	return (*ret);
}

int			parse_cut_uv(char **line, t_cut *cut)
{
	t_cut_uv	*param;
	int			ret;

	cut->cut_type = CUT_UV;
	if (cut->cut_param)
		return (error(ALREADYCUT, NULL));
	if (!(param = ft_memalloc(sizeof(t_cut_uv))))
		return (0);
	if (!parse_cut_uv_element(line, param, &ret) ||
		!check_cut_uv_param(param))
	{
		free(param);
		return (syn_error(SERROR, SYNCUT, UVCUT, NULL));
	}
	cut->cut_param = param;
	return (ret);
}
