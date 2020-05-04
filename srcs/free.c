/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pduhard- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/05 16:55:10 by pduhard-          #+#    #+#             */
/*   Updated: 2020/03/12 17:44:59 by pduhard-         ###   ########lyon.fr   */
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
	free(obj);
}

int		rechain_list(t_obj **list, t_obj *obj)
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

void	delete_object(t_data *data, t_obj *obj)
{
	int		i;

	i = 0;
	if (!rechain_list(&data->objs, obj)
		&& !rechain_list(&data->negative_objs, obj))
		return ; //never happend
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

void unchain_list(t_data *data)
{
	t_data *start;

	start = data;
	while (data->next != start)
		data = data->next;
	data->next = NULL;
}

void free_all_objects(t_obj *objs)
{
	t_obj *next;
	int   i;

	while (objs)
	{
		next = objs->next;
		i = 0;
		if (objs->composed_w)
		{
			while (objs->composed_w[i])
				objs->composed_w[i++]->composed_w = NULL;
			free(objs->composed_w);
		}
		free_object(objs);
		objs = next;
	}
}

void free_data(t_data *data)
{
	free_all_objects(data->objs);

}

void free_all(t_data *data)
{
	t_data	*next;
	t_mlx		*mlx;

	mlx = data->mlx;
	unchain_list(data);
	while (data)
	{
		next = data->next;
		free_data(data);
		data = data->next;
	}
}
