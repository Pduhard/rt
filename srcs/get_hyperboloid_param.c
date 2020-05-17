#include "rt.h"


t_2vecf	get_text_coordinate_hyperboloid(t_3vecf inter_point,
	t_3vecf normal_inter, t_obj *hyperboloid)
{
	return ((t_2vecf){{0, 0}});
	(void)inter_point;
	(void)hyperboloid;
	(void)normal_inter;
}

t_3vecf	get_origin_hyperboloid(t_obj *hyperboloid) // bye
{
	return (((t_hyperboloid *)hyperboloid->obj_param)->origin);
}

t_3vecf	get_hyperboloid_origin(t_obj *hyperboloid,
	t_hyperboloid *hyperboloid_param, int sp_id)
{
	if (sp_id)
		return
		(move_3vecf(hyperboloid_param->origin, hyperboloid->motions, sp_id));
	return (hyperboloid_param->origin);
}

t_3vecf	get_normal_intersect_hyperboloid(t_3vecf inter_point,
	t_obj *hyperboloid, int sp_id)
{
	t_hyperboloid	*param;
	t_3vecf			normal_inter;
	t_3vecf			hyperboloid_origin;
	t_3vecf			cst;

	param = (t_hyperboloid *)hyperboloid->obj_param;
	hyperboloid_origin = get_hyperboloid_origin(hyperboloid, param, sp_id);
	cst = sub_3vecf(inter_point, hyperboloid_origin);
	normal_inter.val[0] = (2 * cst.val[0]) / (param->x_fact * param->x_fact);
	normal_inter.val[1] = (-2 * cst.val[1]) / (param->y_fact * param->y_fact);
	normal_inter.val[2] = (2 * cst.val[2]) / (param->z_fact * param->z_fact);
	normalize_3vecf(&normal_inter);
	return (normal_inter);
}
