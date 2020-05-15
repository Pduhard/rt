/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cylinder.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aplat <aplat@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/15 17:45:12 by aplat             #+#    #+#             */
/*   Updated: 2020/05/15 17:53:52 by aplat            ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

/*
** http://www.illusioncatalyst.com/notes_files/mathematics/
**	line_cylinder_intersection.php
*/

int		check_inside_cylinder(t_3vecf inter_point, t_obj *cylinder)
{
	t_cylinder	*param;
	t_3vecf		tp;
	t_3vecf		cp;

	param = (t_cylinder *)cylinder->obj_param;
	tp = sub_3vecf(inter_point, param->tip);
	cp = sub_3vecf(inter_point, param->center);
	if (get_length_3vecf(product_3vecf(tp, cp))
		/ get_length_3vecf(sub_3vecf(param->tip, param->center))
			> param->radius)
		return (0);
	return (1);
}

void	assign_cylinder_function(t_obj *cylinder)
{
	cylinder->obj_type = OBJ_CYLINDER;
	cylinder->check_inside = &check_inside_cylinder;
	cylinder->ray_intersect = &ray_intersect_cylinder;
	cylinder->get_normal_inter = &get_normal_intersect_cylinder;
	cylinder->get_origin = &get_origin_cylinder;
	cylinder->move = &move_cylinder;
	cylinder->rotate = &rotate_cylinder;
	cylinder->get_text_coordinate = &get_text_coordinate_cylinder;
	// cylinder->get_text_color = &get_uni_color;
}

void	generate_new_cylinder(t_data *data)
{
	t_obj		*cylinder;
	t_cylinder	*param;
	t_3vecf		dir;

	dir = mult_3vecf_33matf(mult_3vecf_33matf(window_to_view(0, 0,
		data->size.val[0], data->size.val[1]), data->rot_mat[1]),
			data->rot_mat[0]);
	normalize_3vecf(&dir);
	if (!(cylinder = ft_memalloc(sizeof(t_obj))))
		return ;
	if (!(param = ft_memalloc(sizeof(t_cylinder))))
		return ;
	param->radius = get_random_number(time(NULL)) * 1.5;
	param->tip = add_3vecf(data->camera->origin, product_c3vecf(dir, 2));
	param->center = add_3vecf(assign_3vecf(0, 1, 0), param->tip);
	cylinder->obj_param = param;
	assign_cylinder_function(cylinder);
	cylinder->text = generate_random_texture(cylinder);
	set_bump_own(cylinder);
	add_object(cylinder, data);
	data->new_obj = 1;
}

t_2vecf	get_text_coordinate_cylinder(t_3vecf inter_point, t_3vecf normal_inter,
	t_obj *cylinder)
{
	t_2vecf		text_coord;
	t_3vecf		cp;
	t_3vecf		cyl_axis[3];
	t_cylinder	*param;

	param = (t_cylinder *)cylinder->obj_param;
	get_uv_axis(cyl_axis, sub_3vecf(param->tip, param->center));
	cp = sub_3vecf(inter_point, param->center);
	text_coord.val[0] = (1 - dot_product_3vecf(cyl_axis[1], cp))
		/ ((2 * M_PI) * param->radius);
	text_coord.val[1] = (atan2(dot_product_3vecf(cyl_axis[0], cp),
		dot_product_3vecf(cyl_axis[2], cp))) / (2 * M_PI);
	return (text_coord);
	(void)normal_inter;
}

void	move_cylinder(t_obj *cylinder, t_3vecf dir, double fact)
{
	t_cylinder	*param;
	t_cut		*cuts;

	param = (t_cylinder *)cylinder->obj_param;
	param->center.val[0] += dir.val[0] * fact;
	param->center.val[1] += dir.val[1] * fact;
	param->center.val[2] += dir.val[2] * fact;
	param->tip.val[0] += dir.val[0] * fact;
	param->tip.val[1] += dir.val[1] * fact;
	param->tip.val[2] += dir.val[2] * fact;
	cuts = cylinder->cuts;
	while (cuts)
	{
		if (cuts->move && cuts->cut_type != CUT_STATIC)
			cuts->move(cuts, dir, fact);
		cuts = cuts->next;
	}
}

