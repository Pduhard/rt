#include "rt.h"

int		parse_texture2(char **line, t_obj *obj) //
{
	char	stripe;
	int		ret;

	stripe = 0;
	ret = 1;
	if (obj->text.text_param)
		return (error(ALREADYTEXTURE, NULL));
	while (stripe != '>' && ret != 0)
	{
		stripe = goto_next_element(line);
		if (!(ft_strncmp_case(*line, "UNI", 3)))
		{
			obj->get_text_color = &get_uni_color;
			obj->text.text_type = TEXT_UNI;
			ret = obj->text.text_param ? 0 : 1;
		}
		else if (!(ft_strncmp_case(*line, "GRID", 4)))
		{
			obj->get_text_color = &get_grid_color;
			obj->text.text_type = TEXT_GRID;
			ret = obj->text.text_param ? 0 : 1;
		}
		else if (!(ft_strncmp_case(*line, "PERLIN", 6)))
		{
			obj->get_text_color = &get_perlin_color;
			obj->text.text_type = TEXT_PERLIN;
			ret = obj->text.text_param ? 0 : 1;
		}
		else if (!(ft_strncmp_case(*line, "MARBLE", 6)))
		{
			obj->get_text_color = &get_marble_color;
			obj->text.text_type = TEXT_MARBLE;
			ret = obj->text.text_param ? 0 : 1;
		}
		else if (!(ft_strncmp_case(*line, "WOOD", 4)))
		{
			obj->get_text_color = &get_wood_color;
			obj->text.text_type = TEXT_WOOD;
			ret = obj->text.text_param ? 0 : 1;
		}
		else if (!(ft_strncmp_case(*line, "FBM", 3)))
		{
			obj->get_text_color = &get_fbm_color;
			obj->text.text_type = TEXT_FBM;
			ret = obj->text.text_param ? 0 : 1;
		}
		else if (!(ft_strncmp_case(*line, "IMAGE", 5)))
		{
			obj->get_text_color = &get_image_color;
			obj->text.text_type = TEXT_IMAGE;
			ret = obj->text.text_param ? 0 : 1;
		}
		else if (!(ft_strncmp_case(*line, "offset", 6)))
			ret = parse_rotation(line, &obj->text.offset, 6);
		else if (!(ft_strncmp_case(*line, "scale", 5)))
			ret = parse_rotation(line, &obj->text.scale, 5);
		else if (!(ft_strncmp_case(*line, "BumpMapping", 11)) && obj->text.text_param)
			ret = parse_bump_mapping(line, obj);
		else if (**line != '<')
			return (error(UNKNOWTEXT, NULL));
		if (obj->text.text_type == TEXT_IMAGE && !(obj->text.text_param))
		{
			if (!(obj->text.text_param = parse_img(line/*, &obj->text*/)))
				return (syn_error(SERROR, TEXT, IMG, OFFSCALE));
		}
		else if (!(obj->text.text_param))
		{
			if (!(obj->text.text_param = parse_proc(line/*, &obj->text*/)))
				return (syn_error(SERROR, TEXT, TEXTPROC, OFFSCALE));
		}
	}
//	if (ret == 0)
//		return (syn_error(SERROR, TEXT, TEXTPROC, OFFSCALE));
	return (ret);
}

void	*parse_img(char **line/*, t_text *text*/)
{
	SDL_Surface		*image_bmp;
	SDL_Surface		*image;
	t_text_img		*param;
	unsigned int	pixels_nb;
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
	if (!(param = malloc(sizeof(t_text_img))))
		return (NULL);
	if (!(image_bmp = IMG_Load(image_name)))
		return (NULL);
	if (!(image = SDL_ConvertSurfaceFormat(image_bmp, SDL_PIXELFORMAT_RGBA8888, 0)))
		return (NULL);
	pixels_nb = image->w * image->h;
	if (!(param->pixels = malloc(sizeof(unsigned int) * pixels_nb)))
		return (NULL);
//	i += name_len + 1;
//	while (ft_isspace(*line[i]))
//		++i;
//	*index = i + 1;
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
	SDL_FreeSurface(image_bmp);
	SDL_FreeSurface(image);
	goto_next_element(line);
	return ((void *)param);
	(void)line;
}

void	*parse_proc(char **line/*, t_text *text*/)
{
	char	stripe;
	int		cmp;
	int		ret;
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
