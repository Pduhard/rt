/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loop_obj.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aplat <aplat@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/18 02:12:23 by aplat             #+#    #+#             */
/*   Updated: 2020/05/18 02:12:42 by aplat            ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

static void	init_obj_rot_mat(t_data *data, t_33matf rot_mat[2])
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

static void	obj_move(t_obj *obj, int way, t_3vecf dir)
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

void		manage_obj_move(t_data *data, int *ret)
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

void		manage_obj_rotation(t_data *data, int *ret)
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
