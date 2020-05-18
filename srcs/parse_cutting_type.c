/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_cutting_type.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aplat <aplat@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/18 04:57:07 by aplat             #+#    #+#             */
/*   Updated: 2020/05/18 04:59:03 by aplat            ###   ########lyon.fr   */
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
	char		stripe;
	int			ret;

	stripe = 0;
	ret = 1;
	cut->cut_type = CUT_SPHERE;
	if (cut->cut_param)
		return (error(ALREADYCUT, NULL));
	if (!(param = ft_memalloc(sizeof(t_sphere))))
		return (0);
	while (stripe != '>' && ret != 0)
	{
		stripe = goto_next_element(line);
		if (!(ft_strncmp_case(*line, "origin", 6)))
			ret = parse_origin(line, &param->origin, 6);
		else if (!(ft_strncmp_case(*line, "radius", 6)))
			ret = parse_double(line, 6, &param->radius);
		else if (stripe == '<' || ret == 0)
		{
			free(param);
			return (syn_error(SERROR, SYNCUT, SPHERECUT, ""));
		}
	}
	cut->cut_param = param;
	cut->move = &move_cut_sphere;
	return (ret);
}

static int	parse_cut_uv(char **line, t_cut *cut)
{
	t_cut_uv	*param;
	char		stripe;
	int			ret;

	stripe = 0;
	ret = 1;
	cut->cut_type = CUT_UV;
	if (cut->cut_param)
		return (error(ALREADYCUT, NULL));
	if (!(param = ft_memalloc(sizeof(t_cut_uv))))
		return (0);
	while (stripe != '>' && ret != 0)
	{
		stripe = goto_next_element(line);
		if (!(ft_strncmp_case(*line, "u_range", 7)))
			ret = parse_rotation(line, &param->u_range, 7);
		else if (!(ft_strncmp_case(*line, "v_range", 7)))
			ret = parse_rotation(line, &param->v_range, 7);
		else if (stripe == '<' || ret == 0)
			return (syn_error(SERROR, SYNCUT, CUBECUT, ZRANGE));
	}
	cut->cut_param = param;
	return (ret);
}

static int	parse_cut_static_real(char **line, t_cut *cut,
	t_cut_type cut_type)
{
	char			stripe;
	int				ret;
	t_cut_classic	*param;

	stripe = 0;
	ret = 1;
	cut->cut_type = cut_type;
	if (cut->cut_param)
		return (error(ALREADYCUT, NULL));
	if (!(param = ft_memalloc(sizeof(t_cut_classic))))
		return (0);
	while (stripe != '>' && ret != 0)
	{
		stripe = goto_next_element(line);
		if (!(ft_strncmp_case(*line, "origin", 6)))
			ret = parse_origin(line, &param->origin, 6);
		else if (!(ft_strncmp_case(*line, "normal", 6)))
			ret = parse_origin(line, &param->normal, 6);
		else if (!(cut->cut_param) && stripe == '<')
			return (syn_error(SERROR, SYNCUT, STATICCUT, ""));
	}
	cut->cut_param = param;
	cut->move = &move_cut_plane;
	cut->rotate = &rotate_cut_plane;
	return (ret ? ret : syn_error(SERROR, SYNCUT, STATICCUT, ""));
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
