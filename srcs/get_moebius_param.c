#include "rt.h"

t_2vecf	get_text_coordinate_moebius(t_3vecf inter_point,
	t_3vecf normal_inter, t_obj *moebius)
{
	t_2vecf		text_coord;
	double		sin_v_2;
	t_moebius	*param;

	param = (t_moebius *)moebius->obj_param;
	inter_point = sub_3vecf(inter_point, param->origin);
	text_coord.val[1] = atan2(inter_point.val[1], inter_point.val[0]);
	if (!is_null((sin_v_2 = sin(text_coord.val[1] / 2))))
		text_coord.val[0] = inter_point.val[2] / sin_v_2;
	else if (!is_null(text_coord.val[1]))
		text_coord.val[0] = (inter_point.val[0] / cos(text_coord.val[1])
			- param->radius) / cos(text_coord.val[1] / 2);
	else
		text_coord.val[0] = (inter_point.val[0] - param->radius);
	text_coord.val[0] = (text_coord.val[0] + param->half_width)
		/ (2 * param->half_width);
	text_coord.val[1] = (text_coord.val[1] + M_PI) / (M_PI * 2);
	return (text_coord);
	(void)normal_inter;
}


t_3vecf	get_origin_moebius(t_obj *moebius)
{
	return (((t_moebius *)moebius->obj_param)->origin);
}

t_3vecf	get_moebius_origin(t_obj *moebius, t_moebius *param, int sp_id)
{
	if (sp_id)
		return (move_3vecf(param->origin, moebius->motions, sp_id));
	return (param->origin);
}

t_3vecf	get_normal_intersect_moebius(t_3vecf inter_point,
	t_obj *moebius, int sp_id)
{
	t_moebius	*param;
	t_3vecf		normal_inter;
	double		c[3];
	t_3vecf		moebius_origin;

	param = (t_moebius *)moebius->obj_param;
	moebius_origin = get_moebius_origin(moebius, param, sp_id);
	c[0] = inter_point.val[0] - moebius_origin.val[0];
	c[1] = inter_point.val[1] - moebius_origin.val[1];
	c[2] = inter_point.val[2] - moebius_origin.val[2];
	normal_inter.val[0] = 2 * c[0] * c[1]
		- 2 * param->radius * c[2]
			- 4 * c[0] * c[2];
	normal_inter.val[1] = -param->radius * param->radius
		+ c[0] * c[0] + 3 * c[1] * c[1]
			- 4 * c[1] * c[2] + c[2] * c[2];
	normal_inter.val[2] = -2 * param->radius * c[0] - 2 * c[0] * c[0]
			- 2 * c[1] * c[1] + 2 * c[1] * c[2];
	normalize_3vecf(&normal_inter);
	return (normal_inter);
}
