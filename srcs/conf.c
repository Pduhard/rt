#include "rt.h"

/*int		parse_scene_name(char *line, t_data *data)
{
	int	i;
	int start;

	i = 4;
	printf("Scene name ==> %s\n", line);
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
}*/



/*int		parse_camera(char *line, t_data *data)
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
}*/

/*int		parse_light(char *line, t_data *data)
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
*/
/*int		parse_rt_line(char *line, t_data *data)
{
	printf("%s\n", line);
	line = ft_strchr(line, '<');
	printf("%s\n", line);
	line++;
	if (!ft_strncmp(line, "scene", 5))
		return (parse_scene(&line, data));
	else if (!ft_strncmp(line++, "name", 4))
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
	(void)data;
	return (1);
}*/

int		parse_rt_conf(char *file_name, t_data *data)
{
	int		fd;
	char	*line = NULL;
	int		ret;
	char	*result;
	char	*result_cpy;

	if ((fd = open(file_name, O_RDONLY)) == -1)
	{
		ft_printf("%s: invalid rt_conf file\n", file_name);
		return (0);
	}
	result = NULL;
	while ((ret = get_next_line(fd, &line)) > 0)
		result = ft_strfjoin(result, line);
	if (ret == -1 || result == '\0' || brackets_rt(result) == 0)
	{
		free(result);
		ft_printf("%s: invalid rt_conf file\n", file_name);
		return (0);
	}
	result_cpy = result;
	if(!parse(&result, data))
	{
		printf("Bad File ==> %s\n", result);
		return (0);
	}
	else
		print_conf(data);
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
	if (line[i] == '\0')
		return (0);
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
		return (0);
	return (1);
}

int		parse(char **line, t_data *data)
{
	char	stripe;

	stripe = goto_next_element(line);
	if (!(ft_strncmp(*line, "scene", 5)))
	{
		printf("Check\n");
		return (parse_scene(line, data));
	}
	printf("Check 2\n");
	return (0);
}

int		parse_onoff(char **line, int *onoff)
{
	char	*s;

	s = *line;

	while (*s != '(' && *s)
		++s;
	if (!ft_strncmp(s, "(ON)", 4))
		*onoff = 1;
	else if (!ft_strncmp(s, "(OFF)", 5))
		*onoff = 0;
	else if (!ft_strncmp(s, "(1)", 3))
		*onoff = 1;
	else if (!ft_strncmp(s, "(0)", 3))
		*onoff = 0;
	else
	{
		ft_printf("ON OFF possible value: ON/1, OFF/0\n");
		return (0);
	}
	goto_next_element(line);
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
		printf("Parse_scene ==> %s\n\n", *line);
		stripe = goto_next_element(line);
		printf("Parse_scene next ==> %s\n\n", *line);
		if (!ft_strncmp(*line, "name", 4))
			ret = parse_scene_name(line, data);
		else if (!ft_strncmp(*line, "size", 4))
			ret = parse_size(line, data);
		else if (!ft_strncmp(*line, "camera", 6))
			ret = parse_camera(line, data);
		else if (!ft_strncmp(*line, "objects", 7))
			ret = parse_objects(line, data);
		else if (!ft_strncmp(*line, "lights", 6))
			ret = parse_lights(line, data);
		else if (!ft_strncmp(*line, "MotionBlur", 10))
			ret = parse_onoff(line, &data->motion_blur);
/*		else
		{
			printf("Unrecognized element\n");
			return (0);
		}*/
	}
	return (ret);
}

int		parse_lights(char **line, t_data *data)
{
	char	stripe;
	int		ret;
	t_light	*light;

	stripe = 0;
	ret = 1;
/*	if (light->param)
	{
		printf("\n Fais IECH Lights ==> %s\n", *line);
		return (0);
	}*/
	if (!(light = malloc(sizeof(t_light))))
		return (0);
	while (stripe != '>' && ret != 0)
	{
		stripe = goto_next_element(line);
		if (!(ft_strncmp(*line, "ambient", 7)))
		{
			light->light_type = LIGHT_AMBIENT;
			ret = parse_origin(line, &light->color, 7);
			printf("Parse_Lights ==> %s\n", *line);
			if (**line == '<')
			{
				printf("ambient rien apres\n");
				return (0);
			}
		}
		else if (!(ft_strncmp(*line, "directional", 11)))
		{
			light->light_type = LIGHT_DIRECTIONAL;
			ret = parse_origin(line, &light->param, 11);
			goto_next_element(line);
			if (!(ft_strncmp(*line, "color", 5)))
				ret = parse_origin(line, &light->color, 5);
			else
			{
				printf("directional ==> %s\n", *line);
				return (0);
			}
		}
		else if (!(ft_strncmp(*line, "point", 5)))
		{
			light->light_type = LIGHT_POINT;
			ret = parse_origin(line, &light->param, 5);
			goto_next_element(line);
			printf("pointsisi ==> %s\n", *line);
			if (!(ft_strncmp(*line, "color", 5)))
				ret = parse_origin(line, &light->color, 5);
			else
			{
				printf("point ==> %s\n", *line);
				return (0);
			}
		}
	}
	if (data->lights)
	{
		light->next = data->lights;
	}
	else
		light->next = NULL;
	data->lights = light;
	return (ret);
}

