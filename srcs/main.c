/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   main.c                                           .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: aplat <aplat@student.le-101.fr>            +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2019/12/21 21:07:09 by pduhard-     #+#   ##    ##    #+#       */
/*   Updated: 2020/02/25 23:19:52 by aplat       ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "rt.h"

int		close_cross(t_data *data)
{
	(void)data;
//	free_all(data);
	exit(0);
	return (0);
}

int		close_cross_info(t_data *data)
{
	mlx_destroy_window(data->mlx->mlx_ptr, data->info->win_ptr);
	printf("closecross Infos\n");
	data->info = NULL;
	free (data->info);
	return (0);
}

int		main(int argc, char **argv)
{
	t_data	*data;
	if (argc != 2)
		return (return_update(ERRORARG, 0, 2));
	else
	{
		if (!(data = init_data(argv[1])))
			return (0);
	}
	render(data);
	open_info(data);
	mlx_hook(data->mlx->win_ptr, 2, 303, key_press, (void *)data);
	mlx_hook(data->mlx->win_ptr, 3, 303, key_release, (void *)data);
	mlx_loop_hook(data->mlx->mlx_ptr, &print_loop_image, (void *)data);
	mlx_hook(data->mlx->win_ptr, 17, (1L << 17), close_cross, (void *)data);
	if (data->info)
		mlx_hook(data->info->win_ptr, 17, (1L << 17), close_cross_info, (void *)data);
	mlx_put_image_to_window(data->mlx->mlx_ptr,
			data->mlx->win_ptr, data->mlx->img_ptr, 0, 0);
	mlx_loop(data->mlx->mlx_ptr);
	return (0);
}