void	rotate_cylinder(t_obj *cylinder, t_3vecf orig, t_33matf rot_mat[2])
{
	t_cylinder	*param;
	t_cut		*cuts;

	param = (t_cylinder *)cylinder->obj_param;
	param->center = sub_3vecf(param->center, orig);
	param->tip = sub_3vecf(param->tip, orig);
	param->center = mult_3vecf_33matf(param->center, rot_mat[1]);
	param->center = mult_3vecf_33matf(param->center, rot_mat[0]);
	param->tip = mult_3vecf_33matf(param->tip, rot_mat[1]);
	param->tip = mult_3vecf_33matf(param->tip, rot_mat[0]);
	param->center = add_3vecf(param->center, orig);
	param->tip = add_3vecf(param->tip, orig);
	cuts = cylinder->cuts;
	while (cuts)
	{
		if (cuts->rotate && cuts->cut_type != CUT_STATIC)
			cuts->rotate(cuts, orig, rot_mat);
		cuts = cuts->next;
	}
}

t_3vecf	get_origin_cylinder(t_obj *cylinder) // a degager
{
	return (((t_cylinder *)cylinder->obj_param)->center);
}

t_3vecf	get_cylinder_origin(t_obj *cylinder, t_cylinder *cylinder_param,
	int sp_id)
{
	if (sp_id)
		return (move_3vecf(cylinder_param->center, cylinder->motions, sp_id));
	return (cylinder_param->center);
}

t_3vecf	get_cylinder_tip(t_obj *cylinder, t_cylinder *cylinder_param, int sp_id)
{
	if (sp_id)
		return (move_3vecf(cylinder_param->tip, cylinder->motions, sp_id));
	return (cylinder_param->tip);
}

t_3vecf	get_normal_intersect_cylinder(t_3vecf inter_point,
		t_obj *cylinder, int sp_id)
{
	t_3vecf	ch;
	t_3vecf	inter_proj;
	double	length_ch;
	double	step_inter_proj;
	t_3vecf	cylinder_origin;

	cylinder_origin = get_cylinder_origin(cylinder, cylinder->obj_param, sp_id);
	ch = sub_3vecf(get_cylinder_tip(cylinder, cylinder->obj_param, sp_id),
		cylinder_origin);
	length_ch = get_length_3vecf(ch);
	length_ch *= length_ch;
	step_inter_proj = dot_product_3vecf(ch,
			sub_3vecf(inter_point, cylinder_origin)) / length_ch;
	inter_proj = add_3vecf(cylinder_origin,
		product_c3vecf(ch, step_inter_proj));
	return (sub_3vecf(inter_proj, inter_point));
}

t_3vecf	get_cylinder_quadratic_cst(t_cylinder *cylinder_param,
	t_3vecf cylinder_origin, t_3vecf cylinder_tip, t_leq l)
{
	t_3vecf	h;
	double	dp_dh;
	double	dp_wh;
	t_3vecf	w;
	t_3vecf cst;

	h = sub_3vecf(cylinder_origin, cylinder_tip);
	normalize_3vecf(&h);
	w = sub_3vecf(l.orig, cylinder_origin);
	dp_dh = dot_product_3vecf(l.dir, h);
	dp_wh = dot_product_3vecf(w, h);
	cst.val[0] = dot_product_3vecf(l.dir, l.dir) - dp_dh * dp_dh;
	cst.val[1] = 2 * (dot_product_3vecf(l.dir, w) - dp_dh * dp_wh);
	cst.val[2] = dot_product_3vecf(w, w) - dp_wh * dp_wh
		- cylinder_param->radius * cylinder_param->radius;
	return (cst);
}

int		ray_intersect_cylinder(t_leq l, t_obj *cylinder, t_dist dist, int sp_id)
{
	t_3vecf		cst;
	int			check;
	t_cylinder	*cylinder_param;
	t_2vecf		roots;

	check = 0;
	cylinder_param = (t_cylinder *)cylinder->obj_param;
	cst = get_cylinder_quadratic_cst(
		cylinder_param,
		get_cylinder_origin(cylinder, cylinder->obj_param, sp_id),
		get_cylinder_tip(cylinder, cylinder->obj_param, sp_id),
		l);
	roots = solve_quadratic(cst.val[0], cst.val[1], cst.val[2]);
	check |= is_closest_intersect(dist, roots.val[0]);
	check |= is_closest_intersect(dist, roots.val[1]);
	return (check);
}
