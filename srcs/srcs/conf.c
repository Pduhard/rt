/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   conf.c                                           .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: aplat <aplat@student.le-101.fr>            +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2019/12/23 01:19:51 by pduhard-     #+#   ##    ##    #+#       */
/*   Updated: 2020/01/17 23:49:30 by pduhard-    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "rt.h"

int		parse_scene_name(char *line, t_data *data)
{
	int	i;
	int start;

	i = 4;
	while (ft_isspace(line[i]))
		++i;
	if (line[i] != '(')
	{
		ft_printf("Syntax error: name syntax: name(scene_name)\n");
		return (0);
	}
	if (data->scene_name)
		ft_strdel(&data->scene_name);
	start = ++i;
	while (line[i] && line[i] != ')')
		++i;
	if (line[i] != ')')
	{
		ft_printf("Syntax error: name syntax: name(scene_name)\n");
		return (0);
	}
	data->scene_name = ft_strsub(line, start, i - start);
	printf("name : |%s|\n", data->scene_name);
	return (1);
}

int		parse_3vecf(char *line, int i, t_3vecf *vec)
{
	int		cpt;

	cpt = 0;
	//++i;
	if (line[i] == '(')
		i++;
	while (cpt < 3)
	{
		vec->val[cpt++] = ft_atold(&(line[i]));
		while (line[i] && ((line[i] != ',' && cpt < 3) || (line[i] != ')' && cpt == 3)))
			++i;
		if (!line[i])
			return (-1);
		++i;
	}
//	while (line[i] && line[i] != ')')
//		++i;
//	if (line[i] != ')')
//		return (-1);
	return (i);
}

int		parse_camera(char *line, t_data *data)
{
	int	i;
	t_cam	*cam;

	if (!(cam = malloc(sizeof(t_cam))))
		return (0);
	i = 6;
	while (ft_isspace(line[i]))
		++i;
	if (line[i] != '(' || (i = parse_3vecf(line, i, &cam->origin)) == -1)
	{
		ft_printf("Syntax error: camera syntax: camera(origin)(rotation)\n");
		return (0);
	}
	while (ft_isspace(line[i]))
		++i;
	if (line[i] != '(' || (i = parse_3vecf(line, i, &cam->rotation)) == -1)
	{
		ft_printf("Syntax error: camera syntax: camera(origin)(rotation)\n");
		return (0);
	}
	if (data->camera)
	{
		//multi camm ?? 
		;
	}
	printf("camval: %f %f %f && %f %f %f\n", cam->origin.val[0], cam->origin.val[1], cam->origin.val[2], cam->rotation.val[0], cam->rotation.val[1], cam->rotation.val[2]);
	data->camera = cam;
	return (1);
}

int		parse_double(char *line, int i, double *val)
{
	if (line[i] == '(')
		i++;
	*val = ft_atold(&(line[i]));
	while (line[i] && line[i] != ')')
			++i;
	if (!line[i])
		return (-1);
	return (i + 1);
}
/*
int		parse_sphere(char *line, t_data *data)
{
	int			i;
	t_obj		*sphere;
	t_sphere	*sphere_param;

	if (!(sphere = malloc(sizeof(t_obj))) || !(sphere_param = malloc(sizeof(t_sphere))))
		return (0);
	i = 6;
	while (ft_isspace(line[i]))
		++i;
	if (line[i] != '(' || (i = parse_3vecf(line, i, &sphere_param->origin)) == -1)
	{
		ft_printf("Syntax error: sphere syntax: sphere(origin)(radius)(color)\n");
		return (0);
	}
	while (ft_isspace(line[i]))
		++i;
	if (line[i] != '(' || (i = parse_double(line, i, &sphere_param->radius)) == -1)
	{
		ft_printf("Syntax error: sphere syntax: sphere(origin)(radius)(color)\n");
		return (0);
	}
	while (ft_isspace(line[i]))
		++i;
	if (line[i] != '(' || (i = parse_3vecf(line, i, &sphere->color)) == -1)
	{
		ft_printf("Syntax error: sphere syntax: sphere(origin)(radius)(color)\n");
		return (0);
	}
	printf("sphere : %f %f %f && %f && %f %f %f\n", sphere_param->origin.val[0], sphere_param->origin.val[1], sphere_param->origin.val[2], sphere_param->radius, sphere->color.val[0], sphere->color.val[1], sphere->color.val[2]);
	sphere->obj_param = sphere_param;
	sphere->obj_type = OBJ_SPHERE;
	if (data->objs)
	{
		sphere->next = data->objs;
	}
	else
		sphere->next = NULL;
	data->objs = sphere;
	return (1);
}
*/
/*
int		parse_plane(char *line, t_data *data)
{
	int			i;
	t_obj		*plane;
	t_plane		*plane_param;

	if (!(plane = malloc(sizeof(t_obj))) || !(plane_param = malloc(sizeof(t_plane))))
		return (0);
	i = 5;
	while (ft_isspace(line[i]))
		++i;
	if (line[i] != '(' || (i = parse_3vecf(line, i, &plane_param->origin)) == -1)
	{
		ft_printf("Syntax error: plane syntax: plane(origin)(normal)(color)\n");
		return (0);
	}
	while (ft_isspace(line[i]))
		++i;
	if (line[i] != '(' || (i = parse_3vecf(line, i, &plane_param->normal)) == -1)
	{
		ft_printf("Syntax error: plane syntax: plane(origin)(normal)(color)\n");
		return (0);
	}
	while (ft_isspace(line[i]))
		++i;
	if (line[i] != '(' || (i = parse_3vecf(line, i, &plane->color)) == -1)
	{
		ft_printf("Syntax error: plane syntax: plane(origin)(normal)(color)\n");
		return (0);
	}
	printf("plane : %f %f %f && %f %f %f && %f %f %f\n", plane_param->origin.val[0], plane_param->origin.val[1], plane_param->origin.val[2], plane_param->normal.val[0], plane_param->normal.val[1], plane_param->normal.val[2] , plane->color.val[0], plane->color.val[1], plane->color.val[2]);
	plane->obj_param = plane_param;
	plane->obj_type = OBJ_PLANE;
	if (data->objs)
	{
		plane->next = data->objs;
	}
	else
		plane->next = NULL;
	data->objs = plane;
	return (1);
}
*/
int		parse_light(char *line, t_data *data)
{
	int			i;
	t_light		*light;

	if (!(light = malloc(sizeof(t_light))))
		return (0);
	i = 5;
	while (ft_isspace(line[i]))
		++i;
	if (line[i++] != '(')
	{
		ft_printf("Syntax error: light syntax: light(light_type)(origin)(intensity)(color)\n");
		return (0);
	}
	printf("%s, \n", &(line[i]));
	if (!ft_strncmp(&(line[i]), "point", 5))
	{
		light->light_type = LIGHT_POINT;
		i += 5;
	}
	else if (!ft_strncmp(&(line[i]), "ambient", 7))
	{
		light->light_type = LIGHT_AMBIENT;
		i += 7;
	}
	else if (!ft_strncmp(&(line[i]), "directional", 11))
	{
		light->light_type = LIGHT_DIRECTIONAL;	
		i+=11;
	}
	else
	{
		ft_printf("Syntax error: light syntax: light(light_type)(origin)(intensity)(color)\n");
		return (0);
	}
	while (ft_isspace(line[i]))
		++i;
	if (line[i++] != ')')
	{
		ft_printf("Syntax error: light syntax: light(light_type)(origin)(intensity)(color)\n");
		return (0);
	}
	while (ft_isspace(line[i]))
		++i;
	if (line[i] != '(' || (i = parse_3vecf(line, i, &light->origin)) == -1)
	{
		ft_printf("Syntax error: light syntax: light(light_type)(origin)(intensity)(color)\n");
		return (0);
	}
	while (ft_isspace(line[i]))
		++i;
	if (line[i] != '(' || (i = parse_3vecf(line, i, &light->intensity)) == -1)
	{
		ft_printf("Syntax error: light syntax: light(light_type)(origin)(intensity)(color)\n");
		return (0);
	}
	while (ft_isspace(line[i]))
		++i;
	if (line[i] != '(' || (i = parse_3vecf(line, i, &light->color)) == -1)
	{
		ft_printf("Syntax error: light syntax: light(light_type)(origin)(intensity)(color)\n");
		return (0);
	}
	printf("light : %f %f %f && %f %f %f && %f %f %f\n", light->origin.val[0], light->origin.val[1], light->origin.val[2], light->intensity.val[0], light->intensity.val[1], light->intensity.val[2], light->color.val[0], light->color.val[1], light->color.val[2]);
	//sphere->obj_param = sphere_param;
	//sphere->obj_type = OBJ_SPHERE;
	if (data->lights)
	{
		light->next = data->lights;
	}
	else
		light->next = NULL;
	data->lights = light;
	return (1);
}

