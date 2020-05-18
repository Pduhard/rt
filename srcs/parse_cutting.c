/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_cutting.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aplat <aplat@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/15 20:22:28 by aplat             #+#    #+#             */
/*   Updated: 2020/05/18 04:56:41 by aplat            ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

static void	free_cut(t_cut *cut)
{
	if (cut->cut_param)
		free(cut->cut_param);
	free(cut);
}

int			parse_cutting(char **line, t_obj *obj)
{
	char	stripe;
	int		ret;
	t_cut	*cut;

	stripe = 0;
	ret = 1;
	if (!(cut = ft_memalloc(sizeof(t_cut))))
		return (0);
	while (stripe != '>' && ret != 0)
	{
		stripe = goto_next_element(line);
		if (**line != '>' && !pick_type_cutting(line, cut, &ret))
			return (0);
		else if (**line != '<' && **line != '>')
		{
			free_cut(cut);
			return (error(UNKNOWCUT, NULL));
		}
	}
	if (obj->cuts)
		cut->next = obj->cuts;
	else
		cut->next = NULL;
	obj->cuts = cut;
	return (ret);
}
