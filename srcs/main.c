/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   main.c                                           .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: pduhard- <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2019/12/21 21:07:09 by pduhard-     #+#   ##    ##    #+#       */
/*   Updated: 2020/01/31 07:38:35 by pduhard-    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "rt.h"

int		main(int argc, char **argv)
{
	t_data	*data;

	t_3vecf	t = solve_cubic(1, -3, 3, -1);
	printf("%lf %lf %lf\n", t.val[0], t.val[1], t.val[2]);
	printf("%lf\n", -54. * t.val[0] * t.val[0] * t.val[0] + 110. * t.val[0] * t.val[0] - 64. * t.val[0] + 8);
	//exit(0);
	//check error
	if (argc != 2)
	{
		if (!(data = init_data("conf/default.rt_conf")))
			return (0);
	}
	else
	{
		if (!(data = init_data(argv[1])))
			return (0);
	}
	render(data);
	mlx_hook(data->mlx->win_ptr, 2, 303, key_press, (void *)data);
	mlx_hook(data->mlx->win_ptr, 3, 303, key_release, (void *)data);
	mlx_loop_hook(data->mlx->mlx_ptr, &print_loop_image, (void *)data);

	mlx_put_image_to_window(data->mlx->mlx_ptr,
			data->mlx->win_ptr, data->mlx->img_ptr, 0, 0);
	mlx_loop(data->mlx->mlx_ptr);
	return (0);
}
