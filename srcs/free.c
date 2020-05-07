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

void unchain_data_list(t_data *data)
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
			{
				if (objs->composed_w[i] != objs)
					objs->composed_w[i]->composed_w = NULL;
				i++;
			}
			free(objs->composed_w);
		}
		free_object(objs);
		objs = next;
	}
}

void free_composed_objects(t_composed *composed)
{
	int i;

	while (composed)
	{
		ft_strdel(&composed->name);
		i = 0;
		while (composed->components[i])
			free_object(composed->components[i++]);
		composed = composed->next;
	}
}

void free_mlx(t_mlx *mlx)
{
	mlx_destroy_image(mlx->mlx_ptr, mlx->img_ptr);
	mlx_destroy_window(mlx->mlx_ptr, mlx->win_ptr);
	free(mlx);
}

void free_lights(t_light *lights)
{
	t_light *next;

	while (lights)
	{
		next = lights->next;
		free(lights);
		lights = next;
	}
}

void free_photon_map(t_kd_tree *map)
{
	if (!map)
		return ;
	free(map->photon);
	free_photon_map(map->left);
	free_photon_map(map->right);
}

void free_data(t_data *data)
{
	free_composed_objects(data->composed_objs);
	free(data->camera);
	free_all_objects(data->objs);
	free_all_objects(data->negative_objs);
	free_lights(data->lights);
	ft_strdel(&data->scene_name);
	ft_strdel(&data->skybox_name);
	free_photon_map(data->indirect_map);
	free_photon_map(data->caustic_map);
}

void free_info(t_data *data)
{
	t_data	*first;
	int    	check;
	t_mlx		*info;

	first = data;
	check = 0;
	info = data->info;
	while (data != first || !check)
	{
		check = 1;
		data->info = NULL;
		data = data->next;
	}
	// free()
	// mlx_destroy_window(data->mlx->mlx_ptr, data->info->win_ptr);
	// mlx_destroy_window(data->mlx->mlx_ptr, data->info->win_ptr);
	free(info);
	// mlx_destroy_image(data->mlx->mlx_ptr, data->info->img_ptr);
	// free(data->info);
}

void free_all(t_data *data)
{
	t_data	*next;
	t_mlx		*mlx;

	mlx = data->mlx;

	free_info(data);
	free_mlx(mlx);
	unchain_data_list(data);
	while (data)
	{
		next = data->next;
		free_data(data);
		data = data->next;
	}
}
