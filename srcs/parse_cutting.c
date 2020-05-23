/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_cutting.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aplat <aplat@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/15 20:22:28 by aplat             #+#    #+#             */
/*   Updated: 2020/05/23 19:36:58 by aplat            ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

static void	free_cut(t_cut *cut)
{
	if (cut->cut_param)
		free(cut->cut_param);
	free(cut);
}

static int	parse_cutting_elements(char **line, t_cut *cut, int *ret)
{
	char	stripe;

	stripe = 0;
	while (stripe != '>' && *ret != 0)
	{
		stripe = goto_next_element(line);
		if (**line != '>' && !pick_type_cutting(line, cut, ret))
			return (0);
		else if (**line != '<' && **line != '>')
			return (0);
	}
	return (*ret);
}

static int	check_cuttings(t_cut *cut)
{
	if (!cut->cut_param && cut->cut_type != CUT_TEXTURE)
		return (0);
	return (1);
}

int			parse_cutting(char **line, t_obj *obj)
{
	int		ret;
	t_cut	*cut;

	ret = 1;
	if (!(cut = ft_memalloc(sizeof(t_cut))))
		return (0);
	if (!parse_cutting_elements(line, cut, &ret) ||
			!check_cuttings(cut))
	{
		free_cut(cut);
		return (error(UNKNOWCUT, NULL));
	}
	if (obj->cuts)
		cut->next = obj->cuts;
	else
		cut->next = NULL;
	obj->cuts = cut;
	return (ret);
}
