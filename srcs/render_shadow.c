/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_shadow.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aplat <aplat@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/18 05:38:38 by aplat             #+#    #+#             */
/*   Updated: 2020/05/18 05:42:08 by aplat            ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

static void	update_transparency(t_3vecf *transp_fact, t_4vecf obj_color)
{
	transp_fact->val[0] -= ((1 - obj_color.val[3]) * (1.1 - obj_color.val[0]));
	transp_fact->val[1] -= ((1 - obj_color.val[3]) * (1.1 - obj_color.val[1]));
	transp_fact->val[2] -= ((1 - obj_color.val[3]) * (1.1 - obj_color.val[2]));
}

t_obj		*check_for_shadow(t_3vecf *transp_fact, t_3vecf light_dir,
	double light_len, t_compute_light_param *p)
{
	t_3vecf	inter_p;
	double	shadow_dist;
	t_obj	*shadow_obj;
	t_3vecf tex_normal_inter;
	t_4vecf	obj_color;

	inter_p = p->inter_point;
	*transp_fact = assign_3vecf(1, 1, 1);
	shadow_dist = MAX_VIEW;
	while ((shadow_obj = ray_first_intersect((t_leq){inter_p, light_dir},
		(t_dist){&shadow_dist, BIAS, light_len}, p->objs, p->sp_id)))
	{
		inter_p = add_3vecf(inter_p, product_c3vecf(light_dir, shadow_dist));
		light_len -= shadow_dist;
		tex_normal_inter = shadow_obj->get_normal_inter(inter_p, shadow_obj,
			p->sp_id);
		normalize_3vecf(&tex_normal_inter);
		obj_color = shadow_obj->get_text_color(inter_p, tex_normal_inter,
			shadow_obj);
		if (obj_color.val[3] > 0)
			update_transparency(transp_fact, obj_color);
		else
			return (shadow_obj);
	}
	return (NULL);
}
