/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   main.c                                           .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: pduhard- <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2019/12/21 21:07:09 by pduhard-     #+#   ##    ##    #+#       */
/*   Updated: 2020/01/24 20:24:19 by pduhard-    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "rt.h"

int		main(int argc, char **argv)
{
	t_data	*data;

//	SDL_Surface *image = SDL_LoadBMP("bleu_outremer.bmp");
//	image = SDL_ConvertSurfaceFormat(image, SDL_PIXELFORMAT_RGB888, 0);
//	SDL_LockSurface(image);
//	unsigned int	*pixels = (unsigned int *)image->pixels;
//	for (int i = 0 ; i < 1000; ++i)
//	{
//		printf("%u %u %u\n", pixels[i] >> 16 & 0xff, pixels[i] >> 8 & 0xff, pixels[i] & 0xff);
//	}
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
	//data->fractal_fct(data);
	mlx_hook(data->mlx->win_ptr, 2, 303, key_press, (void *)data);
	mlx_hook(data->mlx->win_ptr, 3, 303, key_release, (void *)data);
	//mlx_hook(data->mlx->win_ptr, 6, 303, &moov_hook, (void *)data);
	mlx_loop_hook(data->mlx->mlx_ptr, &print_loop_image, (void *)data);

	mlx_put_image_to_window(data->mlx->mlx_ptr,
			data->mlx->win_ptr, data->mlx->img_ptr, 0, 0);
	mlx_loop(data->mlx->mlx_ptr);
	(void)argc;
	return (0);
}
