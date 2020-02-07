/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   texture.c                                        .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: aplat <aplat@student.le-101.fr>            +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2020/01/19 17:18:27 by pduhard-     #+#   ##    ##    #+#       */
/*   Updated: 2020/02/06 21:34:03 by pduhard-    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "rt.h"

t_4vecf	get_uni_color(t_3vecf inter_point, t_3vecf normal_inter, t_obj *obj)
{
	t_text_proc	*text;

	text = (t_text_proc *)obj->text.text_param;
	return (text->color[0]);
	(void)inter_point; // warning
	(void)normal_inter; // warning
}

t_4vecf	get_perlin_color(t_3vecf inter_point, t_3vecf normal_inter, t_obj *obj)
{
	double	perlin_f;
	t_text_proc	*text;
	t_4vecf	color;
	text = (t_text_proc *)obj->text.text_param;

   	perlin_f = fabs(compute_3dperlin_factor(inter_point, obj->text.scale.val[0]));
/*	if (perlin_f > 0.8)
		return (text->color[0]);
	//	return (assign_3vecf(obj->text.color[0].val[0] * perlin_f, obj->text.color[0].val[1] * perlin_f, obj->text.color[0].val[2] * perlin_f));
	else if (perlin_f > 0.5)
		return (text->color[1]);
	//	return (assign_3vecf(obj->text.color[1].val[0] * perlin_f, obj->text.color[1].val[1] * perlin_f, obj->text.color[1].val[2] * perlin_f));
	else
		return (text->color[2]);
	//	return (assign_3vecf(obj->text.color_3.val[0] * perlin_f, obj->text.color_3.val[1] * perlin_f, obj->text.color_3.val[2] * perlin_f));
*/
//	printf("%f\n", perlin_f);
	color.val[0] = text->color[0].val[0] * perlin_f;
	color.val[1] = text->color[0].val[1] * perlin_f;
	color.val[2] = text->color[0].val[2] * perlin_f;
	color.val[3] = text->color[0].val[3] * (perlin_f + PERLIN_TRANSP_ADD);
	return (color);
	(void)normal_inter;
}

double	compute_wood_factor(t_3vecf inter_point, double scale)
{
	double	perlin_f;
	double	wood_f;

	perlin_f = compute_3dperlin_factor(inter_point, scale);
	wood_f = (1. + sin((/*text_coord.val[1] + */perlin_f / 2.) * 150.)) / 2.;

	return (wood_f);
}

double	marble_noise(t_3vecf inter_point, double scale)
{
	double	t;

	t = 0;
	while (scale <= ROUGHCAST_LIMIT)
	{
		t += fabs(compute_3dperlin_factor(inter_point, scale) / scale);
	   	scale *= 2;
	}
	return t;
}

double	compute_marble_factor(t_3vecf inter_point, t_3vecf normal_inter, t_obj *obj, double scale)
{
	double t = (0.5 + 0.5 * sin(scale * 2 * M_PI * (inter_point.val[0] + 2 * marble_noise(inter_point, scale))));
	return (t * t - .5);
	(void)normal_inter;
	(void)obj;
}

t_4vecf	get_wood_color(t_3vecf inter_point, t_3vecf normal_inter, t_obj *obj)
{
	double	wood_f;
	t_4vecf	color;
	t_text_proc	*text;

	text = (t_text_proc *)obj->text.text_param;
	wood_f = compute_wood_factor(inter_point, obj->text.scale.val[0]);
//	color.val[0] = obj->text.color[0].val[0] * (1 - marble_f) + obj->text.color[1].val[0] * marble_f;
//	color.val[1] = obj->text.color[0].val[1] * (1 - marble_f) + obj->text.color[1].val[1] * marble_f;
//	color.val[2] = obj->text.color[0].val[2] * (1 - marble_f) + obj->text.color[1].val[2] * marble_f;

	color.val[0] = linear_interpolate(text->color[0].val[0], text->color[1].val[0], wood_f);
	color.val[1] = linear_interpolate(text->color[0].val[1], text->color[1].val[1], wood_f);
	color.val[2] = linear_interpolate(text->color[0].val[2], text->color[1].val[2], wood_f);
	color.val[3] = linear_interpolate(text->color[0].val[3], text->color[1].val[3], wood_f);
	return (color);
	(void)normal_inter;
}

