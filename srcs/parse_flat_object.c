#include "rt.h"

int		parse_cylinder(char **line, t_obj *cylinder)
{
	char		stripe;
	int			ret;
	t_cylinder	*cylinder_param;

	stripe = 0;
	ret = 1;
	if (cylinder->obj_param)
		return (error(ALREADYOBJ, NULL));
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
			ret = parse_double(line, 6, &cylinder_param->radius);
		else if (stripe == '<')
			return (syn_error(SERROR, CYLINDER, TIP, RADIUS));
	}
	if ((ft_fabs(cylinder_param->radius) == 0.f || is_null_3vecf(sub_3vecf(cylinder_param->center, cylinder_param->tip))) || ret == 0)
		return (syn_error(SERROR, CYLINDER, TIP, RADIUS));
	cylinder->obj_param = cylinder_param;
	cylinder->obj_type = OBJ_CYLINDER;
	cylinder->check_inside = &check_inside_cylinder;
	cylinder->ray_intersect = &ray_intersect_cylinder;
	cylinder->get_normal_inter = &get_normal_intersect_cylinder;
	cylinder->get_origin = &get_origin_cylinder;
	cylinder->move = &move_cylinder;
	cylinder->rotate = &rotate_cylinder;
	cylinder->get_text_coordinate = &get_text_coordinate_cylinder;
	//add_object(cylinder, data);
	return (ret);
}

int		parse_plane(char **line, t_obj *plane)
{
	char	stripe;
	int		ret; 
	t_plane	*plane_param;

	stripe = 0;
	ret = 1;
	if (plane->obj_param)
		return (error(ALREADYOBJ, NULL));
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
		else if (stripe == '<')
			return (syn_error(SERROR, PLANE, NORMAL, XAXIS));
	}
	if (is_null(plane_param->normal.val[0] * plane_param->normal.val[0] + plane_param->normal.val[1] * plane_param->normal.val[1] + plane_param->normal.val[2] * plane_param->normal.val[2]) || !is_null(dot_product_3vecf(plane_param->x2d_axis, plane_param->normal)) || ret == 0)
		return (syn_error(SERROR, PLANE, NORMAL, XAXIS));
	plane->obj_param = plane_param;
	plane->obj_type = OBJ_PLANE;
	plane->check_inside = &check_inside_plane;
	plane->ray_intersect = &ray_intersect_plane;
	plane->get_normal_inter = &get_normal_intersect_plane;
	plane->get_origin = &get_origin_plane;
	plane->move = &move_plane;
	plane->rotate = &rotate_plane;
	plane->get_text_coordinate = &get_text_coordinate_plane;
//	add_object(plane, data);
	return (ret);
}

int		parse_sphere(char **line, t_obj *sphere)
{
	char	stripe;
	int		ret;
	t_sphere	*sphere_param;

	stripe = 0;
	ret = 1;
	if (sphere->obj_param)
		return (error(ALREADYOBJ, NULL));
	if (!(sphere_param = ft_memalloc(sizeof(t_sphere))))
		return (0);
	while (stripe != '>' && ret != 0)
	{
		stripe = goto_next_element(line);
		if (!ft_strncmp_case(*line, "origin", 6))
			ret = parse_origin(line, &sphere_param->origin, 6);
		else if (!ft_strncmp_case(*line, "radius", 6))
			ret = parse_double(line, 6, &sphere_param->radius);
		else if (stripe == '<')
			return (syn_error(SERROR, SPHERE, RADIUS, NULL));
	}
	if (ft_fabs(sphere_param->radius) == 0.f || ret == 0)
		return (syn_error(SERROR, SPHERE, RADIUS, NULL));
	sphere->obj_param = sphere_param;
	sphere->obj_type = OBJ_SPHERE;
	sphere->check_inside = &check_inside_sphere;
	sphere->ray_intersect = &ray_intersect_sphere;
	sphere->get_normal_inter = &get_normal_intersect_sphere;
	sphere->get_origin = &get_origin_sphere;
	sphere->move = &move_sphere;
	sphere->rotate = &rotate_sphere;
	sphere->get_text_coordinate = &get_text_coordinate_sphere;	
//	add_object(sphere, data);
	return (ret);
}

