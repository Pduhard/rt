/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   texture.c                                        .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: pduhard- <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2020/01/19 17:18:27 by pduhard-     #+#   ##    ##    #+#       */
/*   Updated: 2020/01/22 08:44:08 by pduhard-    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "rt.h"

t_3vecf	get_uni_color(t_3vecf inter_point, t_3vecf normal_inter, t_obj *obj)
{
	return (obj->text.color_1);
	(void)inter_point; // warning
	(void)normal_inter; // warning
}

t_3vecf	get_perlin_color(t_3vecf inter_point, t_3vecf normal_inter, t_obj *obj)
{
	double	perlin_f = fabs(compute_perlin_factor(inter_point));

/*	if (perlin_f > 0.8)
		return (assign_3vecf(obj->text.color_1.val[0] * perlin_f, obj->text.color_1.val[1] * perlin_f, obj->text.color_1.val[2] * perlin_f));
	else if (perlin_f > 0.5)
		return (assign_3vecf(obj->text.color_2.val[0] * perlin_f, obj->text.color_2.val[1] * perlin_f, obj->text.color_2.val[2] * perlin_f));
	else
		return (assign_3vecf(obj->text.color_3.val[0] * perlin_f, obj->text.color_3.val[1] * perlin_f, obj->text.color_3.val[2] * perlin_f));
*/	return (assign_3vecf(obj->text.color_1.val[0] * perlin_f, obj->text.color_1.val[1] * perlin_f, obj->text.color_1.val[2] * perlin_f));
	(void)normal_inter;
}

t_3vecf	get_wood_color(t_3vecf inter_point, t_3vecf normal_inter, t_obj *obj)
{
	t_2vecf	text_coord;
	double	perlin_f;
	double	wood_f;
	t_3vecf	color;

	perlin_f = compute_perlin_factor(inter_point);
	text_coord = obj->get_text_coordinate(inter_point, normal_inter, obj);
	wood_f = (1. + sin((/*text_coord.val[1] + */perlin_f / 2.) * 150.)) / 2.;
//	color.val[0] = obj->text.color_1.val[0] * (1 - marble_f) + obj->text.color_2.val[0] * marble_f;
//	color.val[1] = obj->text.color_1.val[1] * (1 - marble_f) + obj->text.color_2.val[1] * marble_f;
//	color.val[2] = obj->text.color_1.val[2] * (1 - marble_f) + obj->text.color_2.val[2] * marble_f;

	color.val[0] = linear_interpolate(obj->text.color_1.val[0], obj->text.color_2.val[0], wood_f);
	color.val[1] = linear_interpolate(obj->text.color_1.val[1], obj->text.color_2.val[1], wood_f);
	color.val[2] = linear_interpolate(obj->text.color_1.val[2], obj->text.color_2.val[2], wood_f);
	return (color);
	(void)normal_inter;
}

t_3vecf	get_marble_color(t_3vecf inter_point, t_3vecf normal_inter, t_obj *obj)
{
	t_2vecf	text_coord;
	double	perlin_f;
	double	marble_f;
	t_3vecf	color;

	//perlin_f = compute_perlin_factor(inter_point);
	perlin_f = compute_perlin_factor(assign_3vecf(inter_point.val[0] * 5, inter_point.val[1] * 5, inter_point.val[2] * 5));
	text_coord = obj->get_text_coordinate(inter_point, normal_inter, obj);
	marble_f = (1. + sin((text_coord.val[1] + perlin_f) * 3.)) / 2.;
	//wood_f = inter_point.val[0] * inter_point.val[0] + inter_point.val[1] * inter_point.val[1] + perlin_f;
//	text_coord = obj->get_text_coordinate(inter_point, normal_inter, obj);
//	color.val[0] = obj->text.color_1.val[0] * (1 - marble_f) + obj->text.color_2.val[0] * marble_f;
//	color.val[1] = obj->text.color_1.val[1] * (1 - marble_f) + obj->text.color_2.val[1] * marble_f;
//	color.val[2] = obj->text.color_1.val[2] * (1 - marble_f) + obj->text.color_2.val[2] * marble_f;

	color.val[0] = linear_interpolate(obj->text.color_1.val[0], obj->text.color_2.val[0], marble_f);
	color.val[1] = linear_interpolate(obj->text.color_1.val[1], obj->text.color_2.val[1], marble_f);
	color.val[2] = linear_interpolate(obj->text.color_1.val[2], obj->text.color_2.val[2], marble_f);
	return (color);
	(void)normal_inter;
}

t_3vecf	get_grid_color(t_3vecf inter_point, t_3vecf normal_inter, t_obj *obj)
{
	t_2vecf	text_coord;

	text_coord = obj->get_text_coordinate(inter_point, normal_inter, obj);
	text_coord.val[0] -= text_coord.val[0] < 0 ? (1 + fmod(text_coord.val[0], 1)) : fmod(text_coord.val[0], 1);
	text_coord.val[1] -= text_coord.val[1] < 0 ? (1 + fmod(text_coord.val[1], 1)) : fmod(text_coord.val[1], 1);
	int		u_grid;
	int		v_grid;

	u_grid = (fmod(text_coord.val[0], 2) == 0);
	v_grid = (fmod(text_coord.val[1], 2) == 0);
	if ((u_grid ^ v_grid))
		return (obj->text.color_1);
	return (obj->text.color_2);
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
	else
	{
		ft_printf("Unrecognized texture\n");
		return (-1);
	}
	while (ft_isspace(line[i]))
		++i;
	if (line[i] != '(' || (i = parse_3vecf(line, i, &(obj->text.color_1))) == -1)
	{
		return (-1);
	}
	while (ft_isspace(line[i]))
		++i;
	if (line[i] != '(' || (i = parse_3vecf(line, i, &(obj->text.color_2))) == -1)
	{
		return (-1);
	}
	while (ft_isspace(line[i]))
		++i;
	if (line[i] != '(' || (i = parse_3vecf(line, i, &(obj->text.color_3))) == -1)
	{
		return (-1);
	}
	while (ft_isspace(line[i]))
		++i;
	if (line[i] == ')')
		++i;
	obj->color = obj->text.color_1;
	return (i);
}
