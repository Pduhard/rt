#include "rt.h"

void	*parse_img(char *name)
{
	SDL_Surface		*row;
	SDL_Surface		*image;
	t_text_img		*param;
	unsigned int	pixels_nb;
	unsigned int	i;

	if (!(param = malloc(sizeof(t_text_img))))
		return (NULL);
	if (!(row = IMG_Load(name)))
		return (NULL);
	if (!(image = SDL_ConvertSurfaceFormat(row, SDL_PIXELFORMAT_RGBA8888, 0)))
		return (NULL);
	pixels_nb = image->w * image->h;
	if (!(param->pixels = malloc(sizeof(unsigned int) * pixels_nb)))
		return (NULL);
	param->width = image->w;
	param->height = image->h;
	SDL_LockSurface(image);
	i = -1;
	while (++i < pixels_nb)
		param->pixels[i] = ((unsigned int *)image->pixels)[i];
	SDL_UnlockSurface(image);
	SDL_FreeSurface(row);
	SDL_FreeSurface(image);
	return ((void *)param);
}

void	*parse_texture_img(char **line)
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
	return ((void *)param);
}

void	*parse_proc(char **line)
{
	char		stripe;
	int			cmp;
	int			ret;
	t_text_proc	*param;

	stripe = 0;
	cmp = -1;
	ret = 1;
	if (!(param = ft_memalloc(sizeof(t_text_proc))))
		return (NULL);
	while (goto_next_element(line) != '>' && ++cmp < 3 && ret != 0)
	{
		if (ft_strncmp_case(*line, "color", 5))
			return (NULL);
		else if (!(ft_strncmp_case(*line, "color", 5)))
			ret = parse_color_transp(line, 5, &param->color[cmp]);
	}
	if (cmp == 3 || cmp == -1 || ret == 0)
		return (NULL);
	return ((void *)param);
}

int		parse_color_transp(char **line, int i, t_4vecf *t)
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
