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
		return (error(ERRORON, NULL));
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
		return (error(ERRORCOLOR, LSTCOLOR));
	goto_next_element(line);
	return (1);
	//*onoff = 1;
}

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
		else if (stripe == '<')
			return (syn_error(SERROR, MOTION, "<spf (Value)>\n", NULL));
	}
	clamp_val((double *)&motion->spf, 0, 32);
	push_front_motion(&obj->motions, motion);
	return (ret);
}

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
		return (error(ERRORMATERIAL, NULL));
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
		while (line[i] && line[i] != ')' && line[i] != ',' && line[i] != '>')
		//while (line[i] && ((line[i] != ',' && cpt < 4) || (line[i] != ')' && cpt == 4)))
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

void	add_component(t_obj *obj, t_composed *composed)
{
	t_obj **obj_tab;
	int		i;

	i = 0;
	while (composed->components && composed->components[i])
		++i;
	if (!(obj_tab = malloc(sizeof(t_obj *) * (i + 2))))
		return ;
	printf("wwwwwww iiiiiiiiiii %d\n", i);
	obj_tab[i + 1] = NULL;
		//printf("i")
	if (composed->components)
	while (i > 0)
	{
		printf("%p\n", composed->components[i - 1]->ray_intersect);
		obj_tab[i] = composed->components[i - 1];
		--i;
	}
	obj_tab[0] = obj;
	free(composed->components);
	composed->components = obj_tab;
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
		while (line[i] && line[i] != ')' && line[i] != ',' && line[i] != '>')
			++i;
		if (line[i] == ')')
			return (i);
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
		printf("S1 ==> %d\nS2 ==> %d\n", *s1, *s2);
		cpt++;
		s1++;
		s2++;
	}
	printf("S1 ==> %s\nS2 ==> %s\nCPT ==> %zu\n", s1, s2, cpt);
	return ((unsigned char)ft_tolower(*s1) - ft_tolower(*s2));
}