/*int		parse_motion(char **line, t_obj *obj)
{
	char	stripe;
	int		ret;
	t_motion *motion;

	stripe = 0;
	ret = 1;
	if (!(motion = ft_memalloc(sizeof(t_motion))))
		return (0);
	while (stripe != '>' && ret != 0)
	{
		stripe = goto_next_element(line);
		if (!(ft_strncmp(*line, "dir", 3)))
		{
			ret = parse_origin(line, &motion->dir, 3);
			goto_next_element(line);
			printf("Parse_moyion ==> %s\n", *line);
			if (!(ft_strncmp(*line, "speed", 5)))
				ret = parse_double2(line, 5, &motion->speed_fact);
			else if (**line != '>')
			{
				printf("Bad conf Motion\n");
				return (0);
			}
		}
		else if (**line != '<')
		{
			printf("Parse_motion ==> %s\n", *line);
			printf("Bad conf 2 Motion\n");
			return (0);
		}
	}
	if (obj->motions)
		motion->next = obj->motions;
	else
		motion->next = NULL;
	obj->motions = motion;
	return (ret);
}
*/

void	push_front_motion(t_motion **root, t_motion *new)
{
	t_motion	*motion;

	if (!*root)
		*root = new;
	else
	{
		motion = *root;
		while (motion->next)
			motion = motion->next;
		motion->next = new;
	}
}

int		parse_motion(char **line, t_obj *obj)
{
	char	stripe;
	int		ret;
	t_motion *motion;

	stripe = 0;
	ret = 1;
	if (!(motion = ft_memalloc(sizeof(t_motion))))
		return (0);
	while (stripe != '>' && ret != 0)
	{
		stripe = goto_next_element(line);
		if (!(ft_strncmp(*line, "dir", 3)))
			ret = parse_origin(line, &motion->dir, 3);
		else if (!(ft_strncmp(*line, "speed", 3)))
			ret = parse_double2(line, 5, &motion->speed_fact);
		else if (!(ft_strncmp(*line, "spf", 3)))
		{
			ret = parse_int(line, 3, &motion->spf);
			printf("\n\n%s\n", *line);
		}
	/*	else if (**line != '<' && **line != '>') //ie end of element
		{
			printf("Parse_motion ==> %s\n", *line);
			printf("Bad conf 2 Motion\n");
			return (0);
		}*/
	}
	push_front_motion(&obj->motions, motion);
	return (ret);
}


/*int		parse_ambient(char **line, t_light *light, t_data *data)
{
	char	stripe;
	int		ret;

	stripe = 0;
	ret = 1;
	printf("\nLight Ambient ==> %s\n", *line);
	if (light->color)
	{
		printf("\n Fais IECH Light ambient ==> %s\n", *line);
		return (0);
	}
	while (stripe != '>' && ret != 0)
	{
		stripe = goto_next_element(line);
		if (!ft_strncmp(*line, "color", 5))
			ret = parse_origin(line, &light->color, 5);
		if (!ft_strncmp(*line, "origin", 6))
			ret = parse_origin(line, &light->origin, 6);
		if (!ft_strncmp(*line, "dir", 3))
			ret = parse_origin(line, &light->dir, 3);
	}
	light->light_type = LIGHT_AMBIENT;
	if (data->lights)
	{
		light->next = data->lights;
	}
	else
		light->next = NULL;
	data->lights = light;
	printf("Fin Light Ambient ==> %s\n", *line);
	print_vec(light->color.val);
	print_vec(light->origin.val);
	print_vec(light->dir.val);
	return (ret);
}*/

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
		printf("Parse_Object ==> %s\n", *line);
		if (!ft_strncmp(*line, "cone", 4))
			ret = parse_cone(line, obj, data);
		else if (!ft_strncmp(*line, "sphere", 6))
			ret = parse_sphere(line, obj, data);
		else if (!ft_strncmp(*line, "plane", 5))
			ret = parse_plane(line, obj, data);
		else if (!ft_strncmp(*line, "cylinder", 8))
			ret = parse_cylinder(line, obj, data);
		else if (!ft_strncmp(*line, "moebius", 7))
			ret = parse_moebius(line, obj, data);
		else if (!ft_strncmp(*line, "texture", 7))
			ret = parse_texture2(line, obj);
		else if (!(ft_strncmp(*line, "cutting", 7)))
			ret = parse_cutting(line, obj);
		else if (!(ft_strncmp(*line, "MotionBlur", 10)))
			ret = parse_motion(line, obj);
		else if (!ft_strncmp(*line, "reflection", 10))
			ret = parse_double2(line, 10, &obj->reflection);
		else if (!ft_strncmp(*line, "refraction", 10))
			ret = parse_double2(line, 10, &obj->refraction);
		else if (!ft_strncmp(*line, "shininess", 9))
			ret = parse_double2(line, 9, &obj->shininess);
		else if (**line != '<')
		{
			ft_printf("Unrecognized element: \n%s\n", *line);
			return (0);
		}
	}
	return (ret);
}

int		parse_cut_static_texture(char **line, t_obj *cut)
{
	char	stripe;

	cut->obj_type = OBJ_CUT_TEXTURE;
	stripe = goto_next_element(line);
	if (stripe != '>')
		return (0);
	return (1);
}

