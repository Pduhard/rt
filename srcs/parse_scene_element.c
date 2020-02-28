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

int		pick_spot(char **line, t_light *light)
{
	int ret;

	ret = 1;
	if (!(ft_strncmp_case(*line, "directional", 11)))
	{
		light->light_type = LIGHT_DIRECTIONAL;
		ret = parse_origin(line, &light->param, 11);
		goto_next_element(line);
		if (!(ft_strncmp_case(*line, "color", 5)))
			ret = parse_origin(line, &light->color, 5);
		else
			return (syn_error(SERROR, LIGHT, DIRECTIONAL, NULL));
	}
	else if (!(ft_strncmp_case(*line, "point", 5)))
	{
		light->light_type = LIGHT_POINT;
		ret = parse_origin(line, &light->param, 5);
		goto_next_element(line);
		if (!(ft_strncmp_case(*line, "color", 5)))
			ret = parse_origin(line, &light->color, 5);
		else
			return (syn_error(SERROR, LIGHT, POINT, NULL));
	}
	else
		return (error(UNKNOWLIGHT, NULL));
	return (ret);
}

int		parse_lights(char **line, t_data *data)
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
				return (syn_error(SERROR, LIGHT, AMBIENT, NULL));
		}
		else
		{	
			ret = pick_spot(line, light);
			if (ret == 0)
				return (0);
		}
		/*else if (!(ft_strncmp_case(*line, "directional", 11)))
		{
			light->light_type = LIGHT_DIRECTIONAL;
			ret = parse_origin(line, &light->param, 11);
			goto_next_element(line);
			if (!(ft_strncmp_case(*line, "color", 5)))
				ret = parse_origin(line, &light->color, 5);
			else
				return (syn_error(SERROR, LIGHT, DIRECTIONAL, NULL));
		}
		else if (!(ft_strncmp_case(*line, "point", 5)))
		{
			light->light_type = LIGHT_POINT;
			ret = parse_origin(line, &light->param, 5);
			goto_next_element(line);
			if (!(ft_strncmp_case(*line, "color", 5)))
				ret = parse_origin(line, &light->color, 5);
			else
				return (syn_error(SERROR, LIGHT, POINT, NULL));
		}*/		
	}
	if (ret == 1)
		goto_next_element(line);
	if (data->lights)
	{
		light->next = data->lights;
	}
	else
		light->next = NULL;
	data->lights = light;
	return (ret);
}

int		parse_name(char **line, char **name)
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
	if (*name)
		ft_strdel(name);
	start = ++i;
	while (s[i] && (s[i] != ')' && s[i] != '>'))
		++i;
	if (s[i] != ')')
		return (error(SERROR, NAME));
	*name = ft_strsub(s, start, i - start);
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
		else if (stripe == '<')
			return (syn_error(SERROR, CAM, ORIGIN, ROTATION));
		stripe = goto_next_element(line);
	}
	data->camera = cam;
	if (!data->camera || ret == 0)
	{
		return (syn_error(SERROR, CAM, ORIGIN, ROTATION));
	}
	return (ret);
}

int		parse_objects(char **line, t_data *data, t_composed *from)
{
	char	stripe;
	int		ret;
	int		composed;
	t_obj	*obj;

	composed = 0;
	stripe = 0;
	ret = 1;
	if (!(obj = ft_memalloc(sizeof(t_obj))))
		return (0);
	while (stripe != '>' && ret != 0)
	{
		stripe = goto_next_element(line);
		//native 
		if (!ft_strncmp_case(*line, "cone", 4))
			ret = parse_cone(line, obj);//, data);
		else if (!ft_strncmp_case(*line, "sphere", 6))
			ret = parse_sphere(line, obj);//, data);
		else if (!ft_strncmp_case(*line, "plane", 5))
			ret = parse_plane(line, obj);//, data);
		else if (!ft_strncmp_case(*line, "cylinder", 8))
			ret = parse_cylinder(line, obj);//, data);
		else if (!ft_strncmp_case(*line, "triangle", 8))
			ret = parse_triangle(line, obj);//, data);
		// from eq
		else if (!from && !ft_strncmp_case(*line, "moebius", 7))
			ret = parse_moebius(line, obj);//, data);
		else if (!from && !ft_strncmp_case(*line, "ellipsoid", 9))
			ret = parse_ellipsoid(line, obj);//, data);
		else if (!from && !ft_strncmp_case(*line, "hyperboloid", 11))
			ret = parse_hyperboloid(line, obj);//, data);
		else if (!from && !ft_strncmp_case(*line, "horse_saddle", 12))
			ret = parse_horse_saddle(line, obj);//, data);
		else if (!from && !ft_strncmp_case(*line, "monkey_saddle", 13))
			ret = parse_monkey_saddle(line, obj);//, data);
		else if (!from && !ft_strncmp_case(*line, "cyclide", 7))
			ret = parse_cyclide(line, obj);//, data);
		else if (!from && !ft_strncmp_case(*line, "fermat", 6))
			ret = parse_fermat(line, obj);//, data);
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
		else if (is_composed_object(line, data, &ret))
			composed = 1;
		else if (**line != '<' && **line != '>')
			return (error(UNKNOWOBJECT, NULL));
	}
	clamp_val(&obj->reflection, 0, 1);
	clamp_val(&obj->shininess, 0, 1);
	clamp_val(&obj->refraction, 0, 3);
	clamp_val(&obj->refraction, 0, 2.42);
	if (obj->shininess > 0)
		obj->shininess = exp(11 - 10 * obj->shininess);
	if (composed)
	{
		//free object
	}
	else
	{
		if (!from)
			add_object(obj, data);
		else
			add_component(obj, from);
	}
	return (ret);
}
