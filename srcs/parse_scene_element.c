#include "rt.h"

int	check_lights(t_data *data)
{
	t_light *light;
	t_light *tmp;

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
	return (1);
}

int		parse_scene_name(char **line, t_data *data)
{
	int		i;
	int		start;
	char	*s;

	i = 4;
	s = *line;
	while (ft_isspace(s[i]))
		++i;
	if (s[i] != '(')
		return (error(SERROR, NAME));
	if (data->scene_name)
		ft_strdel(&data->scene_name);
	start = ++i;
	while (s[i] && (s[i] != ')' && s[i] != '>'))
		++i;
	if (s[i] != ')')
		return (error(SERROR, NAME));
	data->scene_name = ft_strsub(s, start, i - start);
	++i;
	while (ft_isspace(s[i]))
		++i;
	if (goto_next_element(line) != '>')
		return (error(SERROR, NAME));
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
		stripe = goto_next_element(line);
	}
	data->camera = cam;
	if (!data->camera || ret == 0)
	{
		return (error(SERROR, ORIGIN));
	}
	return (ret);
}

int		parse_objects(char **line, t_data *data)
{
	char	stripe;
	int		ret;
	t_obj	*obj;

	stripe = 0;
	ret = 1;
	if (!(obj = ft_memalloc(sizeof(t_obj))))
		return (0);
	while (stripe != '>' && ret != 0)
	{
		stripe = goto_next_element(line);
		if (!ft_strncmp_case(*line, "cone", 4))
			ret = parse_cone(line, obj, data);
		else if (!ft_strncmp_case(*line, "sphere", 6))
			ret = parse_sphere(line, obj, data);
		else if (!ft_strncmp_case(*line, "plane", 5))
			ret = parse_plane(line, obj, data);
		else if (!ft_strncmp_case(*line, "rect", 4))
			ret = parse_rect(line, obj, data);
		else if (!ft_strncmp_case(*line, "cylinder", 8))
			ret = parse_cylinder(line, obj, data);
		else if (!ft_strncmp_case(*line, "moebius", 7))
			ret = parse_moebius(line, obj, data);
		else if (!ft_strncmp_case(*line, "texture", 7))
			ret = parse_texture2(line, obj);
		else if (!(ft_strncmp_case(*line, "cutting", 7)))
			ret = parse_cutting(line, obj);
		else if (!(ft_strncmp_case(*line, "MotionBlur", 10)))
			ret = parse_motion(line, obj);
		else if (!ft_strncmp_case(*line, "reflection", 10))
			ret = parse_double2(line, 10, &obj->reflection);
		else if (!ft_strncmp_case(*line, "refraction", 10))
			ret = parse_double2(line, 10, &obj->refraction);
		else if (!ft_strncmp_case(*line, "shininess", 9))
			ret = parse_double2(line, 9, &obj->shininess);
		else if (!ft_strncmp_case(*line, "material", 8))
			ret = parse_material(line, 8, obj);
		else if (**line != '<')
			return (error(UNKNOWOBJECT, NULL));
	}
	clamp_val(&obj->reflection, 0, 1);
	clamp_val(&obj->shininess, 0, 1);
	clamp_val(&obj->refraction, 0, 3);
	clamp_val(&obj->refraction, 0, 2.42);
	if (obj->shininess > 0)
		obj->shininess = exp(11 - 10 * obj->shininess);
	return (ret);
}