int		parse_cutting(char **line, t_obj *obj)
{
	char	stripe;
	int		ret;
	t_obj	*cut;

	stripe = 0;
	ret = 1;
	printf("Parse Cutting\n");
	if (!(cut = ft_memalloc(sizeof(t_obj))))
		return (0);
	while (stripe != '>' && ret != 0)
	{
		stripe = goto_next_element(line);
		if (!(ft_strncmp(*line, "static", 6)))
			ret = parse_cut_static_real(line, cut);
		else if (!(ft_strncmp(*line, "real", 4)))
			ret = parse_cut_static_real(line, cut);
//		else if (!(ft_strncmp(*line, "plane", 5)))
//			ret = parse_cut_static_plane(line, cut, obj);
		else if (!(ft_strncmp(*line, "texture", 7)))
			ret = parse_cut_static_texture(line, cut);
	
		/*		if (!(cut->cut_param))
		{
			ft_printf("Unrecognized element in Cutting: \n%s\n", *line);
			return (0);
		}*/
	}
	cut->ray_intersect = &ray_intersect_plane;
	cut->get_normal_inter = &get_normal_intersect_plane;
	cut->get_origin = &get_origin_plane;
	cut->move = &move_plane;
	cut->get_text_coordinate = &get_text_coordinate_plane;
	cut->text = obj->text;
	cut->get_text_color = obj->get_text_color;
	cut->get_bump_mapping = obj->get_bump_mapping;
	cut->reflection = obj->reflection;
	cut->refraction = obj->refraction;
	cut->shininess = obj->shininess;
	if (obj->cuts)
		cut->next = obj->cuts;
	else
		cut->next = NULL;
	obj->cuts = cut;
	return (ret);
}


int		parse_cut_static_real(char **line, t_obj *cut)
{
	char	stripe;
	int		ret;
	t_plane	*param;

	stripe = 0;
	ret = 1;
	cut->obj_type = OBJ_PLANE;
	if (cut->obj_param)
	{
		printf("Deja parametre\n");
		return (0);
	}
	if (!(param = ft_memalloc(sizeof(t_plane))))
		return (0);
	while (stripe != '>' && ret != 0)
	{
		stripe = goto_next_element(line);
		if (!(ft_strncmp(*line, "origin", 6)))
			ret = parse_origin(line, &param->origin, 6);
		else if (!(ft_strncmp(*line, "normal", 6)))
			ret = parse_origin(line, &param->normal, 6);
		printf("CUTTING ==> %s\n", *line);
	}
	cut->obj_param = param;
/*	if (obj->cuts)
		cut->next = obj->cuts;
	else
		cut->next = NULL;
	obj->cuts = cut;
*/	printf ("Parse Cutting\nOrigin ==> %f %f %f\n", param->origin.val[0], param->origin.val[1], param->origin.val[2]);
	return (ret);
}

int		parse_texture2(char **line, t_obj *obj)
{
	char	stripe;
	int		ret;

	stripe = 0;
	ret = 1;
	if (obj->text.text_param)
	{
		ft_printf("Remet pas une texture debile\n==> %s\n", *line);
		return (0);
	}
	while (stripe != '>' && ret != 0)
	{
		stripe = goto_next_element(line);
		printf("Parse_Texture ==> %s\n", *line);
		if (!(ft_strncmp(*line, "UNI", 3)))
		{
			obj->get_text_color = &get_uni_color;
			obj->text.text_type = TEXT_UNI;
			ret = obj->text.text_param ? 0 : 1;
		}
		else if (!(ft_strncmp(*line, "GRID", 4)))
		{
			obj->get_text_color = &get_grid_color;
			obj->text.text_type = TEXT_GRID;
			ret = obj->text.text_param ? 0 : 1;
		}
		else if (!(ft_strncmp(*line, "PERLIN", 6)))
		{
			obj->get_text_color = &get_perlin_color;
			obj->text.text_type = TEXT_PERLIN;
			ret = obj->text.text_param ? 0 : 1;
		}
		else if (!(ft_strncmp(*line, "MARBLE", 6)))
		{
			obj->get_text_color = &get_marble_color;
			obj->text.text_type = TEXT_MARBLE;
			ret = obj->text.text_param ? 0 : 1;
		}
		else if (!(ft_strncmp(*line, "WOOD", 4)))
		{
			obj->get_text_color = &get_wood_color;
			obj->text.text_type = TEXT_WOOD;
			ret = obj->text.text_param ? 0 : 1;
		}
		else if (!(ft_strncmp(*line, "IMAGE", 5)))
		{
			obj->get_text_color = &get_image_color;
			obj->text.text_type = TEXT_IMAGE;
			ret = obj->text.text_param ? 0 : 1;
		}
		else if (!(ft_strncmp(*line, "offset", 6)))
			ret = parse_rotation(line, &obj->text.offset, 6);
		else if (!(ft_strncmp(*line, "scale", 5)))
			ret = parse_rotation(line, &obj->text.scale, 5);
		else if (!(ft_strncmp(*line, "BumpMapping", 11)) && obj->text.text_param)
			ret = parse_bump_mapping(line, obj);
//		else
//		{
//			ft_printf("C'est pas un nom de texture gros Con\n==> %s\n", *line);
//			return (0);
//		}
		if (obj->text.text_type == TEXT_IMAGE && !(obj->text.text_param))
		{
			if (!(obj->text.text_param = parse_img(line, &obj->text)))
			{
				ft_printf("Ton image c'est de la merde\n");
				return (0);
			}
		}
		else if (!(obj->text.text_param))
		{
			if (!(obj->text.text_param = parse_proc(line, &obj->text)))
			{
				ft_printf("Ta texture c'est de la merde\n");
				return (0);
			}
		}
	}
	return (ret);
}

