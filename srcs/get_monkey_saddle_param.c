#include "rt.h"


t_2vecf	get_text_coordinate_monkey_saddle(t_3vecf inter_point,
	t_3vecf normal_inter, t_obj *monkey_saddle)
{
	return ((t_2vecf){{0, 0}});
	(void)normal_inter;
	(void)inter_point;
	(void)monkey_saddle;
}

t_3vecf	get_origin_monkey_saddle(t_obj *monkey_saddle)
{
	return (((t_monkey_saddle *)monkey_saddle->obj_param)->origin);
}

t_3vecf	get_monkey_saddle_origin(t_obj *monkey_saddle, t_monkey_saddle *param,
	int sp_id)
{
	if (sp_id)
		return (move_3vecf(param->origin, monkey_saddle->motions, sp_id));
	return (param->origin);
}

t_3vecf	get_normal_intersect_monkey_saddle(t_3vecf inter_point,
	t_obj *monkey_saddle, int sp_id)
{
	t_monkey_saddle	*param;
	t_3vecf			normal_inter;
	double			x;
	double			z;
	t_3vecf			monkey_saddle_origin;

	param = (t_monkey_saddle *)monkey_saddle->obj_param;
	monkey_saddle_origin = get_monkey_saddle_origin(monkey_saddle,
		param, sp_id);
	x = inter_point.val[0] - monkey_saddle_origin.val[0];
	z = inter_point.val[2] - monkey_saddle_origin.val[2];
	normal_inter.val[0] = 3 * x * x - 3 * z * z;
	normal_inter.val[1] = -1;
	normal_inter.val[2] = -6 * x * z;
	normalize_3vecf(&normal_inter);
	return (normal_inter);
}