int		parse_triangle(char **line, t_obj *triangle)
{
	char	stripe;
	int		ret;
	t_triangle	*triangle_param;

	stripe = 0;
	ret = 1;
	if (triangle->obj_param)
		return (error(ALREADYOBJ, NULL));
	if (!(triangle_param = ft_memalloc(sizeof(t_triangle))))
		return (0);
	while (stripe != '>' && ret != 0)
	{
		stripe = goto_next_element(line);
		if (!ft_strncmp_case(*line, "a", 1))
			ret = parse_origin(line, &triangle_param->a, 1);
		else if (!ft_strncmp_case(*line, "b", 1))
			ret = parse_origin(line, &triangle_param->b, 1);
		else if (!ft_strncmp_case(*line, "c", 1))
			ret = parse_origin(line, &triangle_param->c, 1);
		else if (stripe == '<')
			return (syn_error(SERROR, TRIANGLE, TRIBC, NULL));
	}
	if (ret == 0)
		return (syn_error(SERROR, TRIANGLE, TRIBC, NULL));
	triangle_param->origin = assign_3vecf(triangle_param->a.val[0] + triangle_param->a.val[0] + triangle_param->a.val[0], triangle_param->a.val[1] + triangle_param->a.val[1] + triangle_param->a.val[1], triangle_param->a.val[2] + triangle_param->a.val[2] + triangle_param->a.val[2]);
	triangle_param->origin.val[0] /= 3.;
	triangle_param->origin.val[1] /= 3.;
	triangle_param->origin.val[2] /= 3.;
	//if (ft_fabs(triangle_param->radius) == 0.f)
	//	return (syn_error(SERROR, triangle, RADIUS, NULL));
	triangle->obj_param = triangle_param;
	triangle->obj_type = OBJ_TRIANGLE;
	triangle->check_inside = &check_inside_triangle;
	triangle->ray_intersect = &ray_intersect_triangle;
	triangle->get_normal_inter = &get_normal_intersect_triangle;
	triangle->get_origin = &get_origin_triangle;
	triangle->move = &move_triangle;
	triangle->rotate = &rotate_triangle;
	triangle->get_text_coordinate = &get_text_coordinate_triangle;	
//	add_object(triangle, data);
	return (ret);
}

int		parse_horse_saddle(char **line, t_obj *horse_saddle)
{
	char	stripe;
	int		ret;
	t_horse_saddle	*horse_saddle_param;

	stripe = 0;
	ret = 1;
	if (horse_saddle->obj_param)
		return (error(ALREADYOBJ, NULL));
	if (!(horse_saddle_param = ft_memalloc(sizeof(t_horse_saddle))))
		return (0);
	while (stripe != '>' && ret != 0)
	{
		stripe = goto_next_element(line);
		if (!ft_strncmp_case(*line, "origin", 6))
			ret = parse_origin(line, &horse_saddle_param->origin, 6);
		else if (!ft_strncmp_case(*line, "x_fact", 6))
			ret = parse_double(line, 6, &horse_saddle_param->x_fact);
		else if (!ft_strncmp_case(*line, "y_fact", 6))
			ret = parse_double(line, 6, &horse_saddle_param->y_fact);
		else if (stripe == '<')
			return (syn_error(SERROR, HORSE, XYFACT, NULL));
	}
//	if (ft_fabs(horse_saddle_param->radius) == 0.f)
//		return (syn_error(SERROR, SPHERE, ORIGIN, RADIUS, NULL));
//	horse_saddle_param->translat_mat = build_translation_matrix(horse_saddle_param->origin, horse_saddle_param->x_axis, horse_saddle_param->y_axis, horse_saddle_param->z_axis);
	horse_saddle->obj_param = horse_saddle_param;
	horse_saddle->obj_type = OBJ_HORSE_SADDLE;
	horse_saddle->check_inside = &check_inside_horse_saddle;
	horse_saddle->ray_intersect = &ray_intersect_horse_saddle;
	horse_saddle->get_normal_inter = &get_normal_intersect_horse_saddle;
	horse_saddle->get_origin = &get_origin_horse_saddle;
	horse_saddle->move = &move_horse_saddle;
	horse_saddle->get_text_coordinate = &get_text_coordinate_horse_saddle;	
//	add_object(horse_saddle, data);
	return (ret);
}

