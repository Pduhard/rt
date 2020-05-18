/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loop.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aplat <aplat@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/30 20:56:52 by pduhard-          #+#    #+#             */
/*   Updated: 2020/05/18 16:10:11 by aplat            ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

static int		loop_manage(t_data *data)
{
	int	ret;

	ret = 0;
	if (data->selected_obj)
	{
		if (data->selected_obj->rotate)
			manage_obj_rotation(data, &ret);
		manage_obj_move(data, &ret);
	}
	else
	{
		manage_cam_rotation(data, &ret);
		manage_cam_move(data, &ret);
	}
	return (ret);
}

static void		launch_hook(t_data *data)
{
	mlx_hook(data->mlx->win_ptr, 2, (1L << 0), key_press, (void *)data);
	mlx_hook(data->mlx->win_ptr, 3, (1L << 1), key_release, (void *)data);
	mlx_hook(data->mlx->win_ptr, 4, (1L << 2), mouse_hook, (void *)data);
	mlx_hook(data->mlx->win_ptr, 17, (1L << 17), close_cross,
		(void *)data);
}

static t_data	*get_curr_scene(t_data **data_addr)
{
	t_data	*data;
	t_mlx	*mlx;

	if ((*data_addr)->to_next && (*data_addr)->next)
	{
		(*data_addr)->to_next = 0;
		data = (*data_addr)->next;
		data->hooks = (*data_addr)->hooks;
		*data_addr = data;
		data->new_obj = 1;
		data->aa_adapt = (QUALITY & (Q_VERY_LOW | Q_LOW)) ? MIN_AA : NO_AA;
		mlx = data->mlx;
		mlx_destroy_window(mlx->mlx_ptr, mlx->win_ptr);
		mlx_destroy_image(mlx->mlx_ptr, mlx->img_ptr);
		mlx->win_ptr = mlx_new_window(mlx->mlx_ptr, data->size.val[0],
			data->size.val[1], data->scene_name);
		mlx->img_ptr = mlx_new_image(mlx->mlx_ptr, data->size.val[0],
			data->size.val[1]);
		mlx->img_str = (int *)mlx_get_data_addr(mlx->img_ptr, &(mlx->bpp),
			&(mlx->s_l), &(mlx->endian));
		launch_hook(data);
	}
	return (*data_addr);
}

static int		check_for_scene_change(t_data *data)
{
	int rendering;

	rendering = 1;
	if (loop_manage(data) || data->new_obj || data->first_loop == 1)
	{
		data->aa_adapt = (QUALITY & (Q_VERY_LOW | Q_LOW)) ? MIN_AA : NO_AA;
	}
	else if (data->aa_adapt < NO_AA && QUALITY != Q_VERY_LOW)
		data->aa_adapt = NO_AA;
	else if (data->aa_adapt < MAX_AA && QUALITY == Q_HIGH)
		data->aa_adapt = MAX_AA;
	else
		rendering = data->first_loop == 2 && QUALITY == Q_VERY_LOW ? 1 : 0;
	return (rendering);
}

int				print_loop_image(void *param)
{
	t_data	*data;
	// int	rendering;

	// rendering = 1;
	data = get_curr_scene((t_data **)param);
	check_photon_map(data);
	if (check_for_scene_change(data) || (WATER_ON))
	{
		printf("%f\n", data->aa_adapt);
		render(data);
	}
//	loop_manage(data);
//	data->aa_adapt = NO_AA;
	data->new_obj = 0;
	data->first_loop++;
	if (WATER_ON)
		data->water_f += 0.02;
	// printf("%f\n", data->f);
	mlx_put_image_to_window(data->mlx->mlx_ptr,
		data->mlx->win_ptr, data->mlx->img_ptr, 0, 0);
	return (1);
}
