#include "rt.h"

t_2vecf	get_text_coordinate_sphere(t_3vecf inter_point,
	t_3vecf normal_inter, t_obj *sphere)
{
	t_2vecf	text_coord;

	text_coord.val[1] = (1 - fmod((atan2(normal_inter.val[0],
		normal_inter.val[2]) / (2 * M_PI) + 0.5), 1));
	text_coord.val[0] = (normal_inter.val[1] * 0.5 + 0.5);
	return (text_coord);
	(void)inter_point;
	(void)sphere;
}


t_3vecf	get_origin_sphere(t_obj *sphere)
{
	return (((t_sphere *)sphere->obj_param)->origin);
}

t_3vecf	get_sphere_origin(t_obj *sphere, t_sphere *sphere_param, int sp_id)
{
	if (sp_id)
		return (move_3vecf(sphere_param->origin, sphere->motions, sp_id));
	return (sphere_param->origin);
}

t_3vecf	get_normal_intersect_sphere(t_3vecf inter_point,
	t_obj *sphere, int sp_id)
{
	t_sphere	*param;
	t_3vecf		sphere_origin;
	t_3vecf		normal;

	param = (t_sphere *)sphere->obj_param;
	sphere_origin = get_sphere_origin(sphere, param, sp_id);
	normal = sub_3vecf(inter_point, sphere_origin);
	normalize_3vecf(&normal);
	return (normal);
}
