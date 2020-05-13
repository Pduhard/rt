#include "rt.h"


void  write_cmd_strings(void *mp, void *wp)
{
	mlx_string_put(mp, wp, 150, 10, 0xFFFFFF, "Controls");
	mlx_string_put(mp, wp, 10, 60, 0xFFFFFF, "Cam Mode :");
	mlx_string_put(mp, wp, 5, 100, 0xFFFFFF, "ESC          ==> Quit Program");
	mlx_string_put(mp, wp, 5, 125, 0xFFFFFF, "W / S        ==> Forward / Backward");
	mlx_string_put(mp, wp, 5, 150, 0xFFFFFF, "A / D        ==> Left / Right");
	mlx_string_put(mp, wp, 5, 175, 0xFFFFFF, "UP / DOWN    ==> Rotate Verticaly");
	mlx_string_put(mp, wp, 5, 200, 0xFFFFFF, "LEFT / RIGHT ==> Rotate Horizontaly");
	mlx_string_put(mp, wp, 5, 225, 0xFFFFFF, "SPACE        ==> Up");
	mlx_string_put(mp, wp, 5, 250, 0xFFFFFF, "SHIFT        ==> Down");
	mlx_string_put(mp, wp, 5, 275, 0xFFFFFF, "LEFT CLICK   ==> Select Object");
	mlx_string_put(mp, wp, 10, 325, 0xFFFFFF, "Mode Move_Object :");
	mlx_string_put(mp, wp, 5, 375, 0xFFFFFF, "W, A, S, D   ==> Translate Object");
	mlx_string_put(mp, wp, 5, 400, 0xFFFFFF, "ARROWS       ==> Rotate Object");
	mlx_string_put(mp, wp, 5, 425, 0xFFFFFF, "SPACE        ==> Up Object");
	mlx_string_put(mp, wp, 5, 450, 0xFFFFFF, "SHIFT        ==> Down Object");
	mlx_string_put(mp, wp, 5, 475, 0xFFFFFF, "LEFT CLICK   ==> Unselect Object");
}

void	open_info(t_data *data)
{
	if (!data->info)
	{
		if (!(data->info = malloc(sizeof(t_mlx))))
			return ;
		data->info->win_ptr = mlx_new_window(data->mlx->mlx_ptr, 400, 500, "Informations");
		write_cmd_strings(data->mlx->mlx_ptr, data->info->win_ptr);
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

int					check_macro(void)
{
	if (NB_THREADS < 1 || NB_THREADS > 16)
	{
		error(ERRORTHREAD, NULL);
		return (0);
	}
	return (1);
}

int					check_file_setup(t_data *data)
{
	if (data->size.val[0] < 400 || data->size.val[0] > 2560
		|| data->size.val[1] < 400 || data->size.val[1] > 1420)
	{
		error(ERRORSIZE, NULL);
		return (0);
	}
	return (1);
}

int					check_mlx(t_mlx *mlx, t_data *data)
{
	if (!mlx && !(data->mlx = init_mlx(data)))
	{
		free(data);
		return (0);
	}
	else if (mlx)
		data->mlx = mlx;
	return (1);
}

t_data			*init_data(char *file_name, t_mlx *mlx)
{
	t_data		*data;

	if (!(data = (t_data *)ft_memalloc(sizeof(t_data))))
		return (NULL);
	if (!check_macro() || !parse_rt_conf(file_name, data)
		|| !check_file_setup(data) || !check_mlx(mlx, data))
		return (NULL);
	init_perlin(data);
	data->water_f = 0.;
	data->rot_mat[0] = init_rotation_matrix_x(degree_to_radian(data->camera->rotation.val[0]));
	data->rot_mat[1] = init_rotation_matrix_y(degree_to_radian(data->camera->rotation.val[1]));
	data->aa_adapt = (QUALITY & (Q_VERY_LOW | Q_LOW)) ? MIN_AA : NO_AA;
	return (data);
}
