#include "rt.h"

void	open_info(t_data *data)
{
	if (!data->info)
	{
		if (!(data->info = malloc(sizeof(t_mlx))))
			return ;
		data->info->win_ptr = mlx_new_window(data->mlx->mlx_ptr, 400, 500, "Informations");
		mlx_string_put(data->mlx->mlx_ptr, data->info->win_ptr, 150, 10, 0xFFFFFF, "Controls");
		mlx_string_put(data->mlx->mlx_ptr, data->info->win_ptr, 10, 60, 0xFFFFFF, "Cam Mode :");
		mlx_string_put(data->mlx->mlx_ptr, data->info->win_ptr, 5, 100, 0xFFFFFF, "ESC          ==> Quit Program");
		mlx_string_put(data->mlx->mlx_ptr, data->info->win_ptr, 5, 125, 0xFFFFFF, "W / S        ==> Forward / Backward");
		mlx_string_put(data->mlx->mlx_ptr, data->info->win_ptr, 5, 150, 0xFFFFFF, "A / D        ==> Left / Right");
		mlx_string_put(data->mlx->mlx_ptr, data->info->win_ptr, 5, 175, 0xFFFFFF, "UP / DOWN    ==> Rotate Verticaly");
		mlx_string_put(data->mlx->mlx_ptr, data->info->win_ptr, 5, 200, 0xFFFFFF, "LEFT / RIGHT ==> Rotate Horizontaly");
		mlx_string_put(data->mlx->mlx_ptr, data->info->win_ptr, 5, 225, 0xFFFFFF, "SPACE        ==> Up");
		mlx_string_put(data->mlx->mlx_ptr, data->info->win_ptr, 5, 250, 0xFFFFFF, "SHIFT        ==> Down");
		mlx_string_put(data->mlx->mlx_ptr, data->info->win_ptr, 5, 275, 0xFFFFFF, "LEFT CLICK   ==> Select Object");
		mlx_string_put(data->mlx->mlx_ptr, data->info->win_ptr, 10, 325, 0xFFFFFF, "Mode Move_Object :");
		mlx_string_put(data->mlx->mlx_ptr, data->info->win_ptr, 5, 375, 0xFFFFFF, "W, A, S, D   ==> Translate Object");
		mlx_string_put(data->mlx->mlx_ptr, data->info->win_ptr, 5, 400, 0xFFFFFF, "ARROWS       ==> Rotate Object");
		mlx_string_put(data->mlx->mlx_ptr, data->info->win_ptr, 5, 425, 0xFFFFFF, "SPACE        ==> Up Object");
		mlx_string_put(data->mlx->mlx_ptr, data->info->win_ptr, 5, 450, 0xFFFFFF, "SHIFT        ==> Down Object");
		mlx_string_put(data->mlx->mlx_ptr, data->info->win_ptr, 5, 475, 0xFFFFFF, "LEFT CLICK   ==> Unselect Object");
	}
}

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

t_data			*init_data(char *file_name, t_mlx *mlx)
{
	t_data		*data;

	(void)file_name;
	if (!(data = (t_data *)ft_memalloc(sizeof(t_data))))
		return (NULL);
	//ft_bzero(data, sizeof(t_data));
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
	if (!mlx && !(data->mlx = init_mlx(data)))
	{
		free(data->objs);
		free(data);
		return (NULL);
	}
	else if (mlx)
		data->mlx = mlx;
	if (!init_loading_screen(data)) 
	{
		free(data->objs);
		free(data->mlx);
		free(data);
		return (NULL);
	}
//	if ((data->caustics_gi || data->indirect_gi) && !(create_photon_map(data)))
//	{
		//free all
//		return (NULL);
//	}
	data->rot_mat[0] = init_rotation_matrix_x(degree_to_radian(data->camera->rotation.val[0]));
	data->rot_mat[1] = init_rotation_matrix_y(degree_to_radian(data->camera->rotation.val[1]));
	data->f = 1;
	data->hooks = 0;
	data->fps = 0;
	data->delta_time = 0;
	data->aa_adapt = MIN_ANTI_AL;
	return (data);
}
