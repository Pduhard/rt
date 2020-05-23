/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_composed_model.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aplat <aplat@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/18 04:50:06 by aplat             #+#    #+#             */
/*   Updated: 2020/05/18 04:50:12 by aplat            ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

static int	parse_composed_model_element(char **line, t_composed *composed,
	t_data *data, int *ret)
{
	char		stripe;

	stripe = 0;
	*ret = 1;
	while (stripe != '>' && *ret != 0)
	{
		stripe = goto_next_element(line);
		if (!(ft_strncmp_case(*line, "objects", 7)))
			*ret = parse_objects(line, data, composed);
		else if (!(ft_strncmp_case(*line, "name", 4)))
			*ret = parse_name(line, &composed->name, 4);
	}
	return (*ret);
}

static int	check_composed_model(t_composed *composed)
{
	int i;

	if (!composed->name)
	{
		if (composed->components)
		{
			i = 0;
			while (composed->components[i])
				free_object(composed->components[i++]);
			free(composed->components);
		}
		ft_fdprintf(2, "Composed object model need a name\n");
		return (0);
	}
	else if (!composed->components)
	{
		if (composed->name)
			free(composed->name);
		ft_fdprintf(2, "Composed object model need at least one object\n");
		return (0);
	}
	return (1);
}

int	parse_composed_model(char **line, t_data *data)
{
	char		stripe;
	int			ret;
	t_composed	*composed;

	stripe = 0;
	ret = 1;
	if (!(composed = ft_memalloc(sizeof(t_composed))))
		return (0);
	if (!parse_composed_model_element(line, composed, data, &ret) ||
			!check_composed_model(composed))
	{
			free(composed);
			return (0);
	}
	// while (stripe != '>' && ret != 0)
	// {
	// 	stripe = goto_next_element(line);
	// 	if (!(ft_strncmp_case(*line, "objects", 7)))
	// 		ret = parse_objects(line, data, composed);
	// 	else if (!(ft_strncmp_case(*line, "name", 4)))
	// 		ret = parse_name(line, &composed->name, 4);
	// }

	if (data->composed_objs)
		composed->next = data->composed_objs;
	else
		composed->next = NULL;
	data->composed_objs = composed;
	return (ret);
}