int		parse_bump_mapping(char **line, t_obj *obj)
{
//	t_text *text;
	char	stripe;
	int		ret;

//	text = obj
	ret = 1;
	stripe = 0;
	while (stripe != '>' && ret != 0)
	{
		printf("Parse_BUMP BEFORE ==> %s\n", *line);
		stripe = goto_next_element(line);
		printf("Parse_BUMP ==> %s\n", *line);
		if (!(ft_strncmp(*line, "own", 3)))
		{
			set_bump_own(obj);
			ret = parse_double2(line, 3, &obj->text.bump_fact);
		}
		else if (!(ft_strncmp(*line, "independent", 11)))
		{
			ret = parse_bump_inde(line, obj, 11);
		}
/*		else
		{
			ft_printf("PAs un bon param de bumpmapping ==> %s\n", *line);
			return (0);
		}*/
	}
	return (ret);
}

void	set_bump_own(t_obj *obj)//t_text *text)
{
	if (obj->text.text_type == TEXT_PERLIN)
	{
		obj->get_bump_mapping = &get_bump_mapping_perlin;
		obj->text.bump_type = BUMP_PERLIN;
	}
	else if (obj->text.text_type == TEXT_MARBLE)
	{
		obj->get_bump_mapping = &get_bump_mapping_marble;
		obj->text.bump_type = BUMP_MARBLE;
	}
	else if (obj->text.text_type == TEXT_WOOD)
	{
		obj->get_bump_mapping = &get_bump_mapping_wood;
		obj->text.bump_type = BUMP_WOOD;
	}
	else if (obj->text.text_type == TEXT_IMAGE)
	{
		obj->get_bump_mapping = &get_bump_mapping_image;
		obj->text.bump_type = BUMP_IMAGE;
	}
}

int		parse_bump_inde(char **line, t_obj *obj, /*t_text *text, */int	index)
{
	int	i;
	int	start;
	char	*s;
	char	*tmp;

	tmp = *line;
	printf("\n\n\nTEST\n\n\n%s\n\n\n", *line);
	i = index;
	while (ft_isspace(tmp[i]))
		++i;
	if (tmp[i] != '(')
	{
		printf("\n\n\n\nhbjviudvdsv\n\n\n==> %s\n", &tmp[i]);
		return (0);
	}
	start = ++i;
	while (tmp[i] && tmp[i] != ')')
		++i;
	printf("Into Parse_Bump inde ==> %s\n", &tmp[i]);
	if (tmp[i] != ')')
		return (0);
	s =	ft_strsub(tmp, start, i - start);
	set_bump_inde(s, obj);//text);
	printf("Type bump Inde ==> %s\n", s);
	while (tmp[i] && tmp[i] != '(')
		++i;
	*line += i;
	printf("Line apres bump ==> %s\n", *line);
	return (parse_double2(line, 0, &obj->text.bump_fact));
}

void	set_bump_inde(char *s, t_obj *obj)//t_text *text)
{
	if (!(ft_strncmp(s, "PERLIN", 6)))
	{
		obj->get_bump_mapping = &get_bump_mapping_perlin;
		obj->text.bump_type = BUMP_PERLIN;
	}

	//	text->bump_type = BUMP_PERLIN;
	if (!(ft_strncmp(s, "MARBLE", 6)))
	{
		obj->get_bump_mapping = &get_bump_mapping_marble;
		obj->text.bump_type = BUMP_MARBLE;
	}

	//	text->bump_type = BUMP_MARBLE;
	if (!(ft_strncmp(s, "WOOD", 4)))
	{
		obj->get_bump_mapping = &get_bump_mapping_wood;
		obj->text.bump_type = BUMP_WOOD;
	}

//		text->bump_type = BUMP_WOOD;
	if (!(ft_strncmp(s, "IMAGE", 5)))
	{
		obj->get_bump_mapping = &get_bump_mapping_image;
		obj->text.bump_type = BUMP_IMAGE;
	}

	//	text->bump_type = BUMP_IMAGE;
	if (!(ft_strncmp(s, "SINUS", 5)))
	{
		obj->get_bump_mapping = &get_bump_mapping_sinus;
		obj->text.bump_type = BUMP_SINUS;
	}

	//	text->bump_type = BUMP_SINUS;
}

