/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_other_element.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aplat <aplat@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/18 05:03:07 by aplat             #+#    #+#             */
/*   Updated: 2020/05/19 19:00:42 by aplat            ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

int			parse_name(char **line, char **name, int i)
{
	int		start;
	char	*s;

	s = *line;
	while (ft_isspace(s[i]))
		++i;
	if (s[i] != '(')
		return (error(SERROR, NAME));
	if (*name)
		ft_strdel(name);
	start = ++i;
	while (s[i] && (s[i] != ')' && s[i] != '>'))
		++i;
	if (s[i] != ')')
		return (error(SERROR, NAME));
	*name = ft_strsub(s, start, i - start);
	++i;
	while (ft_isspace(s[i]))
		++i;
	if (goto_next_element(line) != '>')
		return (error(SERROR, NAME));
	return (1);
}

int			parse_color_filter(char **line, t_data *data)
{
	char	*s;

	s = *line;
	while (*s != '(' && *s)
		++s;
	if (!ft_strncmp_case(s, "(SEPIA)", 4))
		data->apply_color_filter = &apply_color_filter_sepia;
	else
		return (error(ERRORCOLOR, LSTCOLOR));
	goto_next_element(line);
	return (1);
}

int			parse_material(char **line, int i, t_obj *obj)
{
	char	*s;

	s = *line;
	while (s[i] && s[i] != '(')
		++i;
	if (s[i])
		++i;
	else
		return (0);
	if (!ft_strncmp_case(&(s[i]), "diffuse", 7))
		obj->material_type = MAT_DIFFUSE;
	else if (!ft_strncmp_case(&(s[i]), "negative", 8))
		obj->material_type = MAT_NEGATIVE;
	else
		return (error(ERRORMATERIAL, NULL));
	goto_next_element(line);
	return (1);
}

static void	push_front_motion(t_motion **root, t_motion *new)
{
	t_motion	*motion;

	clamp_val((double *)&new->spf, 0, 32);
	if (!*root)
		*root = new;
	else
	{
		motion = *root;
		while (motion->next)
			motion = motion->next;
		motion->next = new;
	}
}

int			parse_motion(char **line, t_obj *obj)
{
	char		stripe;
	int			ret;
	t_motion	*motion;

	stripe = 0;
	ret = 1;
	if (!(motion = ft_memalloc(sizeof(t_motion))))
		return (0);
	while (stripe != '>' && ret != 0)
	{
		stripe = goto_next_element(line);
		if (!(ft_strncmp_case(*line, "dir", 3)))
			ret = parse_origin(line, &motion->dir, 3);
		else if (!(ft_strncmp_case(*line, "speed", 3)))
			ret = parse_double(line, 5, &motion->speed_fact);
		else if (!(ft_strncmp_case(*line, "spf", 3)))
			ret = parse_int(line, 3, &motion->spf);
		else if (stripe == '<')
		{
			free(motion);
			return (syn_error(SERROR, MOTION, "<spf (Value)>\n", NULL));
		}
	}
	push_front_motion(&obj->motions, motion);
	return (ret);
}
