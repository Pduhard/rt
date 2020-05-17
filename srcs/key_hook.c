/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   key_hook.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aplat <aplat@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/15 19:17:03 by aplat             #+#    #+#             */
/*   Updated: 2020/05/15 19:21:34 by aplat            ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

int		key_press(int keycode, void *param)
{
	t_data *data;

	data = (t_data *)param;
	printf("button %d\n", keycode);
	if (check_mov_key_press(keycode, data) ||
			check_rot_key_press(keycode, data) ||
			check_gen_key_press(keycode, data) ||
			check_delete_key_press(keycode, data) ||
			check_pscreen_key_press(keycode, data) ||
			check_switch_key_press(keycode, data) ||
			check_esc_key_press(keycode, data))
		return (1);
	return (1);
}

int		key_release(int keycode, void *param)
{
	t_data *data;

	data = (t_data *)param;
	if (keycode == A_KEY && (data->hooks & A_HOOK))
		data->hooks -= A_HOOK;
	else if (keycode == S_KEY && (data->hooks & S_HOOK))
		data->hooks -= S_HOOK;
	else if (keycode == D_KEY && (data->hooks & D_HOOK))
		data->hooks -= D_HOOK;
	else if (keycode == W_KEY && (data->hooks & W_HOOK))
		data->hooks -= W_HOOK;
	else if (keycode == SPACE_KEY && (data->hooks & SPACE_HOOK))
		data->hooks -= SPACE_HOOK;
	else if (keycode == SHIFT_KEY && (data->hooks & SHIFT_HOOK))
		data->hooks -= SHIFT_HOOK;
	else if (keycode == ARR_LEFT_KEY && (data->hooks & ARR_LEFT_HOOK))
		data->hooks -= ARR_LEFT_HOOK;
	else if (keycode == ARR_RIGHT_KEY && (data->hooks & ARR_RIGHT_HOOK))
		data->hooks -= ARR_RIGHT_HOOK;
	else if (keycode == ARR_DOWN_KEY && (data->hooks & ARR_DOWN_HOOK))
		data->hooks -= ARR_DOWN_HOOK;
	else if (keycode == ARR_UP_KEY && (data->hooks & ARR_UP_HOOK))
		data->hooks -= ARR_UP_HOOK;
	return (1);
}