int		parse_scene_name(char **line, t_data *data)
{
	int	i;
	int	start;
	char	*s;

	i = 4;
	s = *line;
	while (ft_isspace(s[i]))
		++i;
	if (s[i] != '(')
	{
		printf("Syntax error name");
		return (0);
	}
	if (data->scene_name)
		ft_strdel(&data->scene_name);
	start = ++i;
	while (s[i] && s[i] != ')')
		++i;
	if (s[i] != ')')
	{
		printf("Syntax error name");
		return (0);
	}
	data->scene_name = ft_strsub(s, start, i - start);
	printf("Scene name ==> %s\n", data->scene_name);
	++i;
	while (ft_isspace(s[i]))
		++i;
	printf("Test ==> %s\n", &s[i]);
	if (goto_next_element(line) != '>')
	{
		printf("Error parse_origin\n");
		return(0);
	}
	return (1);
}

void	*parse_img(char **line, t_text *text)
{
	SDL_Surface		*image_bmp;
	SDL_Surface		*image;
	t_text_img		*param;
	unsigned int	pixels_nb;
	unsigned int	i;
	int				name_len;
	char			*image_name;

	i = 0;
	printf("%s\n", *line);
	if (text->text_param)
	{
		ft_printf("Deja une texture\n==> %s\n", *line);
		return (NULL);
	}
	while (**line != '(' && **line)
		++(*line);
	if (**line != '(')
		return (NULL);
	*line += 1;
	if ((name_len = ft_strichr(*line, ')')) == -1)
		return (NULL);
	if (!(image_name = ft_strsub(*line, i, name_len)))
		return (NULL);
	printf("%s %d\n", image_name, name_len);
	if (!(param = malloc(sizeof(t_text_img))))
		return (NULL);
	if (!(image_bmp = IMG_Load(image_name)))
		return (NULL);
	if (!(image = SDL_ConvertSurfaceFormat(image_bmp, SDL_PIXELFORMAT_RGBA8888, 0)))
		return (NULL);
	pixels_nb = image->w * image->h;
	if (!(param->pixels = malloc(sizeof(unsigned int) * pixels_nb)))
		return (NULL);
//	i += name_len + 1;
//	while (ft_isspace(*line[i]))
//		++i;
//	*index = i + 1;
	printf("%s\n", *line);
	param->width = image->w;
	param->height = image->h;
	printf("%d %d\n", image->w, image->h);
	SDL_LockSurface(image);
	i = 0;
	while (i < pixels_nb)
	{
		param->pixels[i] = ((unsigned int *)image->pixels)[i];
		++i;
	}
	printf("%s\n", *line);
	SDL_UnlockSurface(image);
	SDL_FreeSurface(image_bmp);
	SDL_FreeSurface(image);
	goto_next_element(line);
	return ((void *)param);
	(void)line;
}

void	*parse_proc(char **line, t_text *text)
{
	char	stripe;
	int		cmp;
	int		ret;
	t_text_proc	*param;

	stripe = 0;
	cmp = -1;
	ret = 1;
	printf("ParseProc ==> %s\n", *line);
	if (text->text_param)
	{
		ft_printf("Deja une texture\n==> %s\n", *line);
		return (NULL);
	}
	if (!(param = ft_memalloc(sizeof(t_text_proc))))
		return (NULL);
	while (goto_next_element(line) != '>' && ++cmp < 3 && ret != 0)
	{
		printf("Line into parseproc ==> %s\n", *line);
		if (ft_strncmp(*line, "color", 5))
		{
			printf("Pas un bon nom de color\n");
			return (NULL);
		}
		else if (!(ft_strncmp(*line, "color", 5)))
		{
			ret = parse_color_transp(line, 5, &param->color[cmp]);
		}
	}
	if (cmp == 3 || cmp == -1)
	{
		printf("Casse TOI!!!!\n\n");
		return (NULL);
	}
	return ((void *)param);
}

int		parse_color_transp(char **line, int i, t_4vecf *t)
{
	char	*s;

	s = *line;
	while (ft_isspace(s[i]))
		++i;
	if (s[i] != '(' || (i = parse_4vecf(s, i, t)) == -1)
		return (0);
	if (goto_next_element(line) != '>')
	{
		printf("Errore parse_color_transp\n");
		return (0);
	}
	printf("LINE into COLORTRANS ==> %s\nval 0 ==> %f\nval 1 ==> %f\nval 2 ==> %f\nval 3 ==> %f\n", *line, t->val[0], t->val[1], t->val[2], t->val[3]);
	return (1);
}

int		parse_4vecf(char *line, int i, t_4vecf *vec)
{
	int		cpt;

	cpt = 0;
	++i;
	if (line[i] == '(')
		i++;
	while (cpt < 4)
	{
		vec->val[cpt++] = ft_atold(&(line[i]));
		while (line[i] && ((line[i] != ',' && cpt < 4) || (line[i] != ')' && cpt == 4)))
			++i;
		if (!line[i])
			return (-1);
		++i;
	}
	while (line[i] && line[i] != ')')
		++i;
	if (line[i] != ')')
		return (-1);
	return (i);
}

int		parse_origin(char **line, t_3vecf *t, int i)
{
	char	*s;

	s = *line;
//	printf("Test origin 1 ==> %s\n", &s[i]);
	while (ft_isspace(s[i]))
		++i;
	if (s[i] != '(' || (i = parse_3vecf(s, i, t)) == -1)
		return (0);
//	while (ft_isspace(s[i]))
//		++i;
//	printf("Test origin 2 ==> %s\n", &s[i]);
	if (goto_next_element(line) != '>')
	{
		printf("Error parse_origin\n");
		return(0);
	}
	return (1);
}

