/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   cone.c                                           .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: aplat <aplat@student.le-101.fr>            +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2019/12/30 18:21:18 by pduhard-     #+#   ##    ##    #+#       */
/*   Updated: 2020/02/28 23:41:36 by pduhard-         ###   ########lyon.fr   */
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
	t_3vecf	cone_axis;
	t_3vecf	p_tip_vec;
	t_3vecf	orth_dist;
	double	radius_inter;
	int		check_rev;

	check_rev = 0;
	param = (t_cone *)obj->obj_param;
	cone_axis = sub_3vecf(param->center, param->tip);
	height = get_length_3vecf(cone_axis);
	normalize_3vecf(&cone_axis);
	p_tip_vec = sub_3vecf(inter_point, param->tip);
	c_dist = dot_product_3vecf(p_tip_vec, cone_axis);
	if (c_dist < 0 && (check_rev = 1))
		c_dist *= -1;
	radius_inter = (c_dist / height) * param->radius;
	//	if (c_dist < 0 || c_dist > height)
	//		return (0);
	orth_dist.val[0] = p_tip_vec.val[0] - c_dist * (check_rev ? -cone_axis.val[0] : cone_axis.val[0]);
	orth_dist.val[1] = p_tip_vec.val[1] - c_dist * (check_rev ? -cone_axis.val[1] : cone_axis.val[1]);
	orth_dist.val[2] = p_tip_vec.val[2] - c_dist * (check_rev ? -cone_axis.val[2] : cone_axis.val[2]);
	if (get_length_3vecf(orth_dist) > radius_inter)
		return (0);
	return (1);
}

t_2vecf	get_text_coordinate_cone(t_3vecf inter_point, t_3vecf normal_inter, t_obj *cone)
{
	t_2vecf	text_coord;
	t_3vecf	cp;
	t_3vecf	cone_axis[3];
	t_cone	*param;

	param = (t_cone *)cone->obj_param;
	cp = sub_3vecf(inter_point, param->center);
	cone_axis[1] = sub_3vecf(param->tip, param->center);
	normalize_3vecf(&(cone_axis[1]));
	if (cone_axis[1].val[0] != 0)
	{
		cone_axis[0] = assign_3vecf(0, 1, 1);
		cone_axis[0].val[0] = (-cone_axis[1].val[1] - cone_axis[1].val[2]) / cone_axis[1].val[0];
	}
	else if (cone_axis[1].val[1] != 0)
	{
		cone_axis[0] = assign_3vecf(1, 0, 1);
		cone_axis[0].val[1] = (-cone_axis[1].val[0] - cone_axis[1].val[2]) / cone_axis[1].val[1];
	}
	else if (cone_axis[1].val[2] != 0)
	{
		cone_axis[0] = assign_3vecf(1, 1, 0);
		cone_axis[0].val[2] = (-cone_axis[1].val[0] - cone_axis[1].val[1]) / cone_axis[1].val[2];
	}
	/*	t_3vecf	ch = sub_3vecf(param->tip, param->center);
		double	length_ch = get_length_3vecf(ch);
		double	step_inter_proj = dot_product_3vecf(ch, cp) / length_ch / length_ch;
		t_3vecf	inter_proj = assign_3vecf(	param->center.val[0] + ch.val[0] * step_inter_proj,
		param->center.val[1] + ch.val[1] * step_inter_proj,
		param->center.val[2] + ch.val[2] * step_inter_proj);
		double	scale = get_length_3vecf(sub_3vecf(inter_point, inter_proj));
		(void)scale;
		t_3vecf tp = sub_3vecf(inter_point, param->tip);
		double	a = get_length_3vecf(tp);
		double	r = sqrtf(a * a - step_inter_proj * step_inter_proj);*/
	normalize_3vecf(&(cone_axis[0]));
	cone_axis[2] = product_3vecf(cone_axis[0], cone_axis[1]);
	text_coord.val[0] = -dot_product_3vecf(cone_axis[1], cp) / (M_PI * 2);
	text_coord.val[1] = atan2(dot_product_3vecf(cone_axis[0], cp), dot_product_3vecf(cone_axis[2], cp)) / (M_PI * 2);
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
	cone_origin = sp_id ? move_3vecf(cone_param->center, cone->motions, sp_id) : cone_param->center;
	cone_tip = sp_id ? move_3vecf(cone_param->tip, cone->motions, sp_id) : cone_param->tip;
	h = sub_3vecf(cone_origin, cone_tip);
	intersect = dot_product_3vecf(sub_3vecf(inter_point, cone_tip), h);
	hp = sub_3vecf(cone_tip, inter_point);
	cp = sub_3vecf(cone_origin, inter_point);
	tang = product_3vecf(hp, cp);
	normal = product_3vecf(hp, tang);
	if (intersect < 0)
		return (assign_3vecf(-normal.val[0], -normal.val[1], -normal.val[2]));
	else
		return (normal);
}

