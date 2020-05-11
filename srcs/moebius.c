/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   moebius.c                                        .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: pduhard- <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2020/01/31 18:29:04 by pduhard-     #+#   ##    ##    #+#       */
/*   Updated: 2020/03/04 08:06:32 by pduhard-         ###   ########lyon.fr   */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "rt.h"

int		check_inside_moebius(t_3vecf point, t_obj *moebius)
{
	t_moebius	*param;
	double	v;
	double	u;
	double	sin_v_2;

	param = (t_moebius *)moebius->obj_param;
	v = atan2(point.val[1], point.val[0]);
	if (!is_null((sin_v_2 = sin(v / 2))))
		u = point.val[2] / sin_v_2;
	else if (!is_null(v))
		u = (point.val[0] / cos(v) - param->radius) / cos(v / 2);
	else
		u = (point.val[0] - param->radius);
	point.val[0] -= (param->radius + u * cos(v / 2)) * cos(v);
	point.val[1] -= (param->radius + u * cos(v / 2)) * sin(v);
	point.val[2] -= u * sin_v_2;

	if (is_null(point.val[0] * point.val[0] + point.val[1]
			* point.val[1] + point.val[2] * point.val[2])
			&& u < param->half_width && u > -param->half_width)
		return (1);
	return (0);
}

t_2vecf	get_text_coordinate_moebius(t_3vecf inter_point, t_3vecf normal_inter, t_obj *moebius)
{
	t_2vecf	text_coord;
	double	sin_v_2;
	t_moebius	*param;

	param = (t_moebius *)moebius->obj_param;
	inter_point = sub_3vecf(inter_point, param->origin);
	text_coord.val[1] = atan2(inter_point.val[1], inter_point.val[0]);
	if (!is_null((sin_v_2 = sin(text_coord.val[1] / 2))))
		text_coord.val[0] = inter_point.val[2] / sin_v_2;
	else if (!is_null(text_coord.val[1]))
		text_coord.val[0] = (inter_point.val[0] / cos(text_coord.val[1]) - param->radius) / cos(text_coord.val[1] / 2);
	else
		text_coord.val[0] = (inter_point.val[0] - param->radius);
	text_coord.val[0] = (text_coord.val[0] + param->half_width) / (2 * param->half_width);
	text_coord.val[1] = (text_coord.val[1] + M_PI) / (M_PI * 2);
	return (text_coord);
	(void)normal_inter;
}

void	move_moebius(t_obj *moebius, t_3vecf dir, double fact)
{
	t_moebius	*param;
	t_cut		*cuts;

	param = (t_moebius *)moebius->obj_param;
	param->origin.val[0] += dir.val[0] * fact;
	param->origin.val[1] += dir.val[1] * fact;
	param->origin.val[2] += dir.val[2] * fact;
	cuts = moebius->cuts;
	while (cuts)
	{
		if (cuts->move && cuts->cut_type != CUT_STATIC)
			cuts->move(cuts, dir, fact);
		cuts = cuts->next;
	}
}

t_3vecf	get_origin_moebius(t_obj *moebius) //
{
	return (((t_moebius *)moebius->obj_param)->origin);
}

t_3vecf get_moebius_origin(t_obj *moebius, t_moebius *param, int sp_id)
{
		if (sp_id)
			return (move_3vecf(param->origin, moebius->motions, sp_id));
		return (param->origin);
}

t_3vecf	get_normal_intersect_moebius(t_3vecf inter_point, t_obj *moebius, int sp_id)
{
	t_moebius	*param;
	t_3vecf		normal_inter;
	double		c[3];
	t_3vecf		moebius_origin;

	param = (t_moebius *)moebius->obj_param;
	moebius_origin = get_moebius_origin(moebius, param, sp_id);
	c[0] = inter_point.val[0] - moebius_origin.val[0];
	c[1] = inter_point.val[1] - moebius_origin.val[1];
	c[2] = inter_point.val[2] - moebius_origin.val[2];
	normal_inter.val[0] = 2 * c[0] * c[1]
											- 2 * param->radius * c[2]
											- 4 * c[0] * c[2];
	normal_inter.val[1] = -param->radius * param->radius
											+ c[0] * c[0] + 3 * c[1] * c[1]
											- 4 * c[1] * c[2] + c[2] * c[2];
	normal_inter.val[2] = -2 * param->radius * c[0] - 2 * c[0] * c[0]
											- 2 * c[1] * c[1] + 2 * c[1] * c[2];
	normalize_3vecf(&normal_inter);
	return (normal_inter);
}

