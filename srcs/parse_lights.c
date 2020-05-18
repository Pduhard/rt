/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_lights.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aplat <aplat@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/15 20:30:43 by aplat             #+#    #+#             */
/*   Updated: 2020/05/18 05:01:07 by aplat            ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

int			check_lights_cam(t_data *data)
{
	t_light	*light;
	t_light	*tmp;

	tmp = data->lights;
	while (tmp && tmp->light_type != LIGHT_AMBIENT)
		tmp = tmp->next;
	if (!tmp)
	{
		if (!(light = malloc(sizeof(t_light))))
			return (0);
		light->light_type = LIGHT_AMBIENT;
		light->color = assign_3vecf(0.2, 0.2, 0.2);
		light->next = NULL;
		if (data->lights)
			light->next = data->lights;
		else
			light->next = NULL;
		data->lights = light;
	}
	if (!data->camera)
		return (error(ERRORCAM, NULL));
	return (1);
}

static int	pick_spot(char **line, t_light *light)
{
	int		ret;

	ret = 1;
	if (!(ft_strncmp_case(*line, "directional", 11)))
	{
		light->light_type = LIGHT_DIRECTIONAL;
		ret = parse_origin(line, &light->param, 11);
		if (goto_next_element(line) && !(ft_strncmp_case(*line, "color", 5)))
			ret = parse_origin(line, &light->color, 5);
		else
			return (syn_error(SERROR, LIGHT, DIRECTIONAL, NULL));
	}
	else if (!(ft_strncmp_case(*line, "point", 5)))
	{
		light->light_type = LIGHT_POINT;
		ret = parse_origin(line, &light->param, 5);
		if (goto_next_element(line) && !(ft_strncmp_case(*line, "color", 5)))
			ret = parse_origin(line, &light->color, 5);
		else
			return (syn_error(SERROR, LIGHT, POINT, NULL));
	}
	else if (**line != '<')
		return (error(UNKNOWLIGHT, NULL));
	return (ret);
}

static void	push_lights(t_data *data, t_light *light)
{
	if (data->lights)
		light->next = data->lights;
	else
		light->next = NULL;
	data->lights = light;
}

int			parse_lights(char **line, t_data *data)
{
	char	stripe;
	int		ret;
	t_light	*light;

	stripe = 0;
	ret = 2;
	if (!(light = malloc(sizeof(t_light))))
		return (0);
	while (stripe != '>' && ret != 0 && ret != 1)
	{
		stripe = goto_next_element(line);
		if (!(ft_strncmp_case(*line, "ambient", 7)))
		{
			light->light_type = LIGHT_AMBIENT;
			ret = parse_origin(line, &light->color, 7);
			if (**line == '<')
			{
				free(light);
				return (syn_error(SERROR, LIGHT, AMBIENT, NULL));
			}
		}
		else if (**line != '>' && !(ret = pick_spot(line, light)))
		{
			free(light);
			return (0);
		}
	}
	if (ret == 1)
		goto_next_element(line);
	push_lights(data, light);
	return (ret);
}
