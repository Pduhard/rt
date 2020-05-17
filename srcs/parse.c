#include "rt.h"

static void	check_line(char **line)
{
	int		i;
	char	*s;
	char	*tmp;

	i = 0;
	s = *line;
	while (s[i])
	{
		if (!(ft_strncmp(&s[i], "//", 2)))
		{
			tmp = ft_strsub(*line, 0, i);
			free(*line);
			*line = tmp;
			return ;
		}
		i++;
	}
}

static int		brackets_rt(char *line)
{
	int	i;
	int	cmp;

	i = 0;
	cmp = 0;
	while (line[i] != '<' && line[i] != '\0')
		i++;
	if (line[i] == '\0')
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

static int		parse(char *line, t_data *data)
{
	goto_next_element(&line);
	if (!(ft_strncmp_case(line, "scene", 5)))
		return (parse_scene(&line, data));
	else
		return (error(ERRORSCENE, NULL));
	return (0);
}

int		parse_rt_conf(char *file_name, t_data *data)
{
	int		fd;
	char	*line;
	int		ret;
	char	*result;

	line = NULL;
	if ((fd = open(file_name, O_RDONLY)) == -1)
		return (error(ERRORARG, NULL));
	result = NULL;
	while ((ret = get_next_line(fd, &line)) > 0)
	{
		check_line(&line);
		result = ft_strfjoin(result, line);
	}
	if (ret == -1 || !result || !brackets_rt(result) || !parse(result, data))
	{
		ft_strdel(&result);
		return (error(ERRORFILE, NULL));
	}
	ft_strdel(&result);
	return (1);
}
