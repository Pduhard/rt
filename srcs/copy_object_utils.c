/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   copy_object_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aplat <aplat@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/27 20:12:14 by aplat             #+#    #+#             */
/*   Updated: 2020/05/27 20:17:05 by aplat            ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

t_cut	*copy_cut(t_cut *src)
{
	t_cut	*cut;

	cut = ft_memcpy(ft_memalloc(sizeof(t_cut)), src, sizeof(t_cut));
	if (src->cut_type == CUT_STATIC || src->cut_type == CUT_REAL)
		cut->cut_param = ft_memcpy(ft_memalloc(sizeof(t_cut_classic)),
			src->cut_param, sizeof(t_cut_classic));
	else if (src->cut_type == CUT_SPHERE)
		cut->cut_param = ft_memcpy(ft_memalloc(sizeof(t_sphere)),
			src->cut_param, sizeof(t_sphere));
	else if (src->cut_type == CUT_TEXTURE)
		cut->cut_param = NULL;
	else if (src->cut_type == CUT_UV)
		cut->cut_param = ft_memcpy(ft_memalloc(sizeof(t_cut_uv)),
			src->cut_param, sizeof(t_cut_uv));
	cut->next = NULL;
	return (cut);
}
