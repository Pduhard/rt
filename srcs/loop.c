/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loop.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aplat <aplat@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/30 20:56:52 by pduhard-          #+#    #+#             */
/*   Updated: 2020/05/15 19:35:04 by aplat            ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

static void		init_obj_rot_mat(t_data *data, t_33matf rot_mat[2])
{
	int		deg_y;
	int		deg_x;
	t_3vecf	tm;

	deg_x = 0;
	deg_y = 0;
	if ((data->hooks & ARR_LEFT_HOOK))
		deg_y = -5;
	else if ((data->hooks & ARR_RIGHT_HOOK))
		deg_y = 5;
	if ((data->hooks & ARR_DOWN_HOOK))
		deg_x = 5;
	else if ((data->hooks & ARR_UP_HOOK))
		deg_x = -5;
	rot_mat[1] = init_rotation_matrix_y(degree_to_radian(deg_y));
	tm = mult_3vecf_33matf(assign_3vecf(1, 0, 0), data->rot_mat[1]);
	rot_mat[0] = init_rotation_matrix_vec(tm, degree_to_radian(deg_x));
}

void			manage_obj_rotation(t_data *data, int *ret)
{
	t_33matf	rot_mat[2];
	int			i;

	if (data->hooks & ((ARR_LEFT_HOOK | ARR_RIGHT_HOOK
		| ARR_DOWN_HOOK | ARR_UP_HOOK)))
		*ret = 1;
	init_obj_rot_mat(data, rot_mat);
	if (data->selected_obj->composed_w)
	{
		i = -1;
		while (data->selected_obj->composed_w[++i])
			data->selected_obj->composed_w[i]->rotate(
				data->selected_obj->composed_w[i],
				data->selected_obj->composed_w[i]->composed_origin,
				rot_mat);
	}
	else
		data->selected_obj->rotate(
			data->selected_obj,
			data->selected_obj->get_origin(data->selected_obj),
			rot_mat);
}

void			manage_cam_rotation(t_data *data, int *ret)
{
	double radian;

	if (data->hooks & (ARR_LEFT_HOOK | ARR_RIGHT_HOOK))
	{
		if (data->hooks & ARR_LEFT_HOOK)
			data->camera->rotation.val[1] -= 3;
		if (data->hooks & ARR_RIGHT_HOOK)
			data->camera->rotation.val[1] += 3;
		radian = degree_to_radian(data->camera->rotation.val[1]);
		data->rot_mat[1] = init_rotation_matrix_y(radian);
		*ret = 1;
	}
	if (data->hooks & (ARR_UP_HOOK | ARR_DOWN_HOOK))
	{
		if (data->hooks & ARR_UP_HOOK && data->camera->rotation.val[0] < 90)
			data->camera->rotation.val[0] += 3;
		if (data->hooks & ARR_DOWN_HOOK && data->camera->rotation.val[0] > -90)
			data->camera->rotation.val[0] -= 3;
		*ret = 1;
	}
}

void			obj_move(t_obj *obj, int way, t_3vecf dir)
{
	int i;

	i = -1;
	if (obj->composed_w)
	{
		while (obj->composed_w[++i])
		{
			obj->composed_w[i]->move(obj->composed_w[i], dir, way);
			if (way == 1)
				obj->composed_w[i]->composed_origin =
					add_3vecf(obj->composed_w[i]->composed_origin, dir);
			else
				obj->composed_w[i]->composed_origin =
					sub_3vecf(obj->composed_w[i]->composed_origin, dir);
		}
	}
	else
		obj->move(obj, dir, way);
}

void			manage_obj_move(t_data *data, int *ret)
{
	t_3vecf dir;

	if ((data->hooks & (W_HOOK | S_HOOK)) && (*ret = 1))
	{
		dir = mult_3vecf_33matf(assign_3vecf(0, 0, 0.2), data->rot_mat[1]);
		if (data->hooks & W_HOOK)
			obj_move(data->selected_obj, 1, dir);
		if (data->hooks & S_HOOK)
			obj_move(data->selected_obj, -1, dir);
	}
	if ((data->hooks & (A_HOOK | D_HOOK)) && (*ret = 1))
	{
		dir = mult_3vecf_33matf(assign_3vecf(0.2, 0, 0), data->rot_mat[1]);
		if (data->hooks & A_HOOK)
			obj_move(data->selected_obj, -1, dir);
		if (data->hooks & D_HOOK)
			obj_move(data->selected_obj, 1, dir);
	}
	if ((data->hooks & SPACE_HOOK) && (*ret = 1))
		obj_move(data->selected_obj, -1, assign_3vecf(0, 0.2, 0));
	if ((data->hooks & SHIFT_HOOK) && (*ret = 1))
		obj_move(data->selected_obj, 1, assign_3vecf(0, 0.2, 0));
}

void			manage_cam_move(t_data *data, int *ret)
{
	t_3vecf dir;

	if ((data->hooks & (W_HOOK | S_HOOK)) && (*ret = 1))
	{
		dir = mult_3vecf_33matf(assign_3vecf(0, 0, 0.2), data->rot_mat[1]);
		if ((data->hooks & W_HOOK))
			data->camera->origin = add_3vecf(data->camera->origin, dir);
		if ((data->hooks & S_HOOK))
			data->camera->origin = sub_3vecf(data->camera->origin, dir);
	}
	if ((data->hooks & (A_HOOK | D_HOOK)) && (*ret = 1))
	{
		dir = mult_3vecf_33matf(assign_3vecf(0.2, 0, 0), data->rot_mat[1]);
		if (data->hooks & A_HOOK)
			data->camera->origin = sub_3vecf(data->camera->origin, dir);
		if (data->hooks & D_HOOK)
			data->camera->origin = add_3vecf(data->camera->origin, dir);
	}
	if ((data->hooks & SPACE_HOOK) && (*ret = 1))
		data->camera->origin.val[1] -= 0.2;
	if ((data->hooks & SHIFT_HOOK) && (*ret = 1))
		data->camera->origin.val[1] += 0.2;
}

int				loop_manage_cam(t_data *data)
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

void			launch_hook(t_data *data)
{
	mlx_hook(data->mlx->win_ptr, 2, (1L << 0), key_press, (void *)data);
	mlx_hook(data->mlx->win_ptr, 3, (1L << 1), key_release, (void *)data);
	mlx_hook(data->mlx->win_ptr, 4, (1L << 2), mouse_hook, (void *)data);
	mlx_hook(data->mlx->win_ptr, 17, (1L << 17), close_cross,
		(void *)data);
}

t_data			*get_curr_scene(t_data **data_addr)
{
	t_data	*data;
	t_mlx	*mlx;

	if ((*data_addr)->to_next && (*data_addr)->next)
	{
		(*data_addr)->to_next = 0;
		data = (*data_addr)->next;
		data->hooks = (*data_addr)->hooks;
		*data_addr = data;
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

void			check_photon_map(t_data *data)
{
	if ((((data->caustics_gi && !data->caustic_map)
		|| (data->indirect_gi && !data->indirect_map)))
			&& data->first_loop > 1 && !(create_photon_map(data)))
	{
		ft_fdprintf(2, "Internal error: Global Illumination\n");
		free_all(data);
		exit(0);
	}
}

int				check_for_scene_change(t_data *data)
{
	int rendering;

	rendering = 1;
	if (loop_manage_cam(data) || data->new_obj || data->first_loop == 1)
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
	// int		rendering;

	// rendering = 1;
	data = get_curr_scene((t_data **)param);
	check_photon_map(data);
	if (check_for_scene_change(data) || (WATER_ON))
	{
		printf("%f\n", data->aa_adapt);
		render(data);

	}
//	loop_manage_cam(data);
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
