/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   cylinder.c                                       .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: aplat <aplat@student.le-101.fr>            +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2020/01/13 20:10:21 by aplat        #+#   ##    ##    #+#       */
/*   Updated: 2020/02/28 04:48:33 by pduhard-         ###   ########lyon.fr   */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "rt.h"

// http://www.illusioncatalyst.com/notes_files/mathematics/line_cylinder_intersection.php

int		check_inside_cylinder(t_3vecf inter_point, t_obj *cylinder)
{
	t_cylinder	*param;

	param = (t_cylinder *)cylinder->obj_param;
	if (get_length_3vecf(product_3vecf(sub_3vecf(inter_point, param->tip), sub_3vecf(inter_point, param->center))) / get_length_3vecf(sub_3vecf(param->tip, param->center)) > param->radius)
		return (0);
	return (1);
}

t_2vecf	get_text_coordinate_cylinder(t_3vecf inter_point, t_3vecf normal_inter, t_obj *cylinder)
{
	t_2vecf	text_coord;
	t_3vecf	cp;
	t_3vecf	cyl_axis[3];
	t_cylinder	*param;

	param = (t_cylinder *)cylinder->obj_param;
	cp = sub_3vecf(inter_point, param->center);
	cyl_axis[1] = sub_3vecf(param->tip, param->center);
	normalize_3vecf(&(cyl_axis[1]));
	if (cyl_axis[1].val[0] != 0)
	{
		cyl_axis[0] = assign_3vecf(0, 1, 1);
		cyl_axis[0].val[0] = (-cyl_axis[1].val[1] - cyl_axis[1].val[2]) / cyl_axis[1].val[0];
	}
	else if (cyl_axis[1].val[1] != 0)
	{
		cyl_axis[0] = assign_3vecf(1, 0, 1);
		cyl_axis[0].val[1] = (-cyl_axis[1].val[0] - cyl_axis[1].val[2]) / cyl_axis[1].val[1];
	}
	else if (cyl_axis[1].val[2] != 0)
	{
		cyl_axis[0] = assign_3vecf(1, 1, 0);
		cyl_axis[0].val[2] = (-cyl_axis[1].val[0] - cyl_axis[1].val[1]) / cyl_axis[1].val[2];
	}
	normalize_3vecf(&(cyl_axis[0]));
	cyl_axis[2] = product_3vecf(cyl_axis[0], cyl_axis[1]);
	text_coord.val[0] = (1 - dot_product_3vecf(cyl_axis[1], cp)) / ((2 * M_PI) * param->radius);
	//	if (dot_product_3vecf(cyl_axis[0], cp) < 0 && dot_product_3vecf(cyl_axis[2], cp) < 0)
	text_coord.val[1] = (atan2(dot_product_3vecf(cyl_axis[0], cp), dot_product_3vecf(cyl_axis[2], cp))) / (2 * M_PI);
	//	else
	//		text_coord.val[1] = 1 - (1 - atan2(dot_product_3vecf(cyl_axis[0], cp), dot_product_3vecf(cyl_axis[2], cp))) / (2 * M_PI);
	//	printf("%f %f\n", text_coord.val[0], text_coord.val[1]);
	//	inter_point.val[2], inter_point.val[0]);
	//text_coord.val[1] = inter_point.val[1] * M_PI;
	return (text_coord);
	(void)normal_inter;
}

void	move_cylinder(t_obj *cylinder, t_3vecf dir, double fact)
{
	t_cylinder	*param;
	t_obj	*cuts;

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
		cuts->move(cuts, dir, fact);
		cuts = cuts->next;
	}
}

void   rotate_cylinder(t_obj *cylinder, t_3vecf orig, t_33matf rot_mat[2])
{
	t_cylinder *param;
	t_obj   *cuts;

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
		cuts->rotate(cuts, orig, rot_mat);
		cuts = cuts->next;
	}
}

t_3vecf	get_origin_cylinder(t_obj *cylinder)
{
	return (((t_cylinder *)cylinder->obj_param)->center);
}

t_3vecf	get_normal_intersect_cylinder(t_3vecf inter_point, t_obj *cylinder, int sp_id)
{
	//	double	intersect;
	//t_3vecf	h;

	t_cylinder	*cylinder_param;
	//	t_3vecf	hp;
	t_3vecf	ch;
	t_3vecf	cp;
	t_3vecf	inter_proj;
	double	length_ch;
	double	step_inter_proj;
	t_3vecf	cylinder_origin;
	t_3vecf	cylinder_tip;


	//return (assign_3vecf(1, 0, 0));
	cylinder_param = (t_cylinder *)cylinder->obj_param;
	cylinder_origin = sp_id ? move_3vecf(cylinder_param->center, cylinder->motions, sp_id) : cylinder_param->center;
	cylinder_tip = sp_id ? move_3vecf(cylinder_param->tip, cylinder->motions, sp_id) : cylinder_param->tip;
	//	hp = sub_3vecf(inter_point, cylinder_param->tip);
	cp = sub_3vecf(inter_point, cylinder_origin);
	ch = sub_3vecf(cylinder_tip, cylinder_origin);
	length_ch = get_length_3vecf(ch);
	step_inter_proj = dot_product_3vecf(ch, cp) / (length_ch * length_ch);
	inter_proj = assign_3vecf(	cylinder_origin.val[0] + ch.val[0] * step_inter_proj,
			cylinder_origin.val[1] + ch.val[1] * step_inter_proj,
			cylinder_origin.val[2] + ch.val[2] * step_inter_proj);

	return (sub_3vecf(inter_proj, inter_point));
}

