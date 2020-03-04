#include "rt.h"

int			close_cross(t_data *data)
{
	(void)data;
//	free_all(data);
	exit(0);
	return (0);
}

int			close_info(t_data *data)
{
	mlx_destroy_window(data->mlx->mlx_ptr, data->info->win_ptr);
	data->info = NULL;
	free(data->info);
	return (0);
}

int			main(int argc, char **argv)
{
	t_data	*data;

	if (argc != 2)
		return (error(ERRORARG, NULL));
	else
	{
		if (!(data = init_data(argv[1])))
			return (0);
	}
	mlx_hook(data->mlx->win_ptr, 2, 0, key_press, (void *)data);
	mlx_hook(data->mlx->win_ptr, 3, 0, key_release, (void *)data);
	mlx_hook(data->mlx->win_ptr, 4, 0, mouse_hook, (void *)data);
	mlx_hook(data->mlx->win_ptr, 17, (1L << 17), close_cross, (void *)data);
	if (data->info)
		mlx_hook(data->info->win_ptr, 17, 0, close_info, (void *)data);
	mlx_loop_hook(data->mlx->mlx_ptr, &print_loop_image, (void *)data);
	mlx_put_image_to_window(data->mlx->mlx_ptr,
			data->mlx->win_ptr, data->mlx->img_ptr, 0, 0);
	mlx_loop(data->mlx->mlx_ptr);
	return (0);
}
