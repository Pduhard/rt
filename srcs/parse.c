#include "rt.h"

int		parse_rt_conf(char *file_name, t_data *data)
{
	int		fd;
	char	*line;
	int		ret;
	char	*result;
	char	*result_cpy;

	line = NULL;
	if ((fd = open(file_name, O_RDONLY)) == -1)
		return (return_update(ERRORARG, 0, 2));
	result = NULL;
	while ((ret = get_next_line(fd, &line)) > 0)
		result = ft_strfjoin(result, line);
	if (ret == -1 || result == '\0' || brackets_rt(result) == 0)
	{
		free(result);
		return (return_update(ERRORFILE, 0, 2));
	}
	result_cpy = result;
	if (!parse(&result, data))
		return (return_update(ERRORFILE, 0, 2));
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
		return (return_update(ERROREMPTY, 0, 2));
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
		return (return_update(ERRORSTRIPE, 0, 2));
	return (1);
}

int		parse(char **line, t_data *data)
{
	char	stripe;

	stripe = goto_next_element(line);
	if (!(ft_strncmp_case(*line, "scene", 5)))
		return (parse_scene(line, data));
	else
		return (return_update(ERRORSCENE, 0, 2));
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
		else if (!ft_strncmp_case(*line, "ColorFilter", 11))
			ret = parse_color_filter(line, data);
		else
			return (return_update(UNKNOWSCENE, 0, 2));
		if (ret == 0)
			return (ret);
		stripe = goto_next_element(line);
	}
	if (!data->camera)
		return (return_update(ERRORCAM, 0, 2));
	return (ret);
}