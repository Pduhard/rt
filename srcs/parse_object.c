#include "rt.h"

int		pick_native_object(char **line, t_obj *obj)
{
	int	ret;

	ret = 1;
	if (!ft_strncmp_case(*line, "cone", 4))
		ret = parse_cone(line, obj);
	else if (!ft_strncmp_case(*line, "sphere", 6))
		ret = parse_sphere(line, obj);
	else if (!ft_strncmp_case(*line, "plane", 5))
		ret = parse_plane(line, obj);
	else if (!ft_strncmp_case(*line, "cylinder", 8))
		ret = parse_cylinder(line, obj);
	else if (!ft_strncmp_case(*line, "triangle", 8))
		ret = parse_triangle(line, obj);
	return (ret);
}

int		pick_eq_object(char **line, t_obj *obj, t_composed *from)
{
	int	ret;

	ret = 1;
	if (!from && !ft_strncmp_case(*line, "moebius", 7))
		ret = parse_moebius(line, obj);
	else if (!from && !ft_strncmp_case(*line, "ellipsoid", 9))
		ret = parse_ellipsoid(line, obj);
	else if (!from && !ft_strncmp_case(*line, "hyperboloid", 11))
		ret = parse_hyperboloid(line, obj);
	else if (!from && !ft_strncmp_case(*line, "horse_saddle", 12))
		ret = parse_horse_saddle(line, obj);
	else if (!from && !ft_strncmp_case(*line, "monkey_saddle", 13))
		ret = parse_monkey_saddle(line, obj);
	else if (!from && !ft_strncmp_case(*line, "cyclide", 7))
		ret = parse_cyclide(line, obj);
	else if (!from && !ft_strncmp_case(*line, "fermat", 6))
		ret = parse_fermat(line, obj);
	return (ret);
}

int		pick_attribute_object(char **line, t_obj *obj)
{
	int	ret;

	ret = 1;
	if (!ft_strncmp_case(*line, "texture", 7))
		ret = parse_texture2(line, obj);
	else if (!(ft_strncmp_case(*line, "cutting", 7)))
		ret = parse_cutting(line, obj);
	else if (!(ft_strncmp_case(*line, "MotionBlur", 10)))
		ret = parse_motion(line, obj);
	else if (!ft_strncmp_case(*line, "reflection", 10))
		ret = parse_double(line, 10, &obj->reflection);
	else if (!ft_strncmp_case(*line, "refraction", 10))
		ret = parse_double(line, 10, &obj->refraction);
	else if (!ft_strncmp_case(*line, "shininess", 9))
		ret = parse_double(line, 9, &obj->shininess);
	else if (!ft_strncmp_case(*line, "material", 8))
		ret = parse_material(line, 8, obj);
	else if (**line != '<' && **line != '>')
		return (error(UNKNOWOBJECT, NULL));
	return (ret);
}

void  clamp_and_set_dflt(t_obj *obj)
{
	clamp_val(&obj->reflection, 0, 1);
	clamp_val(&obj->shininess, 0, 1);
	clamp_val(&obj->refraction, 0, 3);
	clamp_val(&obj->refraction, 0, 2.42);
	if (obj->text.scale.val[0] == 0 && obj->text.scale.val[1] == 0)
		obj->text.scale = (t_2vecf){{0, 0}};
	if (obj->shininess > 0)
		obj->shininess = exp(11 - 10 * obj->shininess);
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
		if (**line != '>' && !(ret = pick_native_object(line, obj)))
			return (0);
		else if (**line != '>' && !(ret = pick_eq_object(line, obj, from)))
			return (0);
		else if (is_composed_object(line, data, &ret))
			composed = 1;
		else if (**line != '>' && !(ret = pick_attribute_object(line, obj)))
			return (0);
	}
	clamp_and_set_dflt(obj);
	push_object(obj, composed, data, from);
	return (ret);
}
