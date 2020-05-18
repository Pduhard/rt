/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aplat <aplat@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/15 19:07:54 by aplat             #+#    #+#             */
/*   Updated: 2020/05/18 02:31:00 by aplat            ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

static t_mlx	*init_mlx(t_data *data)
{
	t_mlx		*mlx;

	if (!(mlx = malloc(sizeof(t_mlx))))
		return (NULL);
	mlx->mlx_ptr = mlx_init();
	data->mlx = mlx;
	if (!open_info(data))
	{
		free(mlx);
		return (NULL);
	}
	mlx->win_ptr = mlx_new_window(mlx->mlx_ptr,
			data->size.val[0], data->size.val[1], data->scene_name);
	mlx->img_ptr = mlx_new_image(mlx->mlx_ptr,
			data->size.val[0], data->size.val[1]);
	mlx->img_str = (int *)mlx_get_data_addr(mlx->img_ptr,
			&(mlx->bpp), &(mlx->s_l), &(mlx->endian));
	return (mlx);
}

int				check_macro(void)
{
	if (NB_THREADS < 1 || NB_THREADS > 16)
	{
		error(ERRORTHREAD, NULL);
		return (0);
	}
	return (1);
}

int				check_file_setup(t_data *data)
{
	if (data->size.val[0] < 400 || data->size.val[0] > 2560
		|| data->size.val[1] < 400 || data->size.val[1] > 1420)
	{
		error(ERRORSIZE, NULL);
		return (0);
	}
	return (1);
}

int				check_mlx(t_mlx *mlx, t_data *data)
{
	if (!mlx && !(data->mlx = init_mlx(data)))
		return (0);
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
	{
		printf("data next %p\n", data->next);
		free_all(data);
		return (NULL);
	}
	init_perlin(data);
	data->water_f = 0.;
	data->first_loop = 1;
	data->rot_mat[0] = init_rotation_matrix_x(
		degree_to_radian(data->camera->rotation.val[0]));
	data->rot_mat[1] = init_rotation_matrix_y(
		degree_to_radian(data->camera->rotation.val[1]));
	data->aa_adapt = (QUALITY & (Q_VERY_LOW | Q_LOW)) ? MIN_AA : NO_AA;
	return (data);
}
