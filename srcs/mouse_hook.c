/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mouse_hook.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pduhard- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/28 05:17:07 by pduhard-          #+#    #+#             */
/*   Updated: 2020/02/28 05:59:05 by pduhard-         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

int		mouse_hook(int button, int x, int y, void *param)
{
	t_data	*data;
//	t_obj	*obj;
	data = (t_data *)param;
	int		i;
	int		j;

	i = (int)-data->size.val[0] / 2 + x;
	j = (int)-data->size.val[1] / 2 + y;
	printf("i, j %d %d\n", i, j);
	if (button == 1)
	{
		if (data->selected_obj)
			data->selected_obj = NULL;
		else
		{
			t_3vecf orig = data->camera->origin;
			t_3vecf	dir = mult_3vecf_33matf(mult_3vecf_33matf(window_to_view(i, j, data->size.val[0], data->size.val[1]), data->rot_mat[1]), data->rot_mat[0]);
			double	dist;
			data->selected_obj = ray_first_intersect(orig, dir, BIAS, MAX_VIEW, &dist, data->objs, 0, data);
		//	if (obj)
		//		printf("obj type i :%d %s\n", obj->obj_type, obj->obj_type == OBJ_PLANE ? "plane..." : "YESS");
		}
	}
	return (1);
}
/*
int		moov_hook(int x, int y, void *param)
{
	t_data	*data;

	data = (t_data *)param;
	data->mouse_x = x;
	if (x < 0)
		x = 0;
	else if (x > WIN_WIDTH)
		x = WIN_WIDTH;
	data->mouse_y = y;
	if (y < 0)
		y = 0;
	else if (y > WIN_HEIGHT)
		y = WIN_HEIGHT;
//	printf("%d %d\n", data->mouse_x, data->mouse_y);
	data->mouse_x = (double)x * 2 / (double)WIN_WIDTH - 1;
	data->mouse_y = 0;//(double)y * 2 / (double)WIN_HEIGHT - 1;
	printf("%d %d => %f %f\n",x, y,  data->mouse_x, data->mouse_y);

	return (1);
}*/
