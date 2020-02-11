#include "rt.h"

int		return_update(char *error, int ret, int skip)
{
	ft_fdprintf(skip, error);
	return (ret);
}

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
		return (return_update("ON OFF possible value: ON/1, OFF/0\n", 0, 2));
	goto_next_element(line);
	return (1);
}

int		parse_color_filter(char **line, t_data *data)
{
	char	*s;

	s = *line;
	while (*s != '(' && *s)
		++s;
	if (!ft_strncmp_case(s, "(SEPIA)", 4))
		data->apply_color_filter = &apply_color_filter_sepia;
//	else if (!ft_strncmp_case(s, "(SATURATE)", 4))
//		data->apply_color_filter = &apply_color_filter_saturate;
	else
		return (return_update("Color filter possible value: SEPIA\n", 0, 2));
	goto_next_element(line);
	return (1);
	//*onoff = 1;
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
		if (!(ft_strncmp_case(*line, "ambient", 7)))
		{
			light->light_type = LIGHT_AMBIENT;
			ret = parse_origin(line, &light->color, 7);
			if (**line == '<')
				return (return_update("Syntax error light ambient\n", 0, 2));
		}
		else if (!(ft_strncmp_case(*line, "directional", 11)))
		{
			light->light_type = LIGHT_DIRECTIONAL;
			ret = parse_origin(line, &light->param, 11);
			goto_next_element(line);
			if (!(ft_strncmp_case(*line, "color", 5)))
				ret = parse_origin(line, &light->color, 5);
			else
				return (return_update("Syntax error light directionnal\n", 0, 2));
		}
		else if (!(ft_strncmp_case(*line, "point", 5)))
		{
			light->light_type = LIGHT_POINT;
			ret = parse_origin(line, &light->param, 5);
			goto_next_element(line);
			if (!(ft_strncmp_case(*line, "color", 5)))
				ret = parse_origin(line, &light->color, 5);
			else
				return (return_update("Syntax error light point\n", 0, 2));
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
		if (!(ft_strncmp_case(*line, "dir", 3)))
		{
			ret = parse_origin(line, &motion->dir, 3);
			goto_next_element(line);
			printf("Parse_moyion ==> %s\n", *line);
			if (!(ft_strncmp_case(*line, "speed", 5)))
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
		if (!(ft_strncmp_case(*line, "dir", 3)))
			ret = parse_origin(line, &motion->dir, 3);
		else if (!(ft_strncmp_case(*line, "speed", 3)))
			ret = parse_double2(line, 5, &motion->speed_fact);
		else if (!(ft_strncmp_case(*line, "spf", 3)))
			ret = parse_int(line, 3, &motion->spf);
	/*	else if (**line != '<' && **line != '>') //ie end of element
		{
			printf("Parse_motion ==> %s\n", *line);
			printf("Bad conf 2 Motion\n");
			return (0);
		}*/
	}
	clamp_val((double *)&motion->spf, 0, 32);
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
		if (!ft_strncmp_case(*line, "color", 5))
			ret = parse_origin(line, &light->color, 5);
		if (!ft_strncmp_case(*line, "origin", 6))
			ret = parse_origin(line, &light->origin, 6);
		if (!ft_strncmp_case(*line, "dir", 3))
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

int		parse_material(char **line, int i, t_obj *obj)
{
	char	*s;

	s = *line;
	while (s[i] && s[i] != '(')
		++i;
	if (s[i])
		++i;
	else
		return (0);
	if (!ft_strncmp_case(&(s[i]), "diffuse", 7))
		obj->material_type = MAT_DIFFUSE;
	else if (!ft_strncmp_case(&(s[i]), "negative", 8))
		obj->material_type = MAT_NEGATIVE;
	else
		return (return_update("Unknow material type\n", 0, 2));
	goto_next_element(line);
	return (1);
}

int		clamp_val(double *val, double min, double max)
{
	if (*val < min)
		*val = min;
	else if (*val > max)
		*val = max;
	return (0);
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
		if (!(ft_strncmp_case(*line, "UNI", 3)))
		{
			obj->get_text_color = &get_uni_color;
			obj->text.text_type = TEXT_UNI;
			ret = obj->text.text_param ? 0 : 1;
		}
		else if (!(ft_strncmp_case(*line, "GRID", 4)))
		{
			obj->get_text_color = &get_grid_color;
			obj->text.text_type = TEXT_GRID;
			ret = obj->text.text_param ? 0 : 1;
		}
		else if (!(ft_strncmp_case(*line, "PERLIN", 6)))
		{
			obj->get_text_color = &get_perlin_color;
			obj->text.text_type = TEXT_PERLIN;
			ret = obj->text.text_param ? 0 : 1;
		}
		else if (!(ft_strncmp_case(*line, "MARBLE", 6)))
		{
			obj->get_text_color = &get_marble_color;
			obj->text.text_type = TEXT_MARBLE;
			ret = obj->text.text_param ? 0 : 1;
		}
		else if (!(ft_strncmp_case(*line, "WOOD", 4)))
		{
			obj->get_text_color = &get_wood_color;
			obj->text.text_type = TEXT_WOOD;
			ret = obj->text.text_param ? 0 : 1;
		}
		else if (!(ft_strncmp_case(*line, "IMAGE", 5)))
		{
			obj->get_text_color = &get_image_color;
			obj->text.text_type = TEXT_IMAGE;
			ret = obj->text.text_param ? 0 : 1;
		}
		else if (!(ft_strncmp_case(*line, "offset", 6)))
			ret = parse_rotation(line, &obj->text.offset, 6);
		else if (!(ft_strncmp_case(*line, "scale", 5)))
			ret = parse_rotation(line, &obj->text.scale, 5);
		else if (!(ft_strncmp_case(*line, "BumpMapping", 11)) && obj->text.text_param)
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
		if (ft_strncmp_case(*line, "color", 5))
		{
			printf("Pas un bon nom de color\n");
			return (NULL);
		}
		else if (!(ft_strncmp_case(*line, "color", 5)))
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
		if (!(ft_strncmp_case(*line, "color", 5)) && (ret = parse_color_transp(line, &(param->color[cmp]), 5, &(param->transp.val[cmp]))) == 1)
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

void	add_object(t_obj *obj, t_data *data)
{
	if (obj->material_type == MAT_DIFFUSE)
	{
		if (data->objs)
			obj->next = data->objs;
		else
			obj->next = NULL;
		data->objs = obj;
	}
	else if (obj->material_type == MAT_NEGATIVE)
	{
		if (data->negative_objs)
			obj->next = data->negative_objs;
		else
			obj->next = NULL;
		data->negative_objs = obj;
	}
}

int		parse_rotation(char **line, t_2vecf *t, int i)
{
	char	*s;

	s = *line;
	while (ft_isspace(s[i]))
		++i;
	if (s[i] != '(' || (i = parse_2vecf(s, i, t)) == -1)
		return (0);
//	++i;
//	while (ft_isspace(s[i]))
//		++i;	
	if (goto_next_element(line) != '>')
	{
		printf("Error parse_rotation\n");
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
			|| (line[i] != ')' && cpt == 2 && line[i] != '>')))
			++i;
		if (!line[i] || line[i] == '>')
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
		while (line[i] && ((line[i] != ',' && cpt < 3) || (line[i] != ')' && cpt == 3 && line[i] != '>')))
			++i;
		if (!line[i] || line[i] == '>')
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
	while (s[i] && s[i] != ')' && s[i] != '>')
			++i;
	if (!s[i])
		return (0);
	if (goto_next_element(line) != '>')
		return (return_update("Error parse double\n", 0, 2));
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

int		ft_strncmp_case(const char *s1, const char *s2, size_t n)
{
	size_t	cpt;

	cpt = 1;
	if (n < 1)
		return (0);
	cpt = 1;
	while (ft_tolower(*s1) == ft_tolower(*s2) && cpt < n && *s1 != '\0')
	{
		cpt++;
		s1++;
		s2++;
	}
	return ((unsigned char)*s1 - *s2);
}