t_4vecf	get_marble_color(t_3vecf inter_point, t_3vecf normal_inter, t_obj *obj)
{
//	t_2vecf	text_coord;
//	double	perlin_f;
	double	marble_f;
	t_4vecf	color;
	t_text_proc	*text;

	text = (t_text_proc *)obj->text.text_param;
	//perlin_f = compute_perlin_factor(inter_point);
//	perlin_f = compute_3dperlin_factor(assign_3vecf(inter_point.val[0] * 5, inter_point.val[1] * 5, inter_point.val[2] * 5), obj->text.scale.val[0]);
//	text_coord = obj->get_text_coordinate(inter_point, normal_inter, obj);
	marble_f = compute_marble_factor(inter_point, normal_inter, obj, obj->text.scale.val[0]);
	//(1. + sin((text_coord.val[1] + perlin_f / 2) * 3.)) / 2.;
	//wood_f = inter_point.val[0] * inter_point.val[0] + inter_point.val[1] * inter_point.val[1] + perlin_f;
//	text_coord = obj->get_text_coordinate(inter_point, normal_inter, obj);
//	color.val[0] = obj->text.color[0].val[0] * (1 - marble_f) + obj->text.color[1].val[0] * marble_f;
//	color.val[1] = obj->text.color[0].val[1] * (1 - marble_f) + obj->text.color[1].val[1] * marble_f;
//	color.val[2] = obj->text.color[0].val[2] * (1 - marble_f) + obj->text.color[1].val[2] * marble_f;

	color.val[0] = linear_interpolate(text->color[0].val[0], text->color[1].val[0], marble_f);
	color.val[1] = linear_interpolate(text->color[0].val[1], text->color[1].val[1], marble_f);
	color.val[2] = linear_interpolate(text->color[0].val[2], text->color[1].val[2], marble_f);
	color.val[3] = linear_interpolate(text->color[0].val[3], text->color[1].val[3], marble_f);
	return (color);
	(void)normal_inter;
}

t_4vecf	get_image_color(t_3vecf inter_point, t_3vecf normal_inter, t_obj *obj)
{
	t_2vecf	text_coord;
	t_4vecf	color;
	t_text_img	*text;
	int		row;
	int		col;
//	int		offset_row;
//	int		offset_col;
	int		pixel_addr;

	text = (t_text_img *)obj->text.text_param;
	text_coord = obj->get_text_coordinate(inter_point, normal_inter, obj);
	text_coord.val[1] *= obj->text.scale.val[1];
	text_coord.val[0] *= obj->text.scale.val[0];
	if (text_coord.val[1] < 0)
		col = (int)((1 - ((-text_coord.val[1] - (int)-text_coord.val[1]))) * (double)(text->width));
	else
		col = (int)((text_coord.val[1] - (int)text_coord.val[1]) * (double)(text->width));
	if (text_coord.val[0] < 0)
		row = (int)((1 - (-text_coord.val[0] - (int)-text_coord.val[0])) * (double)(text->height));
	else
		row = (int)((text_coord.val[0] - (int)text_coord.val[0]) * (double)(text->height));
//	row = (int)((text_coord.val[0] - (int)text_coord.val[0]) * (double)(text->height));
//	col = (int)((text_coord.val[1] - (int)text_coord.val[1]) * (double)(text->width));
	row += obj->text.offset.val[0] * (double)(text->height);
	col += (1 - obj->text.offset.val[1]) * (double)(text->width);

	row %= text->height;
	col %= text->width;
	pixel_addr = row * text->width + col;
	//pixel_addr = (((row + offset_row - 1) * text->width) % text->height) + ((col + offset_col) % text->width);
//	pixel_addr = pixel_addr % (text->height * text->width);
	color.val[0] = (double)(text->pixels[pixel_addr] >> 24 & 0xff) / 255.;
	color.val[1] = (double)(text->pixels[pixel_addr] >> 16 & 0xff) / 255.;
	color.val[2] = (double)(text->pixels[pixel_addr] >> 8 & 0xff) / 255.;
	color.val[3] = (255. - (double)(text->pixels[pixel_addr] & 0xff)) / 255.;
	//linear_interpolate(text->color[0].val[0], text->color[1].val[0], marble_f);
	//color.val[1] = linear_interpolate(text->color[0].val[1], text->color[1].val[1], marble_f);
	//color.val[2] = linear_interpolate(text->color[0].val[2], text->color[1].val[2], marble_f);
	return (color);
	(void)normal_inter;
}

t_4vecf	get_grid_color(t_3vecf inter_point, t_3vecf normal_inter, t_obj *obj)
{
	t_2vecf	text_coord;
	t_text_proc	*text;

	text = (t_text_proc *)obj->text.text_param;
	text_coord = obj->get_text_coordinate(inter_point, normal_inter, obj);
	text_coord.val[0] *= obj->text.scale.val[0];
	text_coord.val[1] *= obj->text.scale.val[1];
	text_coord.val[0] -= text_coord.val[0] < 0 ? (1 + fmod(text_coord.val[0], 1)) : fmod(text_coord.val[0], 1);
	text_coord.val[1] -= text_coord.val[1] < 0 ? (1 + fmod(text_coord.val[1], 1)) : fmod(text_coord.val[1], 1);
	int		u_grid;
	int		v_grid;

	u_grid = (fmod(text_coord.val[0], 2) == 0);
	v_grid = (fmod(text_coord.val[1], 2) == 0);
	if ((u_grid ^ v_grid))
		return (text->color[0]);
	return (text->color[1]);
}