/*void	*parse_proc(char **line, t_text *text)
{
	char	stripe;
	int		cmp;
	t_text_proc	*param;
	int		ret;

	stripe = 0;
	cmp = 0;
	ft_printf("PARSE_PROC DEBUT ==> %s\n", *line);
	if (text->text_param)
	{
		ft_printf("Deja une texture\n==> %s\n", *line);
		return (NULL);
	}
	if (!(param = ft_memalloc(sizeof(t_text_proc))))
		return (NULL);
	while (stripe != '>' && cmp < 3)
	{
		printf("Parse Proc While ==> %s ==> %d\n", *line, cmp);
		if (**line == '>')
			break ;
		stripe = goto_next_element(line);
		printf("Parse Proc While next ==> %s ==> %d\n", *line, cmp);
		if (!(ft_strncmp(*line, "color", 5)) && (ret = parse_color_transp(line, &(param->color[cmp]), 5, &(param->transp.val[cmp]))) == 1)
		{
			cmp++;
			printf("\n\n\n\nret == %d\n\n\n\n", ret);
		}
		else if (**line != '>' || ret == 0)
		{
			printf("\n\nCACA ==> %s\n\n", *line);
			return (NULL);
		}
	}
	ft_printf("PARSE_PROC FIN==> %s\n%d\n", *line, cmp);
	if (goto_next_element(line) != '>')
	{
		printf("Error parse_proc ==> %s\n", *line);
		return(0);
	}
	return ((void *)param);
}

int		parse_color_transp(char **line, t_3vecf *t, int	i, double *val)
{
	char	*s;
	int		ret;

	s = *line;
	ret = 1;
	printf("\n\nBefore Color_transp ==> %s\n\n", *line);
	while (ft_isspace(s[i]))
		++i;
	printf("Parse 3vecf ==> %s\n", &s[i]);
	ret = parse_origin(line, t, 5);
//	if (s[i] != '(' || (i = parse_3vecf(s, i, t)) == -1)
//		return (0);
	printf("Parse double 2 ==> %s\n", &s[i]);
	ret = parse_double2(line, 0, val);
//	if (s[i] != '(' || (i = parse_double2(&s, i, val)) == 0)
//		return (0);
	ft_printf("\n\nPARSE_COLOR_TRANSP ==> %s\n\n", &s[i]);
	if (goto_next_element(line) != '>')
	{
		printf("Error parse_color_text\n");
		return(0);
	}
	return (1);
}*/

int		parse_cylinder(char **line, t_obj *cylinder, t_data *data)
{
	char	stripe;
	int		ret;
	t_cylinder	*cylinder_param;

	stripe = 0;
	ret = 1;
	printf("\nCylinder ==> %s\n", *line);
	if (cylinder->obj_param)
	{
		printf("\n Fais IECH Cylinder ==> %s\n", *line);
		return (0);
	}
	if (!(cylinder_param = ft_memalloc(sizeof(t_cylinder))))
		return (0);
	while (stripe != '>' && ret != 0)
	{
		stripe = goto_next_element(line);
		if (!ft_strncmp(*line, "origin", 6))
			ret = parse_origin(line, &cylinder_param->center, 6);
		else if (!ft_strncmp(*line, "tip", 3))
			ret = parse_origin(line, &cylinder_param->tip, 3);
		else if (!ft_strncmp(*line, "radius", 6))
			ret = parse_double2(line, 6, &cylinder_param->radius);
	}
	cylinder->obj_param = cylinder_param;
	cylinder->obj_type = OBJ_CYLINDER;
	cylinder->ray_intersect = &ray_intersect_cylinder;
	cylinder->get_normal_inter = &get_normal_intersect_cylinder;
	cylinder->get_origin = &get_origin_cylinder;
	cylinder->move = &move_cylinder;
	cylinder->get_text_coordinate = &get_text_coordinate_cylinder;
	if (data->objs)
		cylinder->next = data->objs;
	else
		cylinder->next = NULL;
	data->objs = cylinder;
	printf("Fin Cylinder ==> %s\n", *line);
	print_vec(cylinder_param->center.val);
	print_vec(cylinder_param->tip.val);
	ft_printf("Radius : %f\n", cylinder_param->radius);
	return (ret);
}

