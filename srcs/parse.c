#include "rt.h"

void	check_line(char **line)
{
	int	i;
	char *s;

	i = 0;
	s = *line;
	while (s[i])
	{
		if (!(ft_strncmp(&s[i], "//", 2)))
			*line = ft_strsub(*line, 0, i);
		i++;
	}
}

int		parse_rt_conf(char *file_name, t_data *data)
{
	int		fd;
	char	*line;
	int		ret;
	char	*result;
	char	*result_cpy;

	line = NULL;
	if ((fd = open(file_name, O_RDONLY)) == -1)
		return (error(ERRORARG, NULL));
	result = NULL;
	while ((ret = get_next_line(fd, &line)) > 0)
	{
		check_line(&line);
		result = ft_strfjoin(result, line);
	}
	if (ret == -1 || result == '\0' || brackets_rt(result) == 0)
	{
		free(result);
		return (error(ERRORFILE, NULL));
	}
	result_cpy = result;
	if (!parse(&result, data))
		return (error(ERRORFILE, NULL));
	return (1);
}

int		brackets_rt(char *line)
{
	int	i;
	int	cmp;

	i = 0;
	cmp = 0;
	while (line[i] != '<' && line[i] != '\0')
		i++;
	if (line[i] == '\0' && i == 0)
		return (error(ERROREMPTY, NULL));
	cmp++;
	i++;
	while (cmp > 0 && line[i] != '\0')
	{
		if (line[i] == '<')
			cmp++;
		if (line[i] == '>')
			cmp--;
		i++;
	}
	if ((cmp == 0 && line[i] != '\0') || cmp != 0)
		return (error(ERRORSTRIPE, NULL));
	return (1);
}

int		parse(char **line, t_data *data)
{
	char	stripe;

	stripe = goto_next_element(line);
	if (!(ft_strncmp_case(*line, "scene", 5)))
		return (parse_scene(line, data));
	else
		return (error(ERRORSCENE, NULL));
	return (0);
}

int		parse_scene(char **line, t_data *data)
{
	char	stripe;
	int		ret;

	stripe = 0;
	ret = 1;

	stripe = goto_next_element(line);
	while (stripe != '>' && ret != 0)
	{
		if (!ft_strncmp_case(*line, "name", 4))
			ret = parse_scene_name(line, data);
		else if (!ft_strncmp_case(*line, "size", 4))
			ret = parse_size(line, data);
		else if (!ft_strncmp_case(*line, "camera", 6))
			ret = parse_camera(line, data);
		else if (!ft_strncmp_case(*line, "objects", 7))
			ret = parse_objects(line, data);
		else if (!ft_strncmp_case(*line, "lights", 6))
			ret = parse_lights(line, data);
		else if (!ft_strncmp_case(*line, "MotionBlur", 10))
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
		else
			return (error(UNKNOWSCENE, NULL));
		if (ret == 0)
			return (ret);
		stripe = goto_next_element(line);
	}
	check_lights(data);
	if (!data->camera)
		return (error(ERRORCAM, NULL));
	return (ret);
}
