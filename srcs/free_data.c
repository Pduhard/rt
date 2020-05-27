/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_data.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aplat <aplat@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/18 01:19:18 by aplat             #+#    #+#             */
/*   Updated: 2020/05/27 20:47:35 by aplat            ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

static void	free_all_objects(t_obj *objs)
{
	t_obj	*next;
	int		i;

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

static void	free_composed_objects(t_composed *composed)
{
	int			i;
	t_composed	*next;

	while (composed)
	{
		ft_strdel(&composed->name);
		i = 0;
		if (composed->components)
		{
			while (composed->components[i])
				free_object(composed->components[i++]);
			free(composed->components);
		}
		next = composed->next;
		free(composed);
		composed = next;
	}
}

static void	free_lights(t_light *lights)
{
	t_light *next;

	while (lights)
	{
		next = lights->next;
		free(lights);
		lights = next;
	}
}

static void	free_photon_map(t_kd_tree *map)
{
	if (!map)
		return ;
	free(map->photon);
	free_photon_map(map->left);
	free_photon_map(map->right);
	free(map);
}

void		free_data(t_data *data)
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
	free(data);
}
