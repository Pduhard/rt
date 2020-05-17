/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aplat <aplat@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/15 21:00:57 by aplat             #+#    #+#             */
/*   Updated: 2020/05/15 21:10:13 by aplat            ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

double	linear_interpolate(double a, double b, double val)
{
	return (a + (b - a) * val);
}

t_4vecf	(*assign_text_color_function(t_text_type type))(t_3vecf,
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
