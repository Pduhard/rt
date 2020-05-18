/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   other_hook.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aplat <aplat@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/18 04:47:48 by aplat             #+#    #+#             */
/*   Updated: 2020/05/18 04:48:09 by aplat            ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

static int	check_gen_key_press2(int keycode, t_data *data)
{
	if (keycode == ALPHA_ZERO_KEY)
		generate_new_cyclide(data);
	else if (keycode == ALPHA_MINUS_KEY)
		generate_new_fermat(data);
	else if (keycode == ALPHA_PLUS_KEY)
		generate_new_moebius(data);
	else
		return (0);
	return (1);
}

int			check_gen_key_press(int keycode, t_data *data)
{
	if (keycode == ALPHA_ONE_KEY)
		generate_new_sphere(data);
	else if (keycode == ALPHA_TWO_KEY)
		generate_new_plane(data);
	else if (keycode == ALPHA_THREE_KEY)
		generate_new_cone(data);
	else if (keycode == ALPHA_FOUR_KEY)
		generate_new_cylinder(data);
	else if (keycode == ALPHA_FIVE_KEY)
		generate_new_triangle(data);
	else if (keycode == ALPHA_SIX_KEY)
		generate_new_ellipsoid(data);
	else if (keycode == ALPHA_SEVEN_KEY)
		generate_new_hyperboloid(data);
	else if (keycode == ALPHA_EIGHT_KEY)
		generate_new_horse_saddle(data);
	else if (keycode == ALPHA_NINE_KEY)
		generate_new_monkey_saddle(data);
	else
		return (check_gen_key_press2(keycode, data));
	return (1);
}

int			check_delete_key_press(int keycode, t_data *data)
{
	if ((keycode == SUPPR_KEY || keycode == DELETE_KEY)
		&& data->selected_obj
			&& data->selected_obj->obj_type != OBJ_SKYBOX)
	{
		delete_object(data, data->selected_obj);
		data->selected_obj = NULL;
		data->new_obj = 1;
		return (1);
	}
	return (0);
}

int			check_switch_key_press(int keycode, t_data *data)
{
	if (keycode == TAB_KEY)
	{
		data->to_next = 1;
		return (1);
	}
	return (0);
}

int			check_esc_key_press(int keycode, t_data *data)
{
	if (keycode == ESC_KEY)
	{
		free_all(data);
		exit(0);
	}
	return (0);
}
