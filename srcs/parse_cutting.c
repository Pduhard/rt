#include "rt.h"

int		parse_cut_texture(char **line, t_obj *cut)
{
	char	stripe;
	t_plane	*param;

	cut->obj_type = OBJ_CUT_TEXTURE;
	if (cut->obj_param)
		return (error(ALREADYCUT, NULL));
	if (!(param = ft_memalloc(sizeof(t_plane))))
		return (0);
	stripe = goto_next_element(line);
	if (stripe != '>')
		return (error(CUTTEXTURE, NULL));
	cut->obj_param = param;
	return (1);
}

int		parse_cut_sphere(char **line, t_obj *cut)
{
	t_sphere	*param;
	char		stripe;
	int			ret;

	stripe = 0;
	ret = 1;
	cut->obj_type = OBJ_SPHERE;
	if (cut->obj_param)
		return (error(ALREADYCUT, NULL));
	if (!(param = ft_memalloc(sizeof(t_sphere))))
		return (0);
	while (stripe != '>' && ret != 0)
	{
		stripe = goto_next_element(line);
		if (!(ft_strncmp_case(*line, "origin", 6)))
			ret = parse_origin(line, &param->origin, 6);
		else if (!(ft_strncmp_case(*line, "radius", 6)))
			ret = parse_double2(line, 6, &param->radius);	
		else if (stripe == '<' || ret == 0)
			return (syn_error(SERROR, SYNCUT, SPHERECUT, ""));
	}
	cut->obj_param = param;
	return (ret);
}

int		parse_cut_cube(char **line, t_obj *cut)
{
	t_cube		*param;
	char		stripe;
	int			ret;

	stripe = 0;
	ret = 1;
	cut->obj_type = OBJ_CUBE;
	if (cut->obj_param)
		return (error(ALREADYCUT, NULL));
	if (!(param = ft_memalloc(sizeof(t_cube))))
		return (0);
	while (stripe != '>' && ret != 0)
	{
		stripe = goto_next_element(line);
		if (!(ft_strncmp_case(*line, "x_range", 7)))
			ret = parse_rotation(line, &param->x_range, 7);
		else if (!(ft_strncmp_case(*line, "y_range", 7)))
			ret = parse_rotation(line, &param->y_range, 7);
		else if (!(ft_strncmp_case(*line, "z_range", 7)))
			ret = parse_rotation(line, &param->z_range, 7);
		else if (stripe == '<' || ret == 0)
			return (syn_error(SERROR, SYNCUT, CUBECUT, ZRANGE));
	}
	cut->obj_param = param;
	return (ret);
}

int		parse_cutting(char **line, t_obj *obj)
{
	char	stripe;
	int		ret;
	t_obj	*cut;

	stripe = 0;
	ret = 1;
	if (!(cut = ft_memalloc(sizeof(t_obj))))
		return (0);
	while (stripe != '>' && ret != 0)
	{
		stripe = goto_next_element(line);
		if (!(ft_strncmp_case(*line, "static", 6)))
			ret = parse_cut_static_real(line, cut);
		else if (!(ft_strncmp_case(*line, "real", 4)))
			ret = parse_cut_static_real(line, cut);
		else if (!(ft_strncmp_case(*line, "texture", 7)))
			ret = parse_cut_texture(line, cut);
		else if (!(ft_strncmp_case(*line, "sphere", 6)))
			ret = parse_cut_sphere(line, cut);
		else if (!(ft_strncmp_case(*line, "cube", 4)))
			ret = parse_cut_cube(line, cut);
		else if (stripe == '<')
			return (error(UNKNOWCUT, NULL));
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
		return (error(ALREADYCUT, NULL));
	if (!(param = ft_memalloc(sizeof(t_plane))))
		return (0);
	while (stripe != '>' && ret != 0)
	{
		stripe = goto_next_element(line);
		if (!(ft_strncmp_case(*line, "origin", 6)))
			ret = parse_origin(line, &param->origin, 6);
		else if (!(ft_strncmp_case(*line, "normal", 6)))
			ret = parse_origin(line, &param->normal, 6);
		else if (!(cut->obj_param) && stripe == '<')
			return (syn_error(SERROR, SYNCUT, STATICCUT, ""));
	}
	cut->obj_param = param;
	if (ret == 0)
		return (syn_error(SERROR, SYNCUT, STATICCUT, ""));
	return (ret);
}