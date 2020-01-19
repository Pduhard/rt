/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   cylinder.c                                       .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: aplat <aplat@student.le-101.fr>            +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2020/01/13 20:10:21 by aplat        #+#   ##    ##    #+#       */
/*   Updated: 2020/01/18 00:52:32 by pduhard-    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "rt.h"
// http://www.illusioncatalyst.com/notes_files/mathematics/line_cylinder_intersection.php

t_3vecf	get_normal_intersect_cylinder(t_3vecf inter_point, t_obj *cylinder)
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
//	t_3vecf	tang;
//	t_3vecf	tmp;

	//return (assign_3vecf(1, 0, 0));
	cylinder_param = (t_cylinder *)cylinder->obj_param;
//	hp = sub_3vecf(inter_point, cylinder_param->tip);
	cp = sub_3vecf(inter_point, cylinder_param->center);
	ch = sub_3vecf(cylinder_param->tip, cylinder_param->center);
	double inter_proj_dist = dot_product_3vecf(ch, cp) / get_length_3vecf(ch);
	length_ch = get_length_3vecf(ch);
	step_inter_proj = inter_proj_dist / length_ch;
	inter_proj = assign_3vecf(	cylinder_param->center.val[0] + ch.val[0] * step_inter_proj,
								cylinder_param->center.val[1] + ch.val[1] * step_inter_proj,
								cylinder_param->center.val[2] + ch.val[2] * step_inter_proj);
	//h = sub_3vecf(cylinder_param->center, cylinder_param->tip);
	
//	tang = product_3vecf(hp, cp);
	//t_3vecf tmp = sub_3vecf(inter_proj, inter_point);
	//printf("ch %f %f %f cp %f %f %f step %f proj %f %f %f => point %f %f %f\n",ch.val[0], ch.val[1], ch.val[2],cp.val[0], cp.val[1], cp.val[2], step_inter_proj, inter_proj.val[0], inter_proj.val[1], inter_proj.val[2], inter_point.val[0], inter_point.val[1], inter_point.val[2]);
	//usleep(10);
	return (sub_3vecf(inter_proj, inter_point));
//	intersect = dot_product_3vecf(sub_3vecf(inter_point, cylinder_param->center), ch);
//	if (intersect < 0)
//		return (assign_3vecf(-tmp.val[0], -tmp.val[1], -tmp.val[2]));
//	else
//		return (tmp);
	//(void)inter_point;
}

int ray_intersect_cylinder(t_3vecf orig, t_3vecf dir, t_obj *cylinder, double *dist, double min_dist, double max_dist)
{
	t_3vecf	h;
	t_3vecf	norm_h;
//	double	h_length;
	double	a;
	double	b;
	double	c;
	int		check = 0;
	t_cylinder	*cylinder_param;

	cylinder_param = (t_cylinder *)cylinder->obj_param;
	h = sub_3vecf(cylinder_param->center, cylinder_param->tip);
	norm_h = h;
	normalize_3vecf(&norm_h);
//	h_length = get_lenght_3vecf(h);

	double	dp_dir_norm_h;
	double	dp_w_norm_h;
	t_3vecf	w;

	w = sub_3vecf(orig, cylinder_param->center);
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

int			parse_cylinder(char *line, t_data *data)
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
	if (line[i] != '(' || (i = parse_3vecf(line, i, &cylinder->color)) == -1)
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
	if (data->objs)
		cylinder->next = data->objs;
	else
		cylinder->next = NULL;
	data->objs = cylinder;
	return (1);
}