int	ray_intersect_cone(t_3vecf orig, t_3vecf dir, t_obj *cone, double *dist, double min_dist, double max_dist, int sp_id)
{
	double	m;
	t_3vecf	h;
	t_3vecf	norm_h;
	double	h_length;
	double	a;
	double	b;
	double	c;
	int		check = 0;
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

	w = sub_3vecf(orig, cone_tip);
	dp_dir_norm_h = dot_product_3vecf(dir, norm_h);
	dp_w_norm_h = dot_product_3vecf(w, norm_h);

	a = dot_product_3vecf(dir, dir) - m * dp_dir_norm_h * dp_dir_norm_h - dp_dir_norm_h * dp_dir_norm_h;

	b = 2 * (dot_product_3vecf(dir, w) - m * dp_dir_norm_h * dp_w_norm_h - dp_dir_norm_h * dp_w_norm_h);
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
		if (hit_point.val[0] < *dist && hit_point.val[0] > min_dist && hit_point.val[0] < max_dist)
		{
			check = 1;
			*dist = hit_point.val[0];
		}
		if (hit_point.val[1] < *dist && hit_point.val[1] > min_dist && hit_point.val[1] < max_dist)
		{
			check = 1;
			*dist = hit_point.val[1];
		}
		return (check);
	}
	return (0);
}

/*int		parse_cone(char *line, t_data *data)
  {
  int			i;
  t_obj		*cone;
  t_cone		*cone_param;

  if (!(cone = malloc(sizeof(t_obj))) || !(cone_param = malloc(sizeof(t_cone))))
  return (0);
  i = 4;
  while (ft_isspace(line[i]))
  ++i;
  if (line[i] != '(' || (i = parse_3vecf(line, i, &cone_param->center)) == -1)
  {
  ft_printf("Syntax error: cone syntax: cone(center)(tip)(radius)(reflection)\n");
  return (0);
  }
  while (ft_isspace(line[i]))
  ++i;
  if (line[i] != '(' || (i = parse_3vecf(line, i, &cone_param->tip)) == -1)
  {
  ft_printf("Syntax error: cone syntax: cone(center)(tip)(radius)(reflection)\n");
  return (0);
  }
  while (ft_isspace(line[i]))
  ++i;
  if (line[i] != '(' || (i = parse_double(line, i, &cone_param->radius)) == -1)
  {
  ft_printf("Syntax error: cone syntax: cone(center)(tip)(radius)(reflection)\n");
  return (0);
  }
  while (ft_isspace(line[i]))
  ++i;
  if (line[i] != '(' || (i = parse_texture(line, i, cone)) == -1)
  {
  ft_printf("Syntax error: cone syntax: cone(center)(tip)(radius)(reflection)\n");
  return (0);
  }
  while (ft_isspace(line[i]))
  ++i;
  if (line[i] != '(' || (i = parse_double(line, i, &cone->reflection)) == -1)
  {
  ft_printf("Syntax error: cone syntax: cone(center)(tip)(radius)(reflection)\n");
  return (0);
  }
  while (ft_isspace(line[i]))
  ++i;
  if (line[i] != '(' || (i = parse_double(line, i, &cone->refraction)) == -1)
  {
  ft_printf("Syntax error: cone syntax: cone(center)(tip)(radius)(reflection)\n");
  return (0);
  }

//printf("cone : %f %f %f && %f %f %f && %f %f %f\n", cone_param->origin.val[0], cone_param->origin.val[1], cone_param->origin.val[2], cone_param->normal.val[0], cone_param->normal.val[1], cone_param->normal.val[2] , cone->color.val[0], cone->color.val[1], cone->color.val[2]);
cone->obj_param = cone_param;
cone->obj_type = OBJ_CONE;
cone->ray_intersect = &ray_intersect_cone;
cone->get_normal_inter = &get_normal_intersect_cone;
cone->get_text_coordinate = &get_text_coordinate_cone;
if (data->objs)
{
cone->next = data->objs;
}
else
cone->next = NULL;
data->objs = cone;
return (1);
}*/
