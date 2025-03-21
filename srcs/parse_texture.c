/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_texture.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aplat <aplat@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/15 20:37:39 by aplat             #+#    #+#             */
/*   Updated: 2020/05/23 19:38:55 by aplat            ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

static int	compute_text(char **line, t_obj *obj)
{
	if (obj->text.text_type == TEXT_IMAGE && !(obj->text.text_param))
	{
		if (!(obj->text.text_param = parse_texture_img(line)))
			return (syn_error(SERROR, TEXT, IMG, OFFSCALE));
	}
	else if (!(obj->text.text_param))
	{
		if (!(obj->text.text_param = parse_proc(line)))
			return (syn_error(SERROR, TEXT, TEXTPROC, OFFSCALE));
	}
	return (1);
}

static int	pick_ugp_text(char **line, t_obj *obj)
{
	if (!(ft_strncmp_case(*line, "UNI", 3)))
	{
		obj->get_text_color = &get_uni_color;
		obj->text.text_type = TEXT_UNI;
		return (compute_text(line, obj));
	}
	else if (!(ft_strncmp_case(*line, "GRID", 4)))
	{
		obj->get_text_color = &get_grid_color;
		obj->text.text_type = TEXT_GRID;
		return (compute_text(line, obj));
	}
	else if (!(ft_strncmp_case(*line, "PERLIN", 6)))
	{
		obj->get_text_color = &get_perlin_color;
		obj->text.text_type = TEXT_PERLIN;
		return (compute_text(line, obj));
	}
	return (1);
}

static int	pick_mwf_text(char **line, t_obj *obj)
{
	if (!(ft_strncmp_case(*line, "MARBLE", 6)))
	{
		obj->get_text_color = &get_marble_color;
		obj->text.text_type = TEXT_MARBLE;
		return (compute_text(line, obj));
	}
	else if (!(ft_strncmp_case(*line, "WOOD", 4)))
	{
		obj->get_text_color = &get_wood_color;
		obj->text.text_type = TEXT_WOOD;
		return (compute_text(line, obj));
	}
	else if (!(ft_strncmp_case(*line, "FBM", 3)))
	{
		obj->get_text_color = &get_fbm_color;
		obj->text.text_type = TEXT_FBM;
		return (compute_text(line, obj));
	}
	return (1);
}

static int	pick_att_image_text(char **line, t_obj *obj)
{
	int	ret;

	ret = 1;
	if (!(ft_strncmp_case(*line, "IMAGE", 5)))
	{
		obj->get_text_color = &get_image_color;
		obj->text.text_type = TEXT_IMAGE;
		return (compute_text(line, obj));
	}
	else if (!(ft_strncmp_case(*line, "offset", 6)))
		ret = parse_rotation(line, &obj->text.offset, 6);
	else if (!(ft_strncmp_case(*line, "scale", 5)))
		ret = parse_rotation(line, &obj->text.scale, 5);
	else if (!(ft_strncmp_case(*line, "BumpMapping", 11)) &&
		obj->text.text_param)
		ret = parse_bump_mapping(line, obj);
	return (ret);
}

int			parse_texture2(char **line, t_obj *obj)
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
		if (**line != '>' && !(ret = pick_ugp_text(line, obj)))
			return (0);
		else if (**line != '>' && !(ret = pick_mwf_text(line, obj)))
			return (0);
		else if (**line != '>' && !(ret = pick_att_image_text(line, obj)))
			return (0);
	}
	if (ret == 0 || !obj->text.text_param || !obj->get_text_color)
		return (syn_error(SERROR, TEXT, TEXTPROC, OFFSCALE));
	return (ret);
}
