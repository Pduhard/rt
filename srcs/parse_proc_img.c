/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_proc_img.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aplat <aplat@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/15 20:35:13 by aplat             #+#    #+#             */
/*   Updated: 2020/05/18 05:05:51 by aplat            ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

static SDL_Surface	*parse_sdl_image(char *name)
{
	SDL_Surface		*row;
	SDL_Surface		*image;

	if (!(row = IMG_Load(name)))
	{
		printf("SDL IMG load error: %s\n", SDL_GetError());
		return (NULL);
	}
	if (!(image = SDL_ConvertSurfaceFormat(row, SDL_PIXELFORMAT_RGBA8888, 0)))
	{
		SDL_FreeSurface(row);
		return (NULL);
	}
	SDL_FreeSurface(row);
	return (image);
}

void				*parse_img(char *name)
{
	SDL_Surface		*image;
	t_text_img		*param;
	unsigned int	pixels_nb;
	unsigned int	i;

	if (!(param = malloc(sizeof(t_text_img))))
		return (NULL);
	if (!(image = parse_sdl_image(name)))
	{
		free(param);
		return (NULL);
	}
	pixels_nb = image->w * image->h;
	if (!(param->pixels = malloc(sizeof(unsigned int) * pixels_nb)))
	{
		free(param);
		SDL_FreeSurface(image);
		return (NULL);
	}
	param->width = image->w;
	param->height = image->h;
	SDL_LockSurface(image);
	i = -1;
	while (++i < pixels_nb)
		param->pixels[i] = ((unsigned int *)image->pixels)[i];
	SDL_UnlockSurface(image);
	SDL_FreeSurface(image);
	return ((void *)param);
}

void				*parse_texture_img(char **line)
{
	t_text_img		*param;
	unsigned int	i;
	int				name_len;
	char			*image_name;

	i = 0;
	while (**line != '(' && **line)
		++(*line);
	if (**line != '(')
		return (NULL);
	*line += 1;
	if ((name_len = ft_strichr(*line, ')')) == -1)
		return (NULL);
	if (!(image_name = ft_strsub(*line, i, name_len)))
		return (NULL);
	goto_next_element(line);
	if (!(param = parse_img(image_name)))
		ft_strdel(&image_name);
	ft_strdel(&image_name);
	return ((void *)param);
}

static int			parse_color_transp(char **line, int i, t_4vecf *t)
{
	char	*s;

	s = *line;
	while (ft_isspace(s[i]))
		++i;
	if (s[i] != '(' || (i = parse_4vecf(s, i, t)) == -1)
		return (0);
	if (goto_next_element(line) != '>')
		return (0);
	return (1);
}

void				*parse_proc(char **line)
{
	int			cmp;
	int			ret;
	t_text_proc	*param;

	cmp = -1;
	ret = 1;
	if (!(param = ft_memalloc(sizeof(t_text_proc))))
		return (NULL);
	while (goto_next_element(line) != '>' && ++cmp < 3 && ret != 0)
	{
		if (ft_strncmp_case(*line, "color", 5))
		{
			free(param);
			return (NULL);
		}
		else if (!(ft_strncmp_case(*line, "color", 5)))
			ret = parse_color_transp(line, 5, &param->color[cmp]);
	}
	if (cmp == 3 || cmp == -1 || ret == 0)
	{
		free(param);
		return (NULL);
	}
	return ((void *)param);
}
