/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_object.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aplat <aplat@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/15 20:32:45 by aplat             #+#    #+#             */
/*   Updated: 2020/05/23 21:36:53 by aplat            ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

static int	pick_native_object(char **line, t_obj *obj)
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

static int	pick_eq_object(char **line, t_obj *obj, t_composed *from)
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

static int	pick_attribute_object(char **line, t_obj *obj)
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
	{
		ret = parse_double(line, 9, &obj->shininess);
		set_shininess(&obj->shininess);
	}
	else if (!ft_strncmp_case(*line, "material", 8))
		ret = parse_material(line, 8, obj);
	else if (**line != '<' && **line != '>')
		return (error(UNKNOWOBJECT, NULL));
	return (ret);
}

int			check_object(t_obj *obj, int composed,
	t_data *data, t_composed *from)
{
	if (!composed)
	{
		if (!obj->obj_param)
			ft_fdprintf(2, "Need to specify object shapes: %s %s\n",
			"possible value: sphere cone cylinder cyclide triangle ellipsoid",
			"hyperboloid horse_saddle monkey_saddle plane fermat or moebius");
		else if (!obj->text.text_param)
			ft_fdprintf(2, "Need to specify texture type: %s\n",
			"possible value: perlin marble wood fbm uni grid or imagem");
		else if (obj->text.scale.val[0] && !obj->text.scale.val[1])
			ft_fdprintf(2, "Texture scale x must not be null\n");
		else if (!obj->text.scale.val[0] && obj->text.scale.val[1])
			ft_fdprintf(2, "Texture scale y must not be null\n");
		else
		{
			clamp_and_set_dflt(obj);
			push_object(obj, composed, data, from);
			return (1);
		}
		return (0);
	}
	clamp_and_set_dflt(obj);
	push_object(obj, composed, data, from);
	return (1);
}

int			parse_objects(char **line, t_data *data, t_composed *from)
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
			return (error_parse_object(obj));
		else if (**line != '>' && !(ret = pick_eq_object(line, obj, from)))
			return (error_parse_object(obj));
		else if (is_composed_object(line, data, &ret))
			composed = 1;
		else if (**line != '>' && !(ret = pick_attribute_object(line, obj)))
			return (error_parse_object(obj));
	}
	if (!check_object(obj, composed, data, from))
		return (error_parse_object(obj));
	return (ret);
}