t_4vecf get_moebius_cubis_cst2(double o[3], double d[3], double r)
{
	t_4vecf		cst;

	cst.val[3] = (-r * r * o[1]) + (o[0] * o[0] * o[1])
						 + (o[1] * o[1] * o[1]) - (2 * r * o[0] * o[2])
						 - (2 * o[0] * o[0] * o[2]) - (2 * o[1] * o[1] * o[2])
						 + (o[1] * o[2] * o[2]);//d
	cst.val[2] = (-r * r * d[1])
						 + (o[0] * o[0] * d[1]) + (2 * o[0] * o[1] * d[0])
						 + (o[1] * o[1] * d[1]) + (2 * o[1] * o[1] * d[1])
						 - (2 * r * o[0] * d[2]) - (2 * r * o[2] * d[0])
						 - (2 * o[0] * o[0] * d[2]) - (4 * o[0] * o[2] * d[0])
						 - (2 * o[1] * o[1] * d[2]) - (4 * o[1] * o[2] * d[1])
						 + (2 * o[1] * o[2] * d[2]) + (o[2] * o[2] * d[1]);//c
	cst.val[1] = (2 * o[0] * d[0] * d[1]) + (o[1] * d[0] * d[0])
						 + (2 * o[1] * d[1] * d[1]) + (o[1] * d[1] * d[1])
						 - (2 * r * d[0] * d[2]) - (4 * o[0] * d[0] * d[2])
						 - (2 * o[2] * d[0] * d[0]) - (4 * o[1] * d[1] * d[2])
						 - (2 * o[2] * d[1] * d[1]) + (o[1] * d[2] * d[2])
						 + (2 * o[2] * d[1] * d[2]);//b
	cst.val[0] = (d[0] * d[0] * d[1]) + (d[1] * d[1] * d[1])
						 - (2 * d[0] * d[0] * d[2]) - (2 * d[1] * d[1] * d[2])
						 + (d[1] * d[2] * d[2]);//a
	return (cst);
}

t_4vecf	get_moebius_cubis_cst(t_moebius *param, t_3vecf moebius_origin, t_leq l)
{
	double		o[3];
	double		d[3];

	o[0] = l.orig.val[0] - moebius_origin.val[0];
	o[1] = l.orig.val[1] - moebius_origin.val[1];
	o[2] = l.orig.val[2] - moebius_origin.val[2];
	d[0] = l.dir.val[0];
	d[1] = l.dir.val[1];
	d[2] = l.dir.val[2];
	return (get_moebius_cubis_cst2(o, d, param->radius));
}

int check_moebius_bnd(t_leq l, t_3vecf moebius_origin, double root, t_obj *moebius)
{
	t_3vecf	coord;

	coord.val[0] = l.orig.val[0] + l.dir.val[0] * root - moebius_origin.val[0];
	coord.val[1] = l.orig.val[1] + l.dir.val[1] * root - moebius_origin.val[1];
	coord.val[2] = l.orig.val[2] + l.dir.val[2] * root - moebius_origin.val[2];
	if ((moebius->check_inside(coord, moebius)))
		return (1);
	return (0);
}

int	ray_intersect_moebius(t_leq l, t_obj *moebius, t_dist dist, int sp_id)
{
	t_4vecf		cst;
	t_3vecf		roots;
	int				check;
	t_3vecf		moebius_origin;
	int				i;

	i = -1;
	check = 0;
	moebius_origin = get_moebius_origin(moebius,
		(t_moebius *)moebius->obj_param, sp_id);
	cst = get_moebius_cubis_cst((t_moebius *)moebius->obj_param,
		moebius_origin, l);
	roots = solve_cubic(cst.val[0], cst.val[1], cst.val[2], cst.val[3]);
	while (++i < 3)
		if (check_moebius_bnd(l, moebius_origin, roots.val[i], moebius))
			check |= is_closest_intersect(dist, roots.val[i]);
	return (check);
}

void  assign_moebius_function(t_obj *moebius)
{
	moebius->obj_type = OBJ_MOEBIUS;
	moebius->check_inside = &check_inside_moebius;
	moebius->ray_intersect = &ray_intersect_moebius;
	moebius->get_normal_inter = &get_normal_intersect_moebius;
	moebius->get_origin = &get_origin_moebius;
	moebius->move = &move_moebius;
	moebius->rotate = NULL;
	moebius->get_text_coordinate = &get_text_coordinate_moebius;
}

void	generate_new_moebius(t_data *data)
{
	t_obj		*moebius;
	t_moebius	*param;
	t_3vecf		dir;

	dir = mult_3vecf_33matf(mult_3vecf_33matf(window_to_view(0, 0,
		data->size.val[0], data->size.val[1]), data->rot_mat[1]), data->rot_mat[0]);
	normalize_3vecf(&dir);
	if (!(moebius = ft_memalloc(sizeof(t_obj))))
		return ;
	if (!(param = ft_memalloc(sizeof(t_moebius))))
		return ;
	param->radius = get_random_number((time(NULL) * 0xcacacaca) << 16) * 3;
	param->origin = add_3vecf(data->camera->origin,
		product_c3vecf(dir, param->radius + 2));
	param->half_width = get_random_number((time(NULL) * 0xabcdef99) << 4)
		* param->radius + 0.1;
	moebius->obj_param = param;
	assign_moebius_function(moebius);
	moebius->text = generate_random_texture(moebius);
	set_bump_own(moebius);
	add_object(moebius, data);
	data->new_obj = 1;
}
