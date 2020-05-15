/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aplat <aplat@student.le-101.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/27 02:04:16 by pduhard-          #+#    #+#             */
/*   Updated: 2020/03/12 18:34:48 by pduhard-         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

double	linear_interpolate(double a, double b, double val)
{
	return (a + (b - a) * val);
}

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
	t_4vecf		color;

	text = (t_text_proc *)obj->text.text_param;
  perlin_f = fabs(compute_3dperlin_factor(inter_point, obj->text.scale.val[0],
		obj->data->permutation, obj->data->gradient));
	color.val[0] = text->color[0].val[0] * perlin_f;
	color.val[1] = text->color[0].val[1] * perlin_f;
	color.val[2] = text->color[0].val[2] * perlin_f;
	color.val[3] = text->color[0].val[3] * (perlin_f + PERLIN_TRANSP_ADD);
	return (color);
	(void)normal_inter;
}

double	compute_wood_factor(t_3vecf inter_point, double scale,
	const unsigned char permutation[512], const t_3vecf gradient[16])
{
	double	perlin_f;
	double	wood_f;

	perlin_f = compute_3dperlin_factor(inter_point, scale, permutation, gradient);
	wood_f = (1. + sin((perlin_f / 2.) * 150.)) / 2.;
	return (wood_f);
}

double	marble_noise(t_3vecf inter_point, double scale,
	const unsigned char permutation[512], const t_3vecf gradient[16])
{
	double	t;

	t = 0;
	while (scale <= ROUGHCAST_LIMIT)
	{
		t += fabs(compute_3dperlin_factor(inter_point, scale,
			permutation, gradient) / scale);
	  scale *= 2;
	}
	return t;
}

double	compute_marble_factor(t_3vecf inter_point, double scale,
	const unsigned char permutation[512], const t_3vecf gradient[16])
{
	double t;

 	t = (0.5 + 0.5 * sin(scale * 2 * M_PI * (inter_point.val[0] + 2
		* marble_noise(inter_point, scale, permutation, gradient))));
	return (t * t - .5);
}

t_4vecf	get_wood_color(t_3vecf inter_point, t_3vecf normal_inter, t_obj *obj)
{
	double	wood_f;
	t_4vecf	color;
	t_text_proc	*text;

	text = (t_text_proc *)obj->text.text_param;
	wood_f = compute_wood_factor(inter_point, obj->text.scale.val[0],
		obj->data->permutation, obj->data->gradient);
	color.val[0] = linear_interpolate(text->color[0].val[0],
		text->color[1].val[0], wood_f);
	color.val[1] = linear_interpolate(text->color[0].val[1],
		text->color[1].val[1], wood_f);
	color.val[2] = linear_interpolate(text->color[0].val[2],
		text->color[1].val[2], wood_f);
	color.val[3] = linear_interpolate(text->color[0].val[3],
		text->color[1].val[3], wood_f);
	return (color);
	(void)normal_inter;
}

t_4vecf	get_marble_color(t_3vecf inter_point, t_3vecf normal_inter, t_obj *obj)
{
	double	marble_f;
	t_4vecf	color;
	t_text_proc	*text;

	text = (t_text_proc *)obj->text.text_param;
	marble_f = compute_marble_factor(inter_point, obj->text.scale.val[0],
		obj->data->permutation, obj->data->gradient);
	color.val[0] = linear_interpolate(text->color[0].val[0],
		text->color[1].val[0], marble_f);
	color.val[1] = linear_interpolate(text->color[0].val[1],
		text->color[1].val[1], marble_f);
	color.val[2] = linear_interpolate(text->color[0].val[2],
		text->color[1].val[2], marble_f);
	color.val[3] = linear_interpolate(text->color[0].val[3],
		text->color[1].val[3], marble_f);
	return (color);
	(void)normal_inter;
}

t_4vecf	get_fbm_color(t_3vecf inter_point, t_3vecf normal_inter, t_obj *obj)
{
	double	fbm_f;
	t_4vecf	color;
	t_text_proc	*text;

	text = (t_text_proc *)obj->text.text_param;
	fbm_f = compute_3dfbm_factor(inter_point, obj->text.scale.val[0]);
	color.val[0] = linear_interpolate(text->color[0].val[0],
		text->color[1].val[0], fbm_f);
	color.val[1] = linear_interpolate(text->color[0].val[1],
		text->color[1].val[1], fbm_f);
	color.val[2] = linear_interpolate(text->color[0].val[2],
		text->color[1].val[2], fbm_f);
	color.val[3] = linear_interpolate(text->color[0].val[3],
		text->color[1].val[3], fbm_f);
	return (color);
	(void)normal_inter;
}

int     get_pixel_addr(t_2vecf text_coord, t_text_img *text, t_obj *obj)
{
	int		row;
	int		col;

	if (text_coord.val[1] < 0)
		col = (int)((1 - ((-text_coord.val[1] - (int)-text_coord.val[1])))
			* (double)(text->width));
	else
		col = (int)((text_coord.val[1] - (int)text_coord.val[1])
			* (double)(text->width));
	if (text_coord.val[0] < 0)
		row = (int)((1 - (-text_coord.val[0] - (int)-text_coord.val[0]))
			* (double)(text->height));
	else
		row = (int)((text_coord.val[0] - (int)text_coord.val[0])
			* (double)(text->height));
	row += obj->text.offset.val[0] * (double)(text->height);
	col += (1 - obj->text.offset.val[1]) * (double)(text->width);
	row %= text->height;
	col %= text->width;
	return (row * text->width + col);
}

t_4vecf	get_image_color(t_3vecf inter_point, t_3vecf normal_inter, t_obj *obj)
{
	t_2vecf			text_coord;
	t_4vecf			color;
	t_text_img	*text;
	int					pixel_addr;

	text = (t_text_img *)obj->text.text_param;
	text_coord = obj->get_text_coordinate(inter_point, normal_inter, obj);
	text_coord.val[1] *= obj->text.scale.val[1];
	text_coord.val[0] *= obj->text.scale.val[0];
	pixel_addr = get_pixel_addr(text_coord, text, obj);
	color.val[0] = (double)(text->pixels[pixel_addr] >> 24 & 0xff) / 255.;
	color.val[1] = (double)(text->pixels[pixel_addr] >> 16 & 0xff) / 255.;
	color.val[2] = (double)(text->pixels[pixel_addr] >> 8 & 0xff) / 255.;
	color.val[3] = (255. - (double)(text->pixels[pixel_addr] & 0xff)) / 255.;
	return (color);
	(void)normal_inter;
}

t_4vecf	get_grid_color(t_3vecf inter_point, t_3vecf normal_inter, t_obj *obj)
{
	t_2vecf	text_coord;
	t_text_proc	*text;
	int		u_grid;
	int		v_grid;

	text = (t_text_proc *)obj->text.text_param;
	text_coord = obj->get_text_coordinate(inter_point, normal_inter, obj);
	text_coord.val[0] *= obj->text.scale.val[0];
	text_coord.val[1] *= obj->text.scale.val[1];
	text_coord.val[0] -= text_coord.val[0] < 0 ? (1 + fmod(text_coord.val[0], 1))
		: fmod(text_coord.val[0], 1);
	text_coord.val[1] -= text_coord.val[1] < 0 ? (1 + fmod(text_coord.val[1], 1))
		: fmod(text_coord.val[1], 1);
	u_grid = (fmod(text_coord.val[0], 2) == 0);
	v_grid = (fmod(text_coord.val[1], 2) == 0);
	if ((u_grid ^ v_grid))
		return (text->color[0]);
	return (text->color[1]);
}

t_4vecf			(*assign_text_color_function(t_text_type type))(t_3vecf,
	t_3vecf, struct s_obj *)
{
		if (type == TEXT_PERLIN)
			return (&get_perlin_color);
		if (type == TEXT_MARBLE)
			return (&get_marble_color);
		if (type == TEXT_WOOD)
			return (&get_wood_color);
		if (type == TEXT_FBM)
			return (&get_fbm_color);
		if (type == TEXT_UNI)
			return (&get_uni_color);
		if (type == TEXT_GRID)
			return (&get_grid_color);
		return (&get_uni_color);
}

t_text	generate_random_texture(t_obj *obj)
{
	t_text		text;
	t_text_proc	*param;

	text.scale.val[0] = get_random_number(time(NULL) << 6)
		* get_random_number(time(NULL) >> 4) * 10;
	text.scale.val[1] = get_random_number(time(NULL) >> 6)
		* get_random_number(time(NULL) << 3) * 10;
	if (!(param = ft_memalloc(sizeof(t_text_proc))))
		exit(EXIT_FAILURE);
	param->color[0] = generate_random_color(time(NULL), 0.25);
	param->color[1] = generate_random_color(time(NULL) & 0xfd9fd897, 0.5);
	param->color[2] = generate_random_color(time(NULL) + 303, 0.55);
	text.text_type = generate_random_enum(6);
	if (text.text_type < 4)
		text.bump_fact = 0.01 + get_random_number(time(NULL)) * 0.03;
	text.text_param = param;
	obj->get_text_color = assign_text_color_function(text.text_type);
	return (text);
}