int		parse_monkey_saddle(char **line, t_obj *monkey_saddle)
{
	char	stripe;
	int		ret;
	t_monkey_saddle	*monkey_saddle_param;

	stripe = 0;
	ret = 1;
	if (monkey_saddle->obj_param)
		return (error(ALREADYOBJ, NULL));
	if (!(monkey_saddle_param = ft_memalloc(sizeof(t_monkey_saddle))))
		return (0);
	while (stripe != '>' && ret != 0)
	{
		stripe = goto_next_element(line);
		if (!ft_strncmp_case(*line, "origin", 6))
			ret = parse_origin(line, &monkey_saddle_param->origin, 6);
		else if (stripe == '<')
			return (syn_error(SERROR, MONKEY, ORIGIN, NULL));
	}
	if (ret == 0)
		return (syn_error(SERROR, MONKEY, ORIGIN, NULL));
//	if (ft_fabs(monkey_saddle_param->radius) == 0.f)
//		return (syn_error(SERROR, SPHERE, ORIGIN, RADIUS, NULL));
//	monkey_saddle_param->translat_mat = build_translation_matrix(monkey_saddle_param->origin, monkey_saddle_param->x_axis, monkey_saddle_param->y_axis, monkey_saddle_param->z_axis);
	monkey_saddle->obj_param = monkey_saddle_param;
	monkey_saddle->obj_type = OBJ_MONKEY_SADDLE;
	monkey_saddle->check_inside = &check_inside_monkey_saddle;
	monkey_saddle->ray_intersect = &ray_intersect_monkey_saddle;
	monkey_saddle->get_normal_inter = &get_normal_intersect_monkey_saddle;
	monkey_saddle->get_origin = &get_origin_monkey_saddle;
	monkey_saddle->move = &move_monkey_saddle;
	monkey_saddle->get_text_coordinate = &get_text_coordinate_monkey_saddle;	
//	add_object(monkey_saddle, data);
	return (ret);
}

int		parse_cyclide(char **line, t_obj *cyclide)
{
	char	stripe;
	int		ret;
	t_cyclide	*cyclide_param;

	stripe = 0;
	ret = 1;
	if (cyclide->obj_param)
		return (error(ALREADYOBJ, NULL));
	if (!(cyclide_param = ft_memalloc(sizeof(t_cyclide))))
		return (0);
	while (stripe != '>' && ret != 0)
	{
		stripe = goto_next_element(line);
		if (!ft_strncmp_case(*line, "origin", 6))
			ret = parse_origin(line, &cyclide_param->origin, 6);
		else if (!ft_strncmp_case(*line, "param", 5))
			ret = parse_double(line, 5, &cyclide_param->param);
		else if (stripe == '<')
			return (syn_error(SERROR, CYCLIDE, ORIGIN, PARAM));
	}
	if (ret == 0)
		return (syn_error(SERROR, CYCLIDE, ORIGIN, PARAM));
//	if (ft_fabs(cyclide_param->radius) == 0.f)
//		return (syn_error(SERROR, SPHERE, ORIGIN, RADIUS, NULL));
//	cyclide_param->translat_mat = build_translation_matrix(cyclide_param->origin, cyclide_param->x_axis, cyclide_param->y_axis, cyclide_param->z_axis);
	cyclide->obj_param = cyclide_param;
	cyclide->obj_type = OBJ_CYCLIDE;
	cyclide->check_inside = &check_inside_cyclide;
	cyclide->ray_intersect = &ray_intersect_cyclide;
	cyclide->get_normal_inter = &get_normal_intersect_cyclide;
	cyclide->get_origin = &get_origin_cyclide;
	cyclide->move = &move_cyclide;
	cyclide->get_text_coordinate = &get_text_coordinate_cyclide;	
//	add_object(cyclide, data);
	return (ret);
}

int		parse_fermat(char **line, t_obj *fermat)
{
	char	stripe;
	int		ret;
	t_fermat	*fermat_param;

	stripe = 0;
	ret = 1;
	if (fermat->obj_param)
		return (error(ALREADYOBJ, NULL));
	if (!(fermat_param = ft_memalloc(sizeof(t_fermat))))
		return (0);
	while (stripe != '>' && ret != 0)
	{
		stripe = goto_next_element(line);
		if (!ft_strncmp_case(*line, "origin", 6))
			ret = parse_origin(line, &fermat_param->origin, 6);
		else if (stripe == '<')
			return (syn_error(SERROR, FERMAT, ORIGIN, NULL));
	}
	if (ret == 0)
		return (syn_error(SERROR, FERMAT, ORIGIN, NULL));
//	if (ft_fabs(fermat_param->radius) == 0.f)
//		return (syn_error(SERROR, SPHERE, ORIGIN, RADIUS, NULL));
//	fermat_param->translat_mat = build_translation_matrix(fermat_param->origin, fermat_param->x_axis, fermat_param->y_axis, fermat_param->z_axis);
	fermat->obj_param = fermat_param;
	fermat->obj_type = OBJ_FERMAT;
	fermat->check_inside = &check_inside_fermat;
	fermat->ray_intersect = &ray_intersect_fermat;
	fermat->get_normal_inter = &get_normal_intersect_fermat;
	fermat->get_origin = &get_origin_fermat;
	fermat->move = &move_fermat;
	fermat->get_text_coordinate = &get_text_coordinate_fermat;	
//	add_object(fermat, data);
	return (ret);
}

