/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   conf.c                                           .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: pduhard- <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2019/12/22 23:34:59 by pduhard-     #+#   ##    ##    #+#       */
/*   Updated: 2020/01/16 01:11:45 by pduhard-    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "rt.h"

int		read_next_elem(int fd, char **line, int i)
{
	while (1)
	{
		if (line[0][i] == '#' || line[0][i] == '\0')
		{
			ft_strdel(line);
			if (get_next_line(fd, line) < 0)
				return (-1);
			i = 0;	
		}
		else if (ft_isspace(line[0][i]))
			i++;
		else
			return (i);
	}
}

int		read_in_elem(int fd, char **line, int i)
{
	while (line[0][i] != '{')
	{
		if (line[0][i] == '#' || line[0][i] == '\0')
		{
			ft_strdel(line);
			if (get_next_line(fd, line) < 0)
				return (-1);
			i = 0;	
		}
		else if (ft_isspace(line[0][i]))
			i++;
		else
			return (-1);
	}
	return (read_next_elem(fd, line, i + 1));
}

int	parse_name(int fd, t_data *data, char **line, int i)
{
	int	start;
	int	end;
	char	*name;

	name = NULL;
	start = read_in_elem(fd, line, i + 4);
	len = start;
	i = len
	while (line[0][i] != '}')
	{
		++i;
		if (!line[0][i])
		{
			name = ft_strfjoin(name, ft_strsub(*line, start, len - i));
			ft_strdel(line);
			if (get_next_line(fd, line) < 0)
			{
				ft_strdel(&name);
				return (-1);
			}
			start = 0;
			i = 0;
		}
	}
	end = i;
	--i;
	while (ft_isspace(line[0][i]))
		--i;
	if (data->scene_name)
		ft_strdel(&data->scene_name);
	data->scene_name = ft_strsub(*line, start, len - start);
	ft_printf("scene_name : %s\n", scene_name);
	if (!line[0][len])
		return (-1);
	return (end + 1);
}

int		parse_scene(int fd, t_data *data, char **line)
{
	int		i;

	i = read_next_elem(fd, line, 0);
	if (i == -1 || ft_strncmp(&(line[0][i]), "scene", 6))
	{
		ft_printf("Rt_conf file should start with a scene{} element\n");
		return (0);
	}
	i = read_in_elem(fd, line, i + 5);
	while (line[0][i] != '}')
	{
		if (!ft_strncmp(&(line[0][i]), "name", 4))
		{
			i = parse_name(fd, data, line, i);
			if (i == -1)
				ft_printf("invalid")
			
				
		}
	}
	printf("%s\n", &(line[0][i]));
//	{
		
//	}
//	skip_comments
	(void)data;
	return (1);
}

int		parse_rt_conf(char *file_name, t_data *data)
{
	int		fd;
	char	*line = NULL;

	if ((fd = open(file_name, O_RDONLY)) == -1)
	{
		ft_printf("%s: invalid rt_conf file\n", file_name);
		return (0);
	}
	if (get_next_line(fd, &line) < 0)
	{
		ft_printf("%s: invalid rt_conf file\n", file_name);
		return (0);
	}
	if (!parse_scene(fd, data, &line))
		return (0);
	return (1);
}
