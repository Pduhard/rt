/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   cone.c                                           .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: aplat <aplat@student.le-101.fr>            +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2019/12/30 18:21:18 by pduhard-     #+#   ##    ##    #+#       */
/*   Updated: 2020/03/12 20:35:39 by pduhard-         ###   ########lyon.fr   */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "rt.h"
// http://www.illusioncatalyst.com/notes_files/mathematics/line_cone_intersection.php

int		check_inside_cone(t_3vecf inter_point, t_obj *obj)
{
	t_cone	*param;
	double	c_dist;
	double	height;
	t_3vecf v[3];
	int		check_rev;

	check_rev = 0;
	param = (t_cone *)obj->obj_param;
	v[0] = sub_3vecf(param->center, param->tip);
	height = get_length_3vecf(v[0]);
	normalize_3vecf(&(v[0]));
	v[1] = sub_3vecf(inter_point, param->tip);
	c_dist = dot_product_3vecf(v[1], v[0]);
	if (c_dist < 0 && (check_rev = 1))
		c_dist *= -1;
	v[2].val[0] = v[1].val[0] - c_dist * (check_rev ? -v[0].val[0] : v[0].val[0]);
	v[2].val[1] = v[1].val[1] - c_dist * (check_rev ? -v[0].val[1] : v[0].val[1]);
	v[2].val[2] = v[1].val[2] - c_dist * (check_rev ? -v[0].val[2] : v[0].val[2]);
	if (get_length_3vecf(v[2]) > (c_dist / height) * param->radius)
		return (0);
	return (1);
}

void	get_cone_axis(t_cone *param, t_3vecf cone_axis[3])
{
		cone_axis[0] = assign_3vecf(0, 0, 0);
		cone_axis[1] = sub_3vecf(param->tip, param->center);
		normalize_3vecf(&(cone_axis[1]));
		if (cone_axis[1].val[0] != 0)
		{
			cone_axis[0] = assign_3vecf(0, 1, 1);
			cone_axis[0].val[0] = (-cone_axis[1].val[1] - cone_axis[1].val[2])
				/ cone_axis[1].val[0];
		}
		else if (cone_axis[1].val[1] != 0)
		{
			cone_axis[0] = assign_3vecf(1, 0, 1);
			cone_axis[0].val[1] = (-cone_axis[1].val[0] - cone_axis[1].val[2])
				/ cone_axis[1].val[1];
		}
		else if (cone_axis[1].val[2] != 0)
		{
			cone_axis[0] = assign_3vecf(1, 1, 0);
			cone_axis[0].val[2] = (-cone_axis[1].val[0] - cone_axis[1].val[1])
				/ cone_axis[1].val[2];
		}
		normalize_3vecf(&(cone_axis[0]));
		cone_axis[2] = product_3vecf(cone_axis[0], cone_axis[1]);
}

t_2vecf	get_text_coordinate_cone(t_3vecf inter_point,
		t_3vecf normal_inter, t_obj *cone)
{
	t_2vecf	text_coord;
	t_3vecf	cp;
	t_3vecf	cone_axis[3];
	t_cone	*param;

	param = (t_cone *)cone->obj_param;
	get_cone_axis(param, cone_axis);
	cp = sub_3vecf(inter_point, param->center);
	text_coord.val[0] = -dot_product_3vecf(cone_axis[1], cp) / (M_PI * 2);
	text_coord.val[1] = atan2(dot_product_3vecf(cone_axis[0], cp),
		dot_product_3vecf(cone_axis[2], cp)) / (M_PI * 2);
	return (text_coord);
	(void)normal_inter;
}

void	move_cone(t_obj *cone, t_3vecf dir, double fact)
{
	t_cone	*param;
	t_cut	*cuts;

	param = (t_cone *)cone->obj_param;
	param->center.val[0] += dir.val[0] * fact;
	param->center.val[1] += dir.val[1] * fact;
	param->center.val[2] += dir.val[2] * fact;
	param->tip.val[0] += dir.val[0] * fact;
	param->tip.val[1] += dir.val[1] * fact;
	param->tip.val[2] += dir.val[2] * fact;
	cuts = cone->cuts;
	while (cuts)
	{
		if (cuts->move && cuts->cut_type != CUT_STATIC)
			cuts->move(cuts, dir, fact);
		cuts = cuts->next;
	}
}

