#include "rt.h"

int		parse_cylinder(char **line, t_obj *cylinder, t_data *data)
{
	char		stripe;
	int			ret;
	t_cylinder	*cylinder_param;

	stripe = 0;
	ret = 1;
	if (cylinder->obj_param)
		return (return_update("Object already declared\n", 0, 2));
	if (!(cylinder_param = ft_memalloc(sizeof(t_cylinder))))
		return (0);
	while (stripe != '>' && ret != 0)
	{
		stripe = goto_next_element(line);
		if (!ft_strncmp_case(*line, "origin", 6))
			ret = parse_origin(line, &cylinder_param->center, 6);
		else if (!ft_strncmp_case(*line, "tip", 3))
			ret = parse_origin(line, &cylinder_param->tip, 3);
		else if (!ft_strncmp_case(*line, "radius", 6))
			ret = parse_double2(line, 6, &cylinder_param->radius);
	}
	if ((ft_fabs(cylinder_param->radius) == 0.f || is_null_3vecf(sub_3vecf(cylinder_param->center, cylinder_param->tip))) && ret != 0)
		return (return_update("Syntax error cylinder\n", 0, 2));
	cylinder->obj_param = cylinder_param;
	cylinder->obj_type = OBJ_CYLINDER;
	cylinder->check_inside = &check_inside_cylinder;
	cylinder->ray_intersect = &ray_intersect_cylinder;
	cylinder->get_normal_inter = &get_normal_intersect_cylinder;
	cylinder->get_origin = &get_origin_cylinder;
	cylinder->move = &move_cylinder;
	cylinder->get_text_coordinate = &get_text_coordinate_cylinder;
	add_object(cylinder, data);
	return (ret);
}

int		parse_plane(char **line, t_obj *plane, t_data *data)
{
	char	stripe;
	int		ret; 
	t_plane	*plane_param;

	stripe = 0;
	ret = 1;
	if (plane->obj_param)
		return (return_update("Object already declared\n", 0, 2));
	if (!(plane_param = ft_memalloc(sizeof(t_plane))))
		return (0);
	while (stripe != '>' && ret != 0)
	{
		stripe = goto_next_element(line);
		if (!ft_strncmp_case(*line, "origin", 6))
			ret = parse_origin(line, &plane_param->origin, 6);
		else if (!ft_strncmp_case(*line, "normal", 6))
			ret = parse_origin(line, &plane_param->normal, 6);
		else if (!ft_strncmp_case(*line, "xaxis", 5))
			ret = parse_origin(line, &plane_param->x2d_axis, 5);
	}
	if (is_null(plane_param->normal.val[0] * plane_param->normal.val[0] + plane_param->normal.val[1] * plane_param->normal.val[1] + plane_param->normal.val[2] * plane_param->normal.val[2]) || !is_null(dot_product_3vecf(plane_param->x2d_axis, plane_param->normal)))
		return (return_update("Syntax error plane\n", 0, 2));
	plane->obj_param = plane_param;
	plane->obj_type = OBJ_PLANE;
	plane->check_inside = &check_inside_plane;
	plane->ray_intersect = &ray_intersect_plane;
	plane->get_normal_inter = &get_normal_intersect_plane;
	plane->get_origin = &get_origin_plane;
	plane->move = &move_plane;
	plane->get_text_coordinate = &get_text_coordinate_plane;
	add_object(plane, data);
	return (ret);
}

