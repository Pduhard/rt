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

void	get_uv_axis(t_3vecf axis[3], t_3vecf first_axis)
{
		axis[0] = assign_3vecf(0, 0, 0);
		axis[1] = first_axis;
		normalize_3vecf(&(axis[1]));
		if (axis[1].val[0] != 0)
		{
			axis[0] = assign_3vecf(0, 1, 1);
			axis[0].val[0] = (-axis[1].val[1] - axis[1].val[2])
				/ axis[1].val[0];
		}
		else if (axis[1].val[1] != 0)
		{
			axis[0] = assign_3vecf(1, 0, 1);
			axis[0].val[1] = (-axis[1].val[0] - axis[1].val[2])
				/ axis[1].val[1];
		}
		else if (axis[1].val[2] != 0)
		{
			axis[0] = assign_3vecf(1, 1, 0);
			axis[0].val[2] = (-axis[1].val[0] - axis[1].val[1])
				/ axis[1].val[2];
		}
		normalize_3vecf(&(axis[0]));
		axis[2] = product_3vecf(axis[0], axis[1]);
}

t_2vecf	get_text_coordinate_cone(t_3vecf inter_point,
		t_3vecf normal_inter, t_obj *cone)
{
	t_2vecf	text_coord;
	t_3vecf	cp;
	t_3vecf	cone_axis[3];
	t_cone	*param;

	param = (t_cone *)cone->obj_param;
	get_uv_axis(cone_axis, sub_3vecf(param->tip, param->center));
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

t_3vecf	get_origin_cone(t_obj *cone) // a degager
{
	return (((t_cone *)cone->obj_param)->tip);
}


t_3vecf get_cone_origin(t_obj *cone, t_cone *cone_param, int sp_id)
{
	if (sp_id)
		return (move_3vecf(cone_param->center, cone->motions, sp_id));
	return (cone_param->center);
}

t_3vecf get_cone_tip(t_obj *cone, t_cone *cone_param, int sp_id)
{
	if (sp_id)
		return (move_3vecf(cone_param->tip, cone->motions, sp_id));
	return (cone_param->tip);
}

t_3vecf	get_normal_intersect_cone(t_3vecf inter_point, t_obj *cone, int sp_id)
{
	t_cone	*cone_param;
	t_3vecf	hp;
	t_3vecf	normal;
	t_3vecf	cone_tip;
	t_3vecf	cone_origin;

	cone_param = (t_cone *)cone->obj_param;
	cone_origin = get_cone_origin(cone, cone_param, sp_id);
	cone_tip = get_cone_tip(cone, cone_param, sp_id);
	hp = sub_3vecf(cone_tip, inter_point);
	normal = product_3vecf(hp, product_3vecf(hp,
			sub_3vecf(cone_origin, inter_point)));
	normalize_3vecf(&normal);
	return (normal);
}

t_3vecf get_cone_quadratic_cst(t_cone *cone_param, t_3vecf cone_origin,
	t_3vecf cone_tip, t_leq l)
{
	double	m;
	t_3vecf	h;
	double	dp_dh;
	double	dp_wh;
	t_3vecf	w;

	h = sub_3vecf(cone_origin, cone_tip);
	m = (cone_param->radius * cone_param->radius) /
		(get_length_3vecf(h) * get_length_3vecf(h));
	normalize_3vecf(&h);
	w = sub_3vecf(l.orig, cone_tip);
	dp_dh = dot_product_3vecf(l.dir, h);
	dp_wh = dot_product_3vecf(w, h);
	return ((t_3vecf){{
			dot_product_3vecf(l.dir, l.dir) - m * dp_dh * dp_dh - dp_dh * dp_dh,
			2 * (dot_product_3vecf(l.dir, w) - m * dp_dh * dp_wh - dp_dh * dp_wh),
			dot_product_3vecf(w, w) - m * dp_wh * dp_wh - dp_wh * dp_wh}});
}

int	ray_intersect_cone(t_leq l, t_obj *cone, t_dist dist, int sp_id)
{
	t_3vecf cst;
	int			check;
	t_cone	*cone_param;
	t_2vecf roots;

	check = 0;
	cone_param = (t_cone *)cone->obj_param;
	cst = get_cone_quadratic_cst(
					cone_param,
					get_cone_origin(cone, cone_param, sp_id),
					get_cone_tip(cone, cone_param, sp_id),
					l);
	roots = solve_quadratic(cst.val[0], cst.val[1], cst.val[2]);
	check |= is_closest_intersect(dist, roots.val[0]);
	check |= is_closest_intersect(dist, roots.val[1]);
	return (check);
}

void  assign_cone_function(t_obj *cone)
{
	cone->obj_type = OBJ_CONE;
	cone->check_inside = &check_inside_cone;
	cone->ray_intersect = &ray_intersect_cone;
	cone->get_normal_inter = &get_normal_intersect_cone;
	cone->get_origin = &get_origin_cone;
	cone->move = &move_cone;
	cone->rotate = &rotate_cone;
	cone->get_text_coordinate = &get_text_coordinate_cone;
}

void	generate_new_cone(t_data *data)
{
	t_obj		*cone;
	t_cone		*param;
	t_3vecf		dir;

	dir = mult_3vecf_33matf(mult_3vecf_33matf(window_to_view(0, 0,
		data->size.val[0], data->size.val[1]), data->rot_mat[1]), data->rot_mat[0]);
	normalize_3vecf(&dir);
	if (!(cone = ft_memalloc(sizeof(t_obj))))
		return ;
	if (!(param = ft_memalloc(sizeof(t_cone))))
		return ;
	param->radius = get_random_number(time(NULL)) * 1.5;
	param->tip = add_3vecf(data->camera->origin, product_c3vecf(dir, 2));
	param->center = add_3vecf(assign_3vecf(0, 1, 0), param->tip);
	cone->obj_param = param;
	cone->text = generate_random_texture(cone);
	assign_cone_function(cone);
	set_bump_own(cone);
	add_object(cone, data);
	data->new_obj = 1;
}
