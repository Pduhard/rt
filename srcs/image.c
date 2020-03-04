/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   image.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pduhard- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/29 05:08:34 by pduhard-          #+#    #+#             */
/*   Updated: 2020/03/04 04:06:41 by pduhard-         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"
/*
int		get_color_at_pixel(double row, double col, )
{

}
*/
void	*parse_img(char *name)
{
	SDL_Surface		*image_row;
	SDL_Surface		*image;
	t_text_img		*param;
	unsigned int	pixels_nb;
	unsigned int	i;
//	int				name_len;
//	char			*image_name;

/*	i = 0;
	while (**line != '(' && **line)
		++(*line);
	if (**line != '(')
		return (NULL);
*//*	*line += 1;
	if ((name_len = ft_strichr(*line, ')')) == -1)
		return (NULL);
	if (!(image_name = ft_strsub(*line, i, name_len)))
		return (NULL);*/
	if (!(param = malloc(sizeof(t_text_img))))
		return (NULL);
	if (!(image_row = IMG_Load(name)))
		return (NULL);
	if (!(image = SDL_ConvertSurfaceFormat(image_row, SDL_PIXELFORMAT_RGBA8888, 0)))
		return (NULL);
	pixels_nb = image->w * image->h;
	if (!(param->pixels = malloc(sizeof(unsigned int) * pixels_nb)))
		return (NULL);
	param->width = image->w;
	param->height = image->h;
	SDL_LockSurface(image);
	i = 0;
	while (i < pixels_nb)
	{
		param->pixels[i] = ((unsigned int *)image->pixels)[i];
		++i;
	}
	SDL_UnlockSurface(image);
	SDL_FreeSurface(image_row);
	SDL_FreeSurface(image);
//	goto_next_element(line);
	return ((void *)param);
//	(void)line;
}

