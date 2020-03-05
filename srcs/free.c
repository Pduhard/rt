/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pduhard- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/05 16:55:10 by pduhard-          #+#    #+#             */
/*   Updated: 2020/03/05 19:34:33 by pduhard-         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

void	free_cuts(t_cut *cuts)
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

void	free_motions(t_motion *motions)
{
	t_motion	*next;

	while (motions)
	{
		next = motions->next;
		free(motions);
		motions = next;
	}
}

void	free_object(t_obj *obj)
{
	free(obj->obj_param);
	free_cuts(obj->cuts);
	free_motions(obj->motions);
	if (obj->text.text_type == TEXT_IMAGE)
		free(((t_text_img *)(obj->text.text_param))->pixels);
	if (obj->text.text_param)
		free(obj->text.text_param);
	//free composed with
	free(obj);
}

void	delete_object(t_data *data)
{
	t_obj	*prev;
	t_obj	*curr;
	int		i;

	i = 0;
	prev = data->objs;
	curr = data->objs;
	if (data->objs == data->selected_obj)
		data->objs = data->objs->next;
	else
	{
		while (prev && prev->next != data->selected_obj)
			prev = prev->next;
		prev->next = data->selected_obj->next;
	}
/*	if (data->selected_obj->composed_w)
	{
		while (data->selected_obj->composed_w[i])
		{
			if (data->selected_obj->composed_w[i] != data->selected_obj)
			{
				data->selected_obj->composed_w[i]->composed_w = NULL;
				curr = data->selected_obj;
				data->selected_obj = data->selected_obj->composed_w[i];
				delete_object(data);
				data->selected_obj = curr;
			}
			i++;
		}
		free(data->selected_obj->composed_w);
	}
*/	free_object(data->selected_obj);
	data->selected_obj = NULL;
	data->new_obj = 1;
}
