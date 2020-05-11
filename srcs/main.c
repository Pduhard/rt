#include "rt.h"

int			close_cross(t_data *data)
{
	free_all(data);
	exit(0);
	return (0);
}

int			close_info(t_data *data)
{
	free_info(data);
	return (0);
}

t_data *init_all_scene(char **argv)
{
	t_data		*data;
	t_data		*first;
	t_data		*data_lst;
	int				i;

 	i = 0;
	data_lst = NULL;
	first = NULL;
	while (argv[++i])
	{
		if ((data = init_data(argv[i], data_lst ? data_lst->mlx : NULL)))
		{
			if (!first && (data_lst = data))
				first = data;
			else if ((data_lst->next = data))
				data_lst = data_lst->next;
		}
	}
	if (!data_lst)
		return (NULL);
	data_lst->next = first;
	return (first);
}

int	is_closest_intersect(t_dist dist, double root)
{
	if (root < *(dist.dist) && root > dist.min_dist && root < dist.max_dist)
	{
		*(dist.dist) = root;
		return (1);
	}
	return (0);
}


int			main(int argc, char **argv)
{
	t_data		*data;

	if (argc < 2)
		return (error(ERRORARG, NULL));
	else if (!(data = init_all_scene(argv)))
		return (error(ERRORARG, NULL));

	mlx_hook(data->mlx->win_ptr, 2, (1L << 0), key_press, (void *)data);
	mlx_hook(data->mlx->win_ptr, 3, (1 << 1), key_release, (void *)data);
	mlx_hook(data->mlx->win_ptr, 4, 0, mouse_hook, (void *)data);
	mlx_hook(data->mlx->win_ptr, 17, (1L << 17), close_cross, (void *)data);
	if (data->info)
		mlx_hook(data->info->win_ptr, 17, 0, close_info, (void *)data);
	mlx_loop_hook(data->mlx->mlx_ptr, &print_loop_image, (void *)&data);
	mlx_loop(data->mlx->mlx_ptr);
	return (0);
}