int		parse_ellipsoid(char **line, t_obj *ellipsoid)
{
	char	stripe;
	int		ret;
	t_ellipsoid	*ellipsoid_param;

	stripe = 0;
	ret = 1;
	if (ellipsoid->obj_param)
		return (error(ALREADYOBJ, NULL));
	if (!(ellipsoid_param = ft_memalloc(sizeof(t_ellipsoid))))
		return (0);
	while (stripe != '>' && ret != 0)
	{
		stripe = goto_next_element(line);
		if (!ft_strncmp_case(*line, "origin", 6))
			ret = parse_origin(line, &ellipsoid_param->origin, 6);
		else if (!ft_strncmp_case(*line, "x_axis", 6))
			ret = parse_double(line, 6, &ellipsoid_param->x_fact);
		else if (!ft_strncmp_case(*line, "y_axis", 6))
			ret = parse_double(line, 6, &ellipsoid_param->y_fact);
		else if (!ft_strncmp_case(*line, "z_axis", 6))
			ret = parse_double(line, 6, &ellipsoid_param->z_fact);
		else if (stripe == '<')
			return (syn_error(SERROR, ELLIPSE, AXIS, NULL));
	}
	if (ret == 0)
		return (syn_error(SERROR, ELLIPSE, AXIS, NULL));
//	if (ft_fabs(ellipsoid_param->radius) == 0.f)
//		return (syn_error(SERROR, SPHERE, ORIGIN, RADIUS, NULL));
//	ellipsoid_param->translat_mat = build_translation_matrix(ellipsoid_param->origin, ellipsoid_param->x_axis, ellipsoid_param->y_axis, ellipsoid_param->z_axis);
	ellipsoid->obj_param = ellipsoid_param;
	ellipsoid->obj_type = OBJ_ELLIPSOID;
	ellipsoid->check_inside = &check_inside_ellipsoid;
	ellipsoid->ray_intersect = &ray_intersect_ellipsoid;
	ellipsoid->get_normal_inter = &get_normal_intersect_ellipsoid;
	ellipsoid->get_origin = &get_origin_ellipsoid;
	ellipsoid->move = &move_ellipsoid;
	ellipsoid->get_text_coordinate = &get_text_coordinate_ellipsoid;	
//	add_object(ellipsoid, data);
	return (ret);
}

int		parse_hyperboloid(char **line, t_obj *hyperboloid)
{
	char	stripe;
	int		ret;
	t_hyperboloid	*hyperboloid_param;

	stripe = 0;
	ret = 1;
	if (hyperboloid->obj_param)
		return (error(ALREADYOBJ, NULL));
	if (!(hyperboloid_param = ft_memalloc(sizeof(t_hyperboloid))))
		return (0);
	while (stripe != '>' && ret != 0)
	{
		stripe = goto_next_element(line);
		if (!ft_strncmp_case(*line, "origin", 6))
			ret = parse_origin(line, &hyperboloid_param->origin, 6);
		else if (!ft_strncmp_case(*line, "x_axis", 6))
			ret = parse_double(line, 6, &hyperboloid_param->x_fact);
		else if (!ft_strncmp_case(*line, "y_axis", 6))
			ret = parse_double(line, 6, &hyperboloid_param->y_fact);
		else if (!ft_strncmp_case(*line, "z_axis", 6))
			ret = parse_double(line, 6, &hyperboloid_param->z_fact);
		else if (!ft_strncmp_case(*line, "surface", 7))
			ret = parse_int(line, 7, &hyperboloid_param->surface);
		else if (stripe == '<')
			return (syn_error(SERROR, HYPERBOL, AXIS, SURFACE));
	}
	if (ret == 0)
		return (syn_error(SERROR, HYPERBOL, AXIS, SURFACE));
//	if (ft_fabs(hyperboloid_param->radius) == 0.f)
//		return (syn_error(SERROR, SPHERE, ORIGIN, RADIUS, NULL));
//	hyperboloid_param->translat_mat = build_translation_matrix(hyperboloid_param->origin, hyperboloid_param->x_axis, hyperboloid_param->y_axis, hyperboloid_param->z_axis);
	if (!hyperboloid_param->surface)
		hyperboloid_param->surface = -1;
	else
		hyperboloid_param->surface = 1;
	hyperboloid->obj_param = hyperboloid_param;
	hyperboloid->obj_type = OBJ_HYPERBOLOID;
	hyperboloid->check_inside = &check_inside_hyperboloid;
	hyperboloid->ray_intersect = &ray_intersect_hyperboloid;
	hyperboloid->get_normal_inter = &get_normal_intersect_hyperboloid;
	hyperboloid->get_origin = &get_origin_hyperboloid;
	hyperboloid->move = &move_hyperboloid;
	hyperboloid->get_text_coordinate = &get_text_coordinate_hyperboloid;	
//	add_object(hyperboloid, data);
	return (ret);
}

