
#include "rt.h"

t_2vecf	get_text_coordinate_ellipsoid(t_3vecf inter_point,
	t_3vecf normal_inter, t_obj *ellipsoid)
{
	t_2vecf	text_coord;

	text_coord.val[1] = (1 - fmod((atan2(normal_inter.val[0],
		normal_inter.val[2])
		/ (2 * M_PI) + 0.5), 1));
	text_coord.val[0] = (normal_inter.val[1] * 0.5 + 0.5);
	return (text_coord);
	(void)inter_point;
	(void)ellipsoid;
}

t_3vecf	get_origin_ellipsoid(t_obj *ellipsoid)
{
	return (((t_ellipsoid *)ellipsoid->obj_param)->origin);
}

t_3vecf	get_ellipsoid_origin(t_obj *ellipsoid, t_ellipsoid *ellipsoid_param,
	int sp_id)
{
	if (sp_id)
		return (move_3vecf(ellipsoid_param->origin, ellipsoid->motions, sp_id));
	return (ellipsoid_param->origin);
}

t_3vecf	get_normal_intersect_ellipsoid(t_3vecf inter_point,
	t_obj *ellipsoid, int sp_id)
{
	t_ellipsoid	*param;
	t_3vecf		normal_inter;
	t_3vecf		cst;
	t_3vecf		ellipsoid_origin;

	param = (t_ellipsoid *)ellipsoid->obj_param;
	ellipsoid_origin = get_ellipsoid_origin(ellipsoid, param, sp_id);
	cst = sub_3vecf(inter_point, ellipsoid_origin);
	normal_inter.val[0] = (2 * cst.val[0]) / (param->x_fact * param->x_fact);
	normal_inter.val[1] = (2 * cst.val[1]) / (param->y_fact * param->y_fact);
	normal_inter.val[2] = (2 * cst.val[2]) / (param->z_fact * param->z_fact);
	normalize_3vecf(&normal_inter);
	return (normal_inter);
}