int		parse_plane(char **line, t_obj *plane, t_data *data)
{
	char	stripe;
	int		ret; 
	t_plane	*plane_param;

	stripe = 0;
	ret = 1;
	printf("\nPlane ==> %s\n", *line);
	if (plane->obj_param)
	{
		printf("\n Fais IECH Plane ==> %s\n", *line);
		return (0);
	}
	if (!(plane_param = ft_memalloc(sizeof(t_plane))))
		return (0);
	while (stripe != '>' && ret != 0)
	{
		stripe = goto_next_element(line);
		if (!ft_strncmp(*line, "origin", 6))
			ret = parse_origin(line, &plane_param->origin, 6);
		else if (!ft_strncmp(*line, "normal", 6))
			ret = parse_origin(line, &plane_param->normal, 6);
		else if (!ft_strncmp(*line, "xaxis", 5))
			ret = parse_origin(line, &plane_param->x2d_axis, 5);
	}
	plane->obj_param = plane_param;
	plane->obj_type = OBJ_PLANE;
	plane->ray_intersect = &ray_intersect_plane;
	plane->get_normal_inter = &get_normal_intersect_plane;
	plane->get_origin = &get_origin_plane;
	plane->move = &move_plane;
	plane->get_text_coordinate = &get_text_coordinate_plane;
	if (data->objs)
		plane->next = data->objs;
	else
		plane->next = NULL;
	data->objs = plane;
	printf("Fin Plane ==> %s\n", *line);
	print_vec(plane_param->origin.val);
	print_vec(plane_param->normal.val);
	return (ret);
}

int		parse_sphere(char **line, t_obj *sphere, t_data *data)
{
	char	stripe;
	int		ret;
	t_sphere	*sphere_param;

	stripe = 0;
	ret = 1;
	printf("\nSphere ==> %s\n", *line);
	if (sphere->obj_param)
	{
		printf("\n Fais IECH Sphere ==> %s\n", *line);
		return (0);
	}
	if (!(sphere_param = ft_memalloc(sizeof(t_sphere))))
		return (0);
	while (stripe != '>' && ret != 0)
	{
		stripe = goto_next_element(line);
		if (!ft_strncmp(*line, "origin", 6))
			ret = parse_origin(line, &sphere_param->origin, 6);
		else if (!ft_strncmp(*line, "radius", 6))
			ret = parse_double2(line, 6, &sphere_param->radius);
	}
	sphere->obj_param = sphere_param;
	sphere->obj_type = OBJ_SPHERE;
	sphere->ray_intersect = &ray_intersect_sphere;
	sphere->get_normal_inter = &get_normal_intersect_sphere;
	sphere->get_origin = &get_origin_sphere;
	sphere->move = &move_sphere;
	sphere->get_text_coordinate = &get_text_coordinate_sphere;
	if (data->objs)
		sphere->next = data->objs;
	else
		sphere->next = NULL;
	data->objs = sphere;
	printf("Fin Sphere ==> %s\n", *line);
	print_vec(sphere_param->origin.val);
	ft_printf("Radius : %f\n", sphere_param->radius);
	return (ret);
}

int		parse_cone(char **line, t_obj *cone, t_data *data)
{
	char	stripe;
	int		ret;
	t_cone	*cone_param;

	stripe = 0;
	ret = 1;
	printf("\nCone ==> %s\n", *line);
	if (cone->obj_param)
	{
		printf("\n Fais IECH Cone ==> %s\n", *line);
		return (0);
	}
	if (!(cone_param = ft_memalloc(sizeof(t_cone))))
		return (0);
	while (stripe != '>' && ret != 0)
	{
		stripe = goto_next_element(line);
		if (!ft_strncmp(*line, "origin", 6))
			ret = parse_origin(line, &cone_param->center, 6);
		else if (!ft_strncmp(*line, "tip", 3))
			ret = parse_origin(line, &cone_param->tip, 3);
		else if (!ft_strncmp(*line, "radius", 6))
			ret = parse_double2(line, 6, &cone_param->radius);
	}
	cone->obj_param = cone_param;
	cone->obj_type = OBJ_CONE;
	cone->ray_intersect = &ray_intersect_cone;
	cone->get_normal_inter = &get_normal_intersect_cone;
	cone->get_origin = &get_origin_cone;
	cone->move = &move_cone;
	cone->get_text_coordinate = &get_text_coordinate_cone;
	if (data->objs)
		cone->next = data->objs;
	else
		cone->next = NULL;
	data->objs = cone;
	printf("Fin Cone ==> %s\n", *line);
	print_vec(cone_param->center.val);
	print_vec(cone_param->tip.val);
	ft_printf("Radius : %f\n", cone_param->radius);
	return (ret);
}

int		parse_moebius(char **line, t_obj *moebius, t_data *data)
{
	char	stripe;
	int		ret;
	t_moebius	*moebius_param;

	stripe = 0;
	ret = 1;
	printf("\nMoebius ==> %s\n", *line);
	if (moebius->obj_param)
	{
		printf("\n Fais IECH Moebius ==> %s\n", *line);
		return (0);
	}
	if (!(moebius_param = ft_memalloc(sizeof(t_moebius))))
		return (0);
	while (stripe != '>' && ret != 0)
	{
		stripe = goto_next_element(line);
		if (!ft_strncmp(*line, "origin", 6))
			ret = parse_origin(line, &moebius_param->origin, 6);
		else if (!ft_strncmp(*line, "radius", 6))
			ret = parse_double2(line, 6, &moebius_param->radius);
		else if (!ft_strncmp(*line, "half_width", 10))
			ret = parse_double2(line, 10, &moebius_param->half_width);
	}
	moebius->obj_param = moebius_param;
	moebius->obj_type = OBJ_SPHERE;
	moebius->ray_intersect = &ray_intersect_moebius;
	moebius->get_normal_inter = &get_normal_intersect_moebius;
	moebius->get_origin = &get_origin_moebius;
	moebius->move = &move_moebius;
	moebius->get_text_coordinate = &get_text_coordinate_moebius;
	if (data->objs)
		moebius->next = data->objs;
	else
		moebius->next = NULL;
	data->objs = moebius;
	printf("Fin Moebius ==> %s\n", *line);
	print_vec(moebius_param->origin.val);
	ft_printf("Radius : %f\n", moebius_param->radius);
	moebius->data = data;
	return (ret);
}


