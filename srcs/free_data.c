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
	int 		i;
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
	printf("photon map \n");
	if (!map)
		return ;
	free(map->photon);
	free_photon_map(map->left);
	free_photon_map(map->right);
	free(map);
}
void	free_data(t_data *data)
{
	printf("free data\n");
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