int ray_intersect_cylinder(t_3vecf orig, t_3vecf dir, t_obj *cylinder, double *dist, double min_dist, double max_dist, int sp_id)
{
	t_3vecf	h;
	t_3vecf	norm_h;
	//	double	h_length;
	double	a;
	double	b;
	double	c;
	int		check = 0;
	t_cylinder	*cylinder_param;
	t_3vecf	cylinder_origin;
	t_3vecf	cylinder_tip;

	cylinder_param = (t_cylinder *)cylinder->obj_param;
	cylinder_origin = sp_id ? move_3vecf(cylinder_param->center, cylinder->motions, sp_id) : cylinder_param->center;
	cylinder_tip = sp_id ? move_3vecf(cylinder_param->tip, cylinder->motions, sp_id) : cylinder_param->tip;
	h = sub_3vecf(cylinder_origin, cylinder_tip);
	norm_h = h;
	normalize_3vecf(&norm_h);
	//	h_length = get_lenght_3vecf(h);

	(void)sp_id;
	double	dp_dir_norm_h;
	double	dp_w_norm_h;
	t_3vecf	w;

	w = sub_3vecf(orig, cylinder_origin);
	dp_dir_norm_h = dot_product_3vecf(dir, norm_h);
	dp_w_norm_h = dot_product_3vecf(w, norm_h);

	a = dot_product_3vecf(dir, dir) - dp_dir_norm_h * dp_dir_norm_h;
	b = 2 * (dot_product_3vecf(dir, w) - dp_dir_norm_h * dp_w_norm_h);
	c = dot_product_3vecf(w, w) - dp_w_norm_h * dp_w_norm_h - cylinder_param->radius * cylinder_param->radius;

	double	delta;
	delta = b * b - 4 * a * c;
	t_2vecf	hit_point;

	if (delta > 0)
	{
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

/*int			parse_cylinder(char *line, t_data *data)
  {
  int			i;
  t_obj		*cylinder;
  t_cylinder	*cylinder_param;

  if (!(cylinder = malloc(sizeof(t_obj))) || !(cylinder_param = malloc(sizeof(t_cylinder))))
  return (0);
  i = 8;
  while (ft_isspace(line[i]))
  i++;
  if (line[i] != '(' || (i = parse_3vecf(line, i, &cylinder_param->center)) == -1)
  {
  ft_printf("Syntax error: cylinder syntax: cylinder(center)(tip)(radius)(reflection)\n");
  return (0);
  }
  while (ft_isspace(line[i]))
  ++i;
  if (line[i] != '(' || (i = parse_3vecf(line, i, &cylinder_param->tip)) == -1)
  {
  ft_printf("Syntax error: cylinder syntax: cylinder(center)(tip)(radius)(reflection)\n");
  return (0);
  }
  while (ft_isspace(line[i]))
  ++i;
  if (line[i] != '(' || (i = parse_double(line, i, &cylinder_param->radius)) == -1)
  {
  ft_printf("Syntax error: cylinder syntax: cylinder(center)(tip)(radius)(reflection)\n");
  return (0);
  }
  while (ft_isspace(line[i]))
  ++i;
  if (line[i] != '(' || (i = parse_texture(line, i, cylinder)) == -1)
  {
  ft_printf("Syntax error: cylinder syntax: cylinder(center)(tip)(radius)(reflection)\n");
  return (0);
  }
  while (ft_isspace(line[i]))
  ++i;
  if (line[i] != '(' || (i = parse_double(line, i, &cylinder->reflection)) == -1)
  {
  ft_printf("Syntax error: cylinder syntax: cylinder(center)(tip)(radius)(reflection)\n");
  return (0);
  }
  while (ft_isspace(line[i]))
  ++i;
  if (line[i] != '(' || (i = parse_double(line, i, &cylinder->refraction)) == -1)
  {
  ft_printf("Syntax error: cylinder syntax: cylinder(center)(tip)(radius)(reflection)\n");
  return (0);
  }

  cylinder->obj_param = cylinder_param;
  cylinder->obj_type = OBJ_CYLINDER;
  cylinder->ray_intersect = &ray_intersect_cylinder;
  cylinder->get_normal_inter = &get_normal_intersect_cylinder;
  cylinder->get_text_coordinate = &get_text_coordinate_cylinder;
  if (data->objs)
  cylinder->next = data->objs;
  else
  cylinder->next = NULL;
  data->objs = cylinder;
  return (1);
  }
  */