void   rotate_cone(t_obj *cone, t_3vecf orig, t_33matf rot_mat[2])
{
	t_cone *param;
	t_cut   *cuts;

	param = (t_cone *)cone->obj_param;
	param->center = sub_3vecf(param->center, orig);
	param->tip = sub_3vecf(param->tip, orig);
	param->center = mult_3vecf_33matf(param->center, rot_mat[1]);
	param->center = mult_3vecf_33matf(param->center, rot_mat[0]);
	param->tip = mult_3vecf_33matf(param->tip, rot_mat[1]);
	param->tip = mult_3vecf_33matf(param->tip, rot_mat[0]);
	param->center = add_3vecf(param->center, orig);
	param->tip = add_3vecf(param->tip, orig);
	cuts = cone->cuts;
	while (cuts)
	{
		if (cuts->rotate && cuts->cut_type != CUT_STATIC)
			cuts->rotate(cuts, orig, rot_mat);
		cuts = cuts->next;
	}
}

t_3vecf	get_origin_cone(t_obj *cone)
{
	return (((t_cone *)cone->obj_param)->tip);
}

t_3vecf	get_normal_intersect_cone(t_3vecf inter_point, t_obj *cone, int sp_id)
{
	double	intersect;
	t_3vecf	h;

	t_cone *cone_param;
	t_3vecf	hp;
	t_3vecf	cp;
	t_3vecf	tang;
	t_3vecf	normal;
	t_3vecf	cone_tip;
	t_3vecf	cone_origin;

	cone_param = (t_cone *)cone->obj_param;
	cone_origin = sp_id ? move_3vecf(cone_param->center, cone->motions, sp_id)
		: cone_param->center;
	cone_tip = sp_id ? move_3vecf(cone_param->tip, cone->motions, sp_id)
		: cone_param->tip;
	h = sub_3vecf(cone_origin, cone_tip);
	intersect = dot_product_3vecf(sub_3vecf(inter_point, cone_tip), h);
	hp = sub_3vecf(cone_tip, inter_point);
	cp = sub_3vecf(cone_origin, inter_point);
	tang = product_3vecf(hp, cp);
	normal = product_3vecf(hp, tang);
	return (intersect < 0 ? assign_3vecf(-normal.val[0],
			-normal.val[1], -normal.val[2]) : normal);
}