int		parse_rt_line(char *line, t_data *data)
{
	printf("%s\n", line);
	if (!ft_strncmp(line, "name", 4))
		return (parse_scene_name(line, data));
	else if (!ft_strncmp(line, "camera", 6))
		return (parse_camera(line, data));
	else if (!ft_strncmp(line, "light", 5))
		return (parse_light(line, data));
	else if (!ft_strncmp(line, "sphere", 6))
		return (parse_sphere(line, data));
	else if (!ft_strncmp(line, "plane", 5))
		return (parse_plane(line, data));
	else if (!ft_strncmp(line, "cone", 4))
		return (parse_cone(line, data));
	else if (!ft_strncmp(line, "cylinder", 8))
		return (parse_cylinder(line, data));
	else
	{
		ft_printf("Unrecognized element: \n%s\n", line);
		return (0);
	}
	data->objs->refraction = 0; // to delete..
	//printf("%s\n", line);
	return (1);
}

int		parse_rt_conf(char *file_name, t_data *data)
{
	int		fd;
	char	*line = NULL;
	int		ret;

	if ((fd = open(file_name, O_RDONLY)) == -1)
	{
		ft_printf("%s: invalid rt_conf file\n", file_name);
		return (0);
	}
	while ((ret = get_next_line(fd, &line)) > 0)
	{
		if (!parse_rt_line(line, data))
		{
			ft_strdel(&line);
			return (0);
		}
		ft_strdel(&line);
	}
	if (ret == -1)
	{
		ft_printf("%s: invalid rt_conf file\n", file_name);
		return (0);
	}
	return (1);
}
