/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_composed_object.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aplat <aplat@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/15 19:56:28 by aplat             #+#    #+#             */
/*   Updated: 2020/05/15 20:18:43 by aplat            ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

static t_composed	*get_composed_object(char *name, t_composed *list)
{
	t_composed *composed;

	composed = NULL;
	while (list)
	{
		if (!ft_strncmp(name, list->name, ft_strlen(list->name)))
			composed = list;
		list = list->next;
	}
	return (composed);
}

static t_obj		**init_composed_obj_tab(t_composed *composed)
{
	int		i;
	t_obj	**obj_tab;

	i = 0;
	while (composed->components[i])
		i++;
	if (!(obj_tab = ft_memalloc(sizeof(t_obj *) * (i + 1))))
		return (NULL);
	return (obj_tab);
}

static int			parse_composed_info(char **line, t_3vecf *origin,
	t_2vecf *rotation, int *ret)
{
	char	stripe;

	stripe = '\0';
	while (stripe != '>' && *ret != 0)
	{
		stripe = goto_next_element(line);
		if (!(ft_strncmp_case(*line, "origin", 6)))
			*ret = parse_origin(line, origin, 6);
		else if (!(ft_strncmp_case(*line, "rotation", 8)))
			*ret = parse_rotation(line, rotation, 8);
		else if (stripe == '<')
			return (syn_error(SERROR, COMPOSED, ROTATION, "\t>\n"));
	}
	return (*ret);
}

static int			fill_obj_tab(t_obj **obj_tab, t_comp_param p,
	t_composed *composed, t_data *data)
{
	t_33matf	rot_mat[2];
	t_3vecf		tm;
	int			i;

	rot_mat[1] = init_rotation_matrix_y(degree_to_radian(p.rotation.val[1]));
	tm = mult_3vecf_33matf(assign_3vecf(1, 0, 0), rot_mat[1]);
	rot_mat[0] = init_rotation_matrix_vec(tm,
		degree_to_radian(p.rotation.val[0]));
	i = 0;
	while (composed->components[i])
	{
		if (!(obj_tab[i] = copy_object(composed->components[i])))
			return (0);
		obj_tab[i]->rotate(obj_tab[i], assign_3vecf(0, 0, 0), rot_mat);
		obj_tab[i]->move(obj_tab[i], p.origin, 1);
		add_object(obj_tab[i], data);
		i++;
	}
	return (1);
}

int			is_composed_object(char **line, t_data *data, int *ret)
{
	t_composed	*composed;
	t_obj		**obj_tab;
	t_3vecf		origin;
	t_2vecf		rotation;
	int       i;

	if (!(composed = get_composed_object(*line, data->composed_objs)) ||
			!(obj_tab = init_composed_obj_tab(composed)) ||
			!parse_composed_info(line, &origin, &rotation, ret) ||
			!fill_obj_tab(obj_tab, (t_comp_param){origin, rotation},
			composed, data))
		return (0);
	i = 0;
	while (obj_tab[i])
	{
		obj_tab[i]->composed_origin = origin;
		obj_tab[i++]->composed_w = obj_tab;
	}
	return (1);
}
