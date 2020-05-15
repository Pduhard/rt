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
	int			i;
	int			j;
	double	dist;
	t_3vecf	dir;

	data = (t_data *)param;
	i = (int)-data->size.val[0] / 2 + x;
	j = (int)-data->size.val[1] / 2 + y;
	if (button == 1)
	{
		if (data->selected_obj)
			data->selected_obj = NULL;
		else
		{
			dir = mult_3vecf_33matf(mult_3vecf_33matf(window_to_view(i, j,
				data->size.val[0], data->size.val[1]), data->rot_mat[1]), data->rot_mat[0]);
			data->selected_obj = ray_first_intersect((t_leq){data->camera->origin,
				dir}, (t_dist){&dist, BIAS, MAX_VIEW}, data->objs, 0);
		}
	}
	return (1);
}