int	ray_intersect_cone(t_leq l, /*t_3vecf orig, t_3vecf dir, */t_obj *cone,
		t_dist dist, /*double *dist, double min_dist, double max_dist, */int sp_id)
{
	double	m;
	t_3vecf	h;
	t_3vecf	norm_h;
	double	h_length;
	double	a;
	double	b;
	double	c;
	int			check = 0;
	t_cone	*cone_param;
	t_3vecf	cone_tip;
	t_3vecf	cone_origin;

	(void)sp_id;
	/*	(void)min_dist;
		(void)max_dist;
		*/
	cone_param = (t_cone *)cone->obj_param;
	cone_origin = sp_id ? move_3vecf(cone_param->center, cone->motions, sp_id) : cone_param->center;
	cone_tip = sp_id ? move_3vecf(cone_param->tip, cone->motions, sp_id) : cone_param->tip;
	h = sub_3vecf(cone_origin, cone_tip);
	norm_h = h;
	normalize_3vecf(&norm_h);
	h_length = get_length_3vecf(h);
	m = (cone_param->radius * cone_param->radius) / (h_length * h_length);

	double	dp_dir_norm_h;
	double	dp_w_norm_h;
	t_3vecf	w;

	w = sub_3vecf(l.orig, cone_tip);
	dp_dir_norm_h = dot_product_3vecf(l.dir, norm_h);
	dp_w_norm_h = dot_product_3vecf(w, norm_h);

	a = dot_product_3vecf(l.dir, l.dir) - m * dp_dir_norm_h * dp_dir_norm_h - dp_dir_norm_h * dp_dir_norm_h;
	b = 2 * (dot_product_3vecf(l.dir, w) - m * dp_dir_norm_h * dp_w_norm_h - dp_dir_norm_h * dp_w_norm_h);
	c = dot_product_3vecf(w, w) - m * dp_w_norm_h * dp_w_norm_h - dp_w_norm_h * dp_w_norm_h;

	double	delta;
	delta = b * b - 4 * a * c;
	t_2vecf	hit_point;

	if (delta > 0)
	{
		//h = sub_3vecf(cone_param->center, cone_param->tip);
		// doubleing point exception ???
		hit_point.val[0] = (-b + sqrtf(delta)) / (2 * a);
		hit_point.val[1] = (-b - sqrtf(delta)) / (2 * a);
		if (hit_point.val[0] < *(dist.dist) && hit_point.val[0] > dist.min_dist && hit_point.val[0] < dist.max_dist)
		{
			check = 1;
			*(dist.dist) = hit_point.val[0];
		}
		if (hit_point.val[1] < *(dist.dist) && hit_point.val[1] > dist.min_dist && hit_point.val[1] < dist.max_dist)
		{
			check = 1;
			*(dist.dist) = hit_point.val[1];
		}
		return (check);
	}
	return (0);
}

void	generate_new_cone(t_data *data)
{
	t_obj		*cone;
	t_cone		*param;
	t_3vecf		dir;
//	int			rd;

	dir = mult_3vecf_33matf(mult_3vecf_33matf(window_to_view(0, 0, data->size.val[0], data->size.val[1]), data->rot_mat[1]), data->rot_mat[0]);
	normalize_3vecf(&dir);
	if (!(cone = ft_memalloc(sizeof(t_obj))))
		return ;
	if (!(param = ft_memalloc(sizeof(t_cone))))
		return ;
	param->radius = get_random_number(time(NULL)) * 1.5;
	param->tip.val[0] = data->camera->origin.val[0] + dir.val[0] * 2;
	param->tip.val[1] = data->camera->origin.val[1] + dir.val[1] * 2;
	param->tip.val[2] = data->camera->origin.val[2] + dir.val[2] * 2;
//	param->center = assign_3vecf(get_random_number((time(NULL) * 0xcacacaca) << 16) - 0.5, get_random_number((time(NULL) * 0xfeabcdef) << 8) - 0.5, get_random_number((time(NULL) * 0x1056ffe) << 4) - 0.5);
//	normalize_3vecf(&param->center);
	param->center = add_3vecf(assign_3vecf(0, 1, 0), param->tip);
/*	rd = time(NULL);
	while (!is_null(dot_product_3vecf(param->normal, param->x2d_axis)))
	{
		param->x2d_axis = assign_3vecf(get_random_number(rd * 0xcacacaca << 16) - 0.5, get_random_number(rd * 0xfeabcdef << 8) - 0.5, get_random_number(rd * 0x1056ffe << 4) - 0.5);
		rd *= time(NULL);
		normalize_3vecf(&param->x2d_axis);
		printf("asad\n");
	}
*/	cone->obj_param = param;
	cone->obj_type = OBJ_CONE;
	cone->check_inside = &check_inside_cone;
	cone->ray_intersect = &ray_intersect_cone;
	cone->get_normal_inter = &get_normal_intersect_cone;
	cone->get_origin = &get_origin_cone;
	cone->move = &move_cone;
	cone->rotate = &rotate_cone;
	cone->get_text_coordinate = &get_text_coordinate_cone;
	cone->get_text_color = &get_uni_color;
	cone->text = generate_random_texture();
	set_bump_own(cone);
	add_object(cone, data);
	data->new_obj = 1;
}
