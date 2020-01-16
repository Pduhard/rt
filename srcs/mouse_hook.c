/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   mouse_hook.c                                     .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: pduhard- <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2019/12/31 12:18:36 by pduhard-     #+#   ##    ##    #+#       */
/*   Updated: 2020/01/16 01:12:55 by pduhard-    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "rt.h"

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
}
