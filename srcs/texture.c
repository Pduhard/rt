/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   texture.c                                        .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: pduhard- <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2020/01/19 17:18:27 by pduhard-     #+#   ##    ##    #+#       */
/*   Updated: 2020/01/19 18:50:16 by pduhard-    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "rt.h"

t_3vecf	get_uni_color(double x, double y, double z, t_obj *obj)
{
	return (obj->text.color_1);
	x = y + z; // warning
}

t_3vecf	get_grid_color(double x, double y, double z, t_obj *obj)
{
	t_2vecf	text_coord;

	text_coord = obj->get_text_coordinate(assign_3vecf(x, y, z), obj);
	
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
