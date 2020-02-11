#include "rt.h"

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
		return (return_update(SERRORNAME, 0, 2));
	if (data->scene_name)
		ft_strdel(&data->scene_name);
	start = ++i;
	while (s[i] && (s[i] != ')' && s[i] != '>'))
		++i;
	if (s[i] != ')')
		return (return_update(SERRORNAME, 0, 2));
	data->scene_name = ft_strsub(s, start, i - start);
	++i;
	while (ft_isspace(s[i]))
		++i;
	if (goto_next_element(line) != '>')
		return (return_update(SERRORNAME, 0, 2));
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
		return (return_update(SERRORSIZE, 0, 2));
	++i;
	while (ft_isspace(s[i]))
		++i;
	if (goto_next_element(line) != '>')
		return (return_update(SERRORSIZE, 0, 2));
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
		return (return_update(ALREADYCAM, 0, 2));
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
		return (return_update(SERRORCAM, 0, 2));
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
			return (return_update(UNKNOWOBJECT, 0, 2));
	}
	clamp_val(&obj->reflection, 0, 1);
	clamp_val(&obj->shininess, 0, 1);
	clamp_val(&obj->refraction, 0, 3);
	clamp_val(&obj->refraction, 1, 2.42);
	if (obj->shininess > 0)
		obj->shininess = exp(11 - 10 * obj->shininess);
	return (ret);
}
