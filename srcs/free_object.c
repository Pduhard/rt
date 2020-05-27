/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_object.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aplat <aplat@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/18 01:21:22 by aplat             #+#    #+#             */
/*   Updated: 2020/05/27 20:47:23 by aplat            ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

static int	rechain_list(t_obj **list, t_obj *obj)
{
	t_obj	*prev;

	prev = *list;
	if (*list == obj)
		*list = obj->next;
	else
	{
		while (prev && prev->next != obj)
			prev = prev->next;
		if (!prev)
			return (0);
		prev->next = obj->next;
	}
	return (1);
}

void		free_cuts(t_cut *cuts)
{
	t_cut		*next;

	while (cuts)
	{
		next = cuts->next;
		if (cuts->cut_param)
			free(cuts->cut_param);
		free(cuts);
		cuts = next;
	}
}

void		free_motions(t_motion *motions)
{
	t_motion	*next;

	while (motions)
	{
		next = motions->next;
		free(motions);
		motions = next;
	}
}

void		free_object(t_obj *obj)
{
	if (obj->obj_param)
		free(obj->obj_param);
	free_cuts(obj->cuts);
	free_motions(obj->motions);
	if (obj->text.text_param)
	{
		if (obj->text.text_type == TEXT_IMAGE)
			free(((t_text_img *)(obj->text.text_param))->pixels);
		free(obj->text.text_param);
	}
	free(obj);
}

void		delete_object(t_data *data, t_obj *obj)
{
	int		i;

	i = 0;
	if (!rechain_list(&data->objs, obj)
		&& !rechain_list(&data->negative_objs, obj))
		return ;
	if (obj->composed_w)
	{
		while (obj->composed_w[i])
		{
			if (obj->composed_w[i] != obj)
			{
				obj->composed_w[i]->composed_w = NULL;
				delete_object(data, obj->composed_w[i]);
			}
			i++;
		}
		free(obj->composed_w);
	}
	free_object(obj);
}
