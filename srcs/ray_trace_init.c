#include "rt.h"

static int		is_on_cell_boundary(t_3vecf origin, t_inter i,
	t_obj *closest_obj, int sp_id)
{
	t_3vecf	new_dir;
	t_3vecf	new_inter_point;
	double	dist;

	new_inter_point = add_3vecf(i.inter_point,
		product_c3vecf(i.normal_inter, CEL_BOUNDARY));
	new_dir = sub_3vecf(new_inter_point, origin);
	normalize_3vecf(&new_dir);
	dist = MAX_VIEW;
	if (closest_obj->ray_intersect((t_leq){origin, new_dir},
		closest_obj, (t_dist){&dist, 2 * CEL_BOUNDARY, MAX_VIEW}, sp_id))
		return (0);
	return (1);
}

static t_3vecf	mult_clr_light(t_4vecf obj_color, t_3vecf light_fact)
{
	t_3vecf lighted_color;

	lighted_color.val[0] = obj_color.val[0] * light_fact.val[0];
	lighted_color.val[1] = obj_color.val[1] * light_fact.val[1];
	lighted_color.val[2] = obj_color.val[2] * light_fact.val[2];
	return (lighted_color);
}

int	init_lighted_color(t_obj *closest_obj, t_inter i,
	t_leq l, t_ilc_p p)
{
	t_3vecf light_fact;

	if (closest_obj->obj_type == OBJ_SKYBOX)
		light_fact = assign_3vecf(1, 1, 1);
	else
		light_fact = compute_lights((t_compute_light_param){
			i.inter_point, i.normal_inter,
			l.dir, p.data->lights, p.data->objs,
			p.sp_id, p.data, closest_obj->shininess});
	*(p.lighted_color) = mult_clr_light(p.obj_color, light_fact);
	if (p.data->cel_shading && is_on_cell_boundary(l.orig,
		(t_inter){i.inter_point, i.normal_inter}, closest_obj, p.sp_id))
	{
		p.lighted_color->val[0] = 0;
		p.lighted_color->val[1] = 0;
		p.lighted_color->val[2] = 0;
		return (0);
	}
	return (1);
}


t_inter init_inter(t_leq l, double closest_dist, t_obj *closest_obj, int sp_id)
{
	t_inter i;

	i.inter_point = add_3vecf(l.orig, product_c3vecf(l.dir, closest_dist));
	i.normal_inter = closest_obj->get_normal_inter(i.inter_point,
		closest_obj, sp_id);
	normalize_3vecf(&i.normal_inter);
	return (i);
}
