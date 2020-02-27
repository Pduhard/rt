#include "rt.h"

static t_mlx	*init_mlx(t_data *data)
{
	t_mlx		*mlx;

	if (!(mlx = malloc(sizeof(t_mlx))))
		return (NULL);
	mlx->mlx_ptr = mlx_init();
	data->mlx = mlx;
	open_info(data);
	mlx->win_ptr = mlx_new_window(mlx->mlx_ptr,
			data->size.val[0], data->size.val[1], data->scene_name);
	mlx->img_ptr = mlx_new_image(mlx->mlx_ptr,
			data->size.val[0], data->size.val[1]);
	mlx->img_str = (int *)mlx_get_data_addr(mlx->img_ptr,
			&(mlx->bpp), &(mlx->s_l), &(mlx->endian));
	return (mlx);
}

t_data			*init_data(char *file_name)
{
	t_data		*data;

	(void)file_name;
	if (!(data = (t_data *)malloc(sizeof(t_data))))
		return (NULL);
	ft_bzero(data, sizeof(t_data));
	if (NB_THREADS < 1 || NB_THREADS > 16)
	{
		error(ERRORTHREAD, NULL);
		return (NULL);
	}
	if (!(parse_rt_conf(file_name, data)))
		return (NULL); //free all
	if (data->size.val[0] < 400 || data->size.val[0] > 2560 || data->size.val[1] < 400 || data->size.val[1] > 1420)
	{
		error(ERRORSIZE, NULL);
		return (NULL);
	}
	if (!(data->mlx = init_mlx(data)))
	{
		free(data->objs);
		free(data);
		return (NULL);
	}
	if ((data->caustics_gi || data->indirect_gi) && !(create_photon_map(data)))
	{
		//free all
		return (NULL);
	}
	data->rot_mat[0] = init_rotation_matrix_x(degree_to_radian(data->camera->rotation.val[0]));
	data->rot_mat[1] = init_rotation_matrix_y(degree_to_radian(data->camera->rotation.val[1]));
	data->f = 1;
	data->hooks = 0;
	data->fps = 0;
	data->delta_time = 0;
	return (data);
}
