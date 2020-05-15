/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_scene_element.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aplat <aplat@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/15 20:36:05 by aplat             #+#    #+#             */
/*   Updated: 2020/05/15 20:36:48 by aplat            ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

int		parse_onoff(char **line, int *onoff)
{
	char	*s;

	s = *line;
	while (*s != '(' && *s)
		++s;
	if (!ft_strncmp_case(s, "(ON)", 4))
		*onoff = 1;
	else if (!ft_strncmp_case(s, "(OFF)", 5))
		*onoff = 0;
	else if (!ft_strncmp_case(s, "(1)", 3))
		*onoff = 1;
	else if (!ft_strncmp_case(s, "(0)", 3))
		*onoff = 0;
	else
		return (error(ERRORON, NULL));
	goto_next_element(line);
	return (1);
}

int		parse_size(char **line, t_data *data)
{
	int		i;
	char	*s;

	i = 4;
	s = *line;
	while (ft_isspace(s[i]))
		++i;
	if (s[i] != '(' || (i = parse_2vecf(s, i, &data->size)) == -1)
		return (error(SERROR, SIZE));
	++i;
	while (ft_isspace(s[i]))
		++i;
	if (goto_next_element(line) != '>')
		return (error(SERROR, SIZE));
	return (1);
}

int		parse_camera(char **line, t_data *data)
{
	char	stripe;
	int		ret;
	t_cam	*cam;

	stripe = 0;
	ret = 1;
	if (data->camera)
		return (error(ALREADYCAM, NULL));
	if (!(cam = ft_memalloc(sizeof(t_cam))))
		return (0);
	stripe = goto_next_element(line);
	while (stripe != '>' && ret != 0)
	{
		if (!ft_strncmp_case(*line, "origin", 6))
			ret = parse_origin(line, &cam->origin, 6);
		else if (!ft_strncmp_case(*line, "rotation", 8))
			ret = parse_rotation(line, &cam->rotation, 8);
		else if (stripe == '<')
			return (syn_error(SERROR, CAM, ORIGIN, ROTATION));
		stripe = goto_next_element(line);
	}
	data->camera = cam;
	if (!data->camera || ret == 0)
		return (syn_error(SERROR, CAM, ORIGIN, ROTATION));
	return (ret);
}

int		pick_options(char **line, t_data *data)
{
	int ret;

	ret = 1;
	if (!ft_strncmp_case(*line, "MotionBlur", 10))
		ret = parse_onoff(line, &data->motion_blur);
	else if (!ft_strncmp_case(*line, "Stereoscopy", 11))
		ret = parse_onoff(line, &data->stereoscopy);
	else if (!ft_strncmp_case(*line, "Anti-Alliasing", 14))
		ret = parse_onoff(line, &data->anti_al);
	else if (!ft_strncmp_case(*line, "Cel_Shading", 11))
		ret = parse_onoff(line, &data->cel_shading);
	else if (!ft_strncmp_case(*line, "Indirect_GI", 11))
		ret = parse_onoff(line, &data->indirect_gi);
	else if (!ft_strncmp_case(*line, "Caustics_GI", 11))
		ret = parse_onoff(line, &data->caustics_gi);
	else if (!ft_strncmp_case(*line, "Fog", 3))
		ret = parse_rotation(line, &data->fog, 3);
	else if (!ft_strncmp_case(*line, "ColorFilter", 11))
		ret = parse_color_filter(line, data);
	else if (!ft_strncmp_case(*line, "Skybox", 6))
		ret = parse_name(line, &data->skybox_name, 6);
	else if (**line != '<')
		return (error(UNKNOWSCENE, NULL));
	return (ret);
}

int		check_scene_param(t_data *data)
{
	if (!check_lights_cam(data))
		return (0);
	if (!(check_skybox(data)))
	{
		ft_printf("Invalid skybox image\n");
		return (0);
	}
	return (1);
}

int		parse_scene(char **line, t_data *data)
{
	char	stripe;
	int		ret;

	stripe = 0;
	ret = 1;
	while (stripe != '>' && ret != 0)
	{
		stripe = goto_next_element(line);
		if (!ft_strncmp_case(*line, "name", 4))
			ret = parse_name(line, &data->scene_name, 4);
		else if (!ft_strncmp_case(*line, "size", 4))
			ret = parse_size(line, data);
		else if (!ft_strncmp_case(*line, "camera", 6))
			ret = parse_camera(line, data);
		else if (!ft_strncmp_case(*line, "objects", 7))
			ret = parse_objects(line, data, NULL);
		else if (!ft_strncmp_case(*line, "composed", 8))
			ret = parse_composed_model(line, data);
		else if (!ft_strncmp_case(*line, "lights", 6))
			ret = parse_lights(line, data);
		else if (**line != '>' && !(ret = pick_options(line, data)))
			return (0);
	}
	return (check_scene_param(data) ? ret : 0);
}
