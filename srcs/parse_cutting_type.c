/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_cutting_type.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aplat <aplat@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/18 04:57:07 by aplat             #+#    #+#             */
/*   Updated: 2020/05/19 18:45:16 by aplat            ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

static int	parse_cut_texture(char **line, t_cut *cut)
{
	char	stripe;

	cut->cut_type = CUT_TEXTURE;
	if (cut->cut_param)
		return (error(ALREADYCUT, NULL));
	stripe = goto_next_element(line);
	if (stripe != '>')
		return (error(CUTTEXTURE, NULL));
	cut->cut_param = NULL;
	return (1);
}

static int	parse_cut_sphere(char **line, t_cut *cut)
{
	t_sphere	*param;
	int			ret;

	ret = 1;
	cut->cut_type = CUT_SPHERE;
	if (cut->cut_param)
		return (error(ALREADYCUT, NULL));
	if (!(param = ft_memalloc(sizeof(t_sphere))))
		return (0);
	if (!parse_sphere_element(line, &ret, param) ||
			!check_sphere_param(param, ret))
	{
		free(param);
		return (syn_error(SERROR, SYNCUT, SPHERECUT, ""));
	}
	cut->cut_param = param;
	cut->move = &move_cut_sphere;
	return (ret);
}

int			pick_type_cutting(char **line, t_cut *cut, int *ret)
{
	if (!(ft_strncmp_case(*line, "static", 6)))
		*ret = parse_cut_static_real(line, cut, CUT_REAL);
	else if (!(ft_strncmp_case(*line, "real", 4)))
		*ret = parse_cut_static_real(line, cut, CUT_STATIC);
	else if (!(ft_strncmp_case(*line, "texture", 7)))
		*ret = parse_cut_texture(line, cut);
	else if (!(ft_strncmp_case(*line, "sphere", 6)))
		*ret = parse_cut_sphere(line, cut);
	else if (!(ft_strncmp_case(*line, "uv", 2)))
		*ret = parse_cut_uv(line, cut);
	return (1);
}
