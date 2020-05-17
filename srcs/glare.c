#include "rt.h"

static t_3vecf   add_glare_effect(t_leq l, t_3vecf light_pos, double light_dist)
{
	double fact;
	t_3vecf	light_npoint;
	double	lndist;

	light_npoint = add_3vecf(l.orig, product_c3vecf(l.dir, light_dist));
	lndist = get_length_3vecf(sub_3vecf(light_pos, light_npoint));
	if (lndist < 0.2)
		fact = (0.2 - lndist) * 5;
	else
		return (assign_3vecf(0, 0, 0));
	return (assign_3vecf(fact, fact, fact));
}

t_3vecf		compute_glare(t_leq l, t_light *lights, t_3vecf *inter_point)
{
	t_3vecf	glare;
	t_3vecf	light_dir;
	t_3vecf	light_pos;
	double	obj_dist;
	double	light_dist;

	obj_dist = inter_point ?
		get_length_3vecf(sub_3vecf(*inter_point, l.orig)) : MAX_VIEW;
	light_pos = assign_3vecf(0, 0, 0);
	glare = assign_3vecf(0, 0, 0);
	while (lights)
	{
		if (lights->light_type == LIGHT_POINT)
		{
			light_dir = sub_3vecf(lights->param, l.orig);
			light_dist = get_length_3vecf(light_dir);
			normalize_3vecf(&light_dir);
			light_pos = lights->param;

			if (light_dist < obj_dist)
				glare = add_3vecf(glare, add_glare_effect(l, light_pos, light_dist));
		}
		lights = lights->next;
	}
	return (glare);
}

t_3vecf add_color_effect(t_data *data, t_clre_param p,
	t_3vecf lighted_color, t_inter i)
{
	double	fog_fact;

	if (data->fog.val[0] || data->fog.val[1])
	{
		if (p.closest_dist < data->fog.val[0])
			fog_fact = 1;
		else if (p.closest_dist < data->fog.val[1])
			fog_fact = (data->fog.val[1] - p.closest_dist)
				/ (data->fog.val[1] - data->fog.val[0]);
		else
			fog_fact = 0;
		lighted_color.val[0] = 1 * (1 - fog_fact) + lighted_color.val[0] * fog_fact;
		lighted_color.val[1] = 1 * (1 - fog_fact) + lighted_color.val[1] * fog_fact;
		lighted_color.val[2] = 1 * (1 - fog_fact) + lighted_color.val[2] * fog_fact;
	}
	if (p.depth == RAY_DEPTH)
		return (add_3vecf(lighted_color,
			compute_glare(p.l, data->lights, &(i.inter_point))));
	return (lighted_color);
}
