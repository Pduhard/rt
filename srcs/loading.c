/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loading.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pduhard- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/29 04:30:05 by pduhard-          #+#    #+#             */
/*   Updated: 2020/02/29 07:50:20 by pduhard-         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

void	update_loading_screen_gi(int pc, t_text_img *img, t_data *data)
{
//	double	pc;
//	double	div;
	int		img_y;
	int		img_x;
	int		x;
	int		y;

	//return ;
//	pc = 0;
//	div = 0;
//	if (ind_i > 0)
//	{
//		pc += ind_i;// / (double)NB_INDIRECT_PHOTON;
//		div += NB_INDIRECT_PHOTON;
//	}
//	if (caus_i > 0)
//	{
//		pc += caus_i;// / (double)NB_CAUSTIC_PHOTON;
//		div += NB_CAUSTIC_PHOTON;
//	}
//	if (div)
//		pc /= div;
//	img_x = (int)((double)img->width * pc);
//	printf("%d\n", pc);
//	exit(0);
	img_x = pc;
	img_y = 0;
	x = data->size.val[0] / 4 + pc;
	y = data->size.val[1] / 4;
	//printf("\n\n%f \n\n", pc);
	while (img_y < img->height)
	{
		//mlx_pixel_put(data->mlx->mlx_ptr, data->mlx->win_ptr, x, y, (img->pixels[img_y * img->width + img_x] & 0xffffff00) >> 8);
	//	mlx_pixel_put(data->mlx->mlx_ptr, data->mlx->win_ptr)
		data->mlx->img_str[y * (int)data->size.val[0] + x] = (img->pixels[img_y * img->width + img_x] & 0xffffff00) >> 8;
		y++;
		img_y++;
	}
	mlx_put_image_to_window(data->mlx->mlx_ptr,
		data->mlx->win_ptr, data->mlx->img_ptr, 0, 0);
}

int		init_loading_screen(t_data *data)
{
	int		x;
	int		y;
	int		img_x;
	int		img_y;
	t_text_img *img = parse_img("img/global_illu.png");

	x = data->size.val[0] / 4;
	img_x = 0;
	while (img_x < img->width)
	{
		y = data->size.val[1] / 4;
		img_y = 0;
		while (img_y < img->height)
		{
			data->mlx->img_str[y * (int)data->size.val[0] + x] = (img->pixels[img_y * img->width + img_x] & 0xffffff00) >> 8;
			y++;
			img_y++;
		}
		img_x++;
		x++;
	}
	//free img
	x = data->size.val[0] / 4;
	img_x = 0;
	
	img = parse_img("img/render.png");
	while (img_x < img->width)
	{
		y = data->size.val[1] / 2;
		img_y = 0;
		while (img_y < img->height)
		{
			data->mlx->img_str[y * (int)data->size.val[0] + x] = (img->pixels[img_y * img->width + img_x] & 0xffffff00) >> 8;
			y++;
			img_y++;
		}
		img_x++;
		x++;
	}
//	*data->load_mlx = *data->mlx;

	mlx_put_image_to_window(data->mlx->mlx_ptr,
		data->mlx->win_ptr, data->mlx->img_ptr, 0, 0);
	(void)data;
	return (1);
}
