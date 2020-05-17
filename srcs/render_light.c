#include "rt.h"

static void    get_light_dir(t_3vecf *light_dir, double *light_len, t_light *lights, t_3vecf inter_point)
{
	if (lights->light_type == LIGHT_POINT)
	{
		*light_dir = sub_3vecf(lights->param, inter_point);
		*light_len = get_length_3vecf(*light_dir);
	}
	else if (lights->light_type == LIGHT_DIRECTIONAL)
	{
		*light_dir = neg_3vecf(lights->param);
		// assign_3vecf(-lights->param.val[0], -lights->param.val[1], -lights->param.val[2]);
		*light_len = MAX_VIEW;
	}
	normalize_3vecf(light_dir);// same
}


static t_3vecf    add_diffuse(t_compute_light_param p,
	t_3vecf transp_fact, t_3vecf light_dir)
{
	double	dp_n_ldir;
	t_3vecf	diffuse;

	dp_n_ldir = dot_product_3vecf(p.normal_inter, light_dir);
	if (dp_n_ldir > 0)
	{
		diffuse.val[0] = p.lights->color.val[0] * transp_fact.val[0] * dp_n_ldir;
		diffuse.val[1] = p.lights->color.val[1] * transp_fact.val[1] * dp_n_ldir;
		diffuse.val[2] = p.lights->color.val[2] * transp_fact.val[2] * dp_n_ldir;
	}
	else
		return (assign_3vecf(0, 0, 0));
	return (diffuse);
}

static t_3vecf add_specular(t_compute_light_param p, t_3vecf transp_fact,
	t_3vecf light_dir, t_3vecf inv_dir)
{
	t_3vecf spec_vec;
	t_3vecf specular;
	double	ref_dot_idir;
	double  spec_fact;

	if (p.shininess)
	{
		spec_vec = reflect_ray(light_dir, p.normal_inter);
		ref_dot_idir = dot_product_3vecf(spec_vec, inv_dir);
		if (ref_dot_idir > 0)
		{
			spec_fact = powf(ref_dot_idir / (get_length_3vecf(spec_vec)
				* get_length_3vecf(inv_dir)), p.shininess);
			specular.val[0] = p.lights->color.val[0] * transp_fact.val[0] * spec_fact;
			specular.val[1] = p.lights->color.val[1] * transp_fact.val[1] * spec_fact;
			specular.val[2] = p.lights->color.val[2] * transp_fact.val[2] * spec_fact;
			return (specular);
		}
	}
	return (assign_3vecf(0, 0, 0));
}

static t_3vecf add_light_effect(t_compute_light_param p, t_3vecf light_fact)
{
	if (p.data->caustics_gi)
		light_fact = add_3vecf(light_fact,
			compute_global_illumination(p.inter_point, p.normal_inter,
				p.data->caustic_map, NN_CAUSTIC_PHOTON_MAX));
	if (p.data->indirect_gi)
		light_fact = add_3vecf(light_fact,
			compute_global_illumination(p.inter_point, p.normal_inter,
				p.data->indirect_map, NN_INDIRECT_PHOTON_MAX));
	if (p.data->cel_shading)
	{
		cel_shade(&(light_fact.val[0]));
		cel_shade(&(light_fact.val[1]));
		cel_shade(&(light_fact.val[2]));
	}
	return (light_fact);
}

t_3vecf	compute_lights(t_compute_light_param p)
{
	t_3vecf	light_fact;
	t_3vecf	transp_fact;
	double	light_len;
	t_3vecf	light_dir;
	t_obj		*shadow_obj;

	light_fact = assign_3vecf(0, 0, 0);
	while (p.lights)
	{
		if (p.lights->light_type == LIGHT_AMBIENT)
			light_fact = add_3vecf(light_fact, p.lights->color);
		else
		{
			get_light_dir(&light_dir, &light_len, p.lights, p.inter_point);
			shadow_obj = check_for_shadow(&transp_fact, light_dir, light_len, &p);
			clamp_transparency(&transp_fact);
			if (!shadow_obj && (transp_fact.val[0] ||
				transp_fact.val[1] || transp_fact.val[2]))
				light_fact = add_3vecf(light_fact, add_3vecf(
					add_diffuse(p, transp_fact, light_dir),
					add_specular(p, transp_fact, light_dir, neg_3vecf(p.dir))));
		}
		p.lights = p.lights->next;
	}
	return (add_light_effect(p, light_fact));
}