/*void	*parse_procedural(char *line, int *index)
{
	int	i;
	t_text_proc	*param;

	if (!(param = malloc(sizeof(t_text_proc))))
		return (NULL);
	i = *index;
	if (line[i] != '(' || (i = parse_3vecf(line, i, &(param->color[0]))) == -1)
	{
		return (NULL);
	}
	while (ft_isspace(line[i]))
		++i;
	if (line[i] != '(' || (i = parse_3vecf(line, i, &(param->color[1]))) == -1)
	{
		return (NULL);
	}
	while (ft_isspace(line[i]))
		++i;
	if (line[i] != '(' || (i = parse_3vecf(line, i, &(param->color_3))) == -1)
	{
		return (NULL);
	}
	while (ft_isspace(line[i]))
		++i;
	if (line[i] == ')')
		++i;
	*index = i;
	return ((void *)param);
}

void	*parse_image(char *line, int *index)
{
	SDL_Surface		*image_bmp;
	SDL_Surface		*image;
	t_text_img		*param;
	unsigned int	pixels_nb;
	unsigned int	i;
	int				name_len;
	char			*image_name;

	i = *index;
	printf("%s\n", &(line[*index]));
	if (line[i++] != '(')
		return (NULL);
	if ((name_len = ft_strichr(&(line[i]), ')')) == -1)
		return (NULL);
	if (!(image_name = ft_strsub(line, i, name_len)))
		return (NULL);
//	printf("%s %d\n", image_name, name_len);
	if (!(param = malloc(sizeof(t_text_img))))
		return (NULL);
	if (!(image_bmp = IMG_Load(image_name)))
		return (NULL);
	if (!(image = SDL_ConvertSurfaceFormat(image_bmp, SDL_PIXELFORMAT_RGB888, 0)))
		return (NULL);
	pixels_nb = image->w * image->h;
	if (!(param->pixels = malloc(sizeof(unsigned int) * pixels_nb)))
		return (NULL);
	i += name_len + 1;
	while (ft_isspace(line[i]))
		++i;
	*index = i + 1;
	printf("%s\n", &(line[*index]));
	param->width = image->w;
	param->height = image->h;
	printf("%d %d\n", image->w, image->h);
	SDL_LockSurface(image);
	i = 0;
	while (i < pixels_nb)
	{
		param->pixels[i] = ((unsigned int *)image->pixels)[i];
		++i;
	}
	printf("%s\n", &(line[*index]));
	SDL_UnlockSurface(image);
	SDL_FreeSurface(image_bmp);
	SDL_FreeSurface(image);
	return ((void *)param);
	(void)line;
	(void)index;
}

int		parse_texture(char *line, int i, t_obj *obj)
{
	if (line[i] == '(')
		++i;
	while (ft_isspace(line[i]))
		++i;
	if (!(ft_strncmp(&(line[i]), "UNI", 3)))
	{
		obj->get_text_color = &get_uni_color;
		obj->text.text_type = TEXT_UNI;
		i += 3;
	}
	else if (!(ft_strncmp(&(line[i]), "GRID", 4)))
	{
		obj->get_text_color = &get_grid_color;
		obj->text.text_type = TEXT_GRID;
		i += 4;
	}
	else if (!(ft_strncmp(&(line[i]), "PERLIN", 6)))
	{
		obj->get_text_color = &get_perlin_color;
		obj->text.text_type = TEXT_PERLIN;
		i += 6;
	}
	else if (!(ft_strncmp(&(line[i]), "MARBLE", 6)))
	{
		obj->get_text_color = &get_marble_color;
		obj->text.text_type = TEXT_MARBLE;
		i += 6;
	}
	else if (!(ft_strncmp(&(line[i]), "WOOD", 4)))
	{
		obj->get_text_color = &get_wood_color;
		obj->text.text_type = TEXT_WOOD;
		i += 4;
	}
	else if (!(ft_strncmp(&(line[i]), "IMAGE", 5)))
	{
		obj->get_text_color = &get_image_color;
		obj->text.text_type = TEXT_IMAGE;
		i += 5;
	}
	else
	{
		ft_printf("Unrecognized texture\n");
		return (-1);
	}
	if (obj->text.text_type == TEXT_IMAGE)
	{
		if (!(obj->text.text_param = parse_image(line, &i)))
			return (-1);
	}
	else
	{
		if (!(obj->text.text_param = parse_procedural(line, &i)))
			return (-1);
	}
	while (ft_isspace(line[i]))
		++i;
//	obj->color = assign_3vecf(1, 1, 1);
	//bj->text.color[0];
	return (i);
}*/