int		parse_camera(char **line, t_data *data)
{
	char	stripe;
	int		ret;
	t_cam	*cam;

	stripe = 0;
	ret = 1;
	if (data->camera)
	{
		printf("\n Fais IECH Cam ==> %s\n", *line);
		return (0);
	}
	if(!(cam = ft_memalloc(sizeof(t_cam))))
		return (0);
	while (stripe != '>' && ret != 0)
	{
		stripe = goto_next_element(line);
		if (!ft_strncmp(*line, "origin", 6))
			ret = parse_origin(line, &cam->origin, 6);
		else if (!ft_strncmp(*line, "rotation", 8))
			ret = parse_rotation(line, &cam->rotation, 8);
	}
	data->camera = cam;
	return (ret);
}

int		parse_rotation(char **line, t_2vecf *t, int i)
{
	char	*s;

	s = *line;
	while (ft_isspace(s[i]))
		++i;
	if (s[i] != '(' || (i = parse_2vecf(s, i, t)) == -1)
		return (0);
	++i;
//	while (ft_isspace(s[i]))
//		++i;	
	if (goto_next_element(line) != '>')
	{
		printf("Error parse_rotation\n");
		return(0);
	}
	return (1);
}

int		parse_size(char **line, t_data *data)
{
	int	i;
	char	*s;

	i = 4;
	s = *line;
	while (ft_isspace(s[i]))
		++i;
	if (s[i] != '(' || (i = parse_2vecf(s, i, &data->size)) == -1)
	{
		ft_printf("Syntax error: size\n");
		return (0);
	}
	++i;
	while (ft_isspace(s[i]))
		++i;
//	printf("Test size ==> %s\n", &s[i]);
	if (goto_next_element(line) != '>')
	{
		printf("Error parse_origin\n");
		return(0);
	}
	return (1);
}

char	goto_next_element(char **line)
{
	char	*s;
	char	c;

	s = *line;
//	s++;
	while (*s && *s != '<' && *s != '>')
		++s;
	c = *s;
	s++;
	while (ft_isspace(*s))
		s++;
	if (*s == '\0')
		return ('>');
	*line = s;
	return (c);
}

int		parse_2vecf(char *line, int i, t_2vecf *vec)
{
	int		cpt;

	cpt = 0;
	if (line[i] == '(')
		i++;
	while (cpt < 2)
	{
		vec->val[cpt++] = ft_atold(&(line[i]));
		while (line[i] && ((line[i] != ',' && cpt < 2)
			|| (line[i] != ')' && cpt == 2)))
			++i;
		if (!line[i])
			return (-1);
		++i;
	}
	return (i);
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

int		parse_int(char **line, int i, int *val)
{
	char *s;

	s = *line;
	while (ft_isspace(s[i]))
		i++;
	if (s[i] == '(')
		i++;
	*val = ft_atoi(&(s[i]));
	while (s[i] && s[i] != ')')
			++i;
	if (!s[i])
		return (0);
	if (goto_next_element(line) != '>')
	{
		printf("Error parse_int\n");
		return(0);
	}
	return (1);
}

int		parse_double2(char **line, int i, double *val)
{
	char *s;

	s = *line;
	while (ft_isspace(s[i]))
		i++;
	if (s[i] == '(')
		i++;
	*val = ft_atold(&(s[i]));
	while (s[i] && s[i] != ')')
			++i;
	if (!s[i])
		return (0);
	if (goto_next_element(line) != '>')
	{
		printf("Error parse_double2\n");
		return(0);
	}
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
		return (0);
	return (i + 1);
}

void	print_conf(t_data *data)
{
	ft_printf("It's Good!\n");
	ft_printf("Scene Name ==> %s\n", data->scene_name);
	ft_printf("Scene Size ==> %f / %f\n", data->size.val[0], data->size.val[1]);
	ft_printf("		Camera Origin ==>\n");
	print_vec(data->camera->origin.val);
	ft_printf("		Camera Rotation ==>\n");
//	print_vec2(data->camera->rotation.val);
}

//void	print_t3vecf(t_3vecf *t)
//{
//	ft_printf("Val 1 : %f\nVal 2 : %f\nVal 3 : %f\n", t.val[0], t.val[1], t.val[2]);
//}

int		check_normal(t_3vecf *t)
{
	if (t->val[0] == 0 && t->val[1] == 0 && t->val[2] == 0)
	{
		ft_fdprintf(2, "Normal Nul\n");
		return(0);
	}
	return (1);
}

int		isequal_3vecf(t_3vecf *t1, t_3vecf *t2)
{
	if (t1->val[0] == t2->val[0] && t1->val[1] == t2->val[1] && t1->val[2] == t2->val[2])
	{
		ft_fdprintf(2, "Same Origin and Tip\n");
		return (0);
	}
	return (1);
}