int		parse_cone(char **line, t_obj *cone)
{
	char	stripe;
	int		ret;
	t_cone	*cone_param;

	stripe = 0;
	ret = 1;
	if (cone->obj_param)
		return (error(ALREADYOBJ, NULL));
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
			ret = parse_double(line, 6, &cone_param->radius);
		else if (stripe == '<')
			return (syn_error(SERROR, CONE, TIP, RADIUS));
	}
	if ((ft_fabs(cone_param->radius) == 0.f || is_null_3vecf(sub_3vecf(cone_param->center, cone_param->tip))) || ret == 0)
		return (syn_error(SERROR, CONE, TIP, RADIUS));
	cone->obj_param = cone_param;
	cone->obj_type = OBJ_CONE;
	cone->ray_intersect = &ray_intersect_cone;
	cone->check_inside = &check_inside_cone;
	cone->get_normal_inter = &get_normal_intersect_cone;
	cone->get_origin = &get_origin_cone;
	cone->move = &move_cone;
	cone->rotate = &rotate_cone;
	cone->get_text_coordinate = &get_text_coordinate_cone;
//	add_object(cone, data);
	return (ret);
}

int		parse_moebius(char **line, t_obj *moebius)
{
	char	stripe;
	int		ret;
	t_moebius	*moebius_param;

	stripe = 0;
	ret = 1;
	if (moebius->obj_param)
		return (error(ALREADYOBJ, NULL));
	if (!(moebius_param = ft_memalloc(sizeof(t_moebius))))
		return (0);
	while (stripe != '>' && ret != 0)
	{
		stripe = goto_next_element(line);
		if (!ft_strncmp_case(*line, "origin", 6))
			ret = parse_origin(line, &moebius_param->origin, 6);
		else if (!ft_strncmp_case(*line, "radius", 6))
			ret = parse_double(line, 6, &moebius_param->radius);
		else if (!ft_strncmp_case(*line, "half_width", 10))
			ret = parse_double(line, 10, &moebius_param->half_width);
		else if (stripe == '<')
			return (syn_error(SERROR, MOEBIUS, RADIUS, HALFWIDTH));
	}
	if ((moebius_param->radius <= 0.f || moebius_param->half_width <= 0.f) || ret == 0)
		return (syn_error(SERROR, MOEBIUS, RADIUS, HALFWIDTH));
	moebius->obj_param = moebius_param;
	moebius->obj_type = OBJ_MOEBIUS;
	moebius->check_inside = &check_inside_moebius;
	moebius->ray_intersect = &ray_intersect_moebius;
	moebius->get_normal_inter = &get_normal_intersect_moebius;
	moebius->get_origin = &get_origin_moebius;
	moebius->move = &move_moebius;
	moebius->get_text_coordinate = &get_text_coordinate_moebius;
//	add_object(moebius, data);
//	moebius->data = data;
	return (ret);
}

int		check_skybox(t_data *data)
{
	t_sphere	*param;
	t_text_img	*image;
	t_obj		*sky;

	if (!data->skybox_name)
		return (1);
	if (!(image = parse_img(data->skybox_name)))
		return (0);
	if (!(sky = ft_memalloc(sizeof(t_obj))))
		return (0);
	if (!(param = ft_memalloc(sizeof(t_sphere))))
		return (0);
	sky->text.text_type = TEXT_IMAGE;
	sky->text.scale.val[0] = 1;
	sky->text.scale.val[1] = 1;
	sky->text.text_param = image;
	param->origin = assign_3vecf(0, 0, 0);
	param->radius = 10000;
	sky->obj_param = param;
	sky->obj_type = OBJ_SKYBOX;
	sky->check_inside = &check_inside_sphere;
	sky->ray_intersect = &ray_intersect_sphere;
	sky->get_normal_inter = &get_normal_intersect_sphere;
	sky->get_text_color = &get_image_color;
	sky->get_origin = &get_origin_sphere;
	sky->move = &move_sphere;
	sky->rotate = &rotate_sphere;
	sky->get_text_coordinate = &get_text_coordinate_sphere;
	add_object(sky, data);
	return (1);

}