int		parse_sphere(char **line, t_obj *sphere, t_data *data)
{
	char	stripe;
	int		ret;
	t_sphere	*sphere_param;

	stripe = 0;
	ret = 1;
	if (sphere->obj_param)
		return (return_update("Object already declared\n", 0, 2));
	if (!(sphere_param = ft_memalloc(sizeof(t_sphere))))
		return (0);
	while (stripe != '>' && ret != 0)
	{
		stripe = goto_next_element(line);
		if (!ft_strncmp_case(*line, "origin", 6))
			ret = parse_origin(line, &sphere_param->origin, 6);
		else if (!ft_strncmp_case(*line, "radius", 6))
			ret = parse_double2(line, 6, &sphere_param->radius);
	}
	if (ft_fabs(sphere_param->radius) == 0.f && ret != 0)
		return (return_update("Syntax error Sphere\n", 0, 2));
	sphere->obj_param = sphere_param;
	sphere->obj_type = OBJ_SPHERE;
	sphere->check_inside = &check_inside_sphere;
	sphere->ray_intersect = &ray_intersect_sphere;
	sphere->get_normal_inter = &get_normal_intersect_sphere;
	sphere->get_origin = &get_origin_sphere;
	sphere->move = &move_sphere;
	sphere->get_text_coordinate = &get_text_coordinate_sphere;	
	add_object(sphere, data);
	return (ret);
}

int		parse_cone(char **line, t_obj *cone, t_data *data)
{
	char	stripe;
	int		ret;
	t_cone	*cone_param;

	stripe = 0;
	ret = 1;
	if (cone->obj_param)
		return (return_update("Object already declared\n", 0, 2));
	if (!(cone_param = ft_memalloc(sizeof(t_cone))))
		return (0);
	while (stripe != '>' && ret != 0)
	{
		stripe = goto_next_element(line);
		if (!ft_strncmp_case(*line, "origin", 6))
			ret = parse_origin(line, &cone_param->center, 6);
		else if (!ft_strncmp_case(*line, "tip", 3))
			ret = parse_origin(line, &cone_param->tip, 3);
		else if (!ft_strncmp_case(*line, "radius", 6))
			ret = parse_double2(line, 6, &cone_param->radius);
	}
	if ((ft_fabs(cone_param->radius) == 0.f || is_null_3vecf(sub_3vecf(cone_param->center, cone_param->tip))) && ret != 0)
		return (return_update("Syntax error Cone\n", 0, 2));
	cone->obj_param = cone_param;
	cone->obj_type = OBJ_CONE;
	cone->ray_intersect = &ray_intersect_cone;
	cone->check_inside = &check_inside_cone;
	cone->get_normal_inter = &get_normal_intersect_cone;
	cone->get_origin = &get_origin_cone;
	cone->move = &move_cone;
	cone->get_text_coordinate = &get_text_coordinate_cone;
	add_object(cone, data);
	return (ret);
}

int		parse_moebius(char **line, t_obj *moebius, t_data *data)
{
	char	stripe;
	int		ret;
	t_moebius	*moebius_param;

	stripe = 0;
	ret = 1;
	if (moebius->obj_param)
		return (return_update("Object already declared\n", 0, 2));
	if (!(moebius_param = ft_memalloc(sizeof(t_moebius))))
		return (0);
	while (stripe != '>' && ret != 0)
	{
		stripe = goto_next_element(line);
		if (!ft_strncmp_case(*line, "origin", 6))
			ret = parse_origin(line, &moebius_param->origin, 6);
		else if (!ft_strncmp_case(*line, "radius", 6))
			ret = parse_double2(line, 6, &moebius_param->radius);
		else if (!ft_strncmp_case(*line, "half_width", 10))
			ret = parse_double2(line, 10, &moebius_param->half_width);
	}
	if ((moebius_param->radius <= 0.f || moebius_param->half_width <= 0.f) && ret != 0)
		return (return_update("Syntax error Moebius\n", 0, 2));
	moebius->obj_param = moebius_param;
	moebius->obj_type = OBJ_SPHERE;
	moebius->check_inside = &check_inside_moebius;
	moebius->ray_intersect = &ray_intersect_moebius;
	moebius->get_normal_inter = &get_normal_intersect_moebius;
	moebius->get_origin = &get_origin_moebius;
	moebius->move = &move_moebius;
	moebius->get_text_coordinate = &get_text_coordinate_moebius;
	add_object(moebius, data);
	moebius->data = data;
	return (ret);
}
