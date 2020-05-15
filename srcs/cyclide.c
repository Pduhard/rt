/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cyclide.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aplat <aplat@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/15 17:40:20 by aplat             #+#    #+#             */
/*   Updated: 2020/05/15 17:44:39 by aplat            ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

int		check_inside_cyclide(t_3vecf point, t_obj *cyclide)
{
	return (0);
	(void)point;
	(void)cyclide;
}

t_2vecf	get_text_coordinate_cyclide(t_3vecf inter_point, t_3vecf normal_inter,
	t_obj *cyclide)
{
	return ((t_2vecf){{0, 0}});
	(void)normal_inter;
	(void)inter_point;
	(void)cyclide;
}

void	move_cyclide(t_obj *cyclide, t_3vecf dir, double fact)
{
	t_cyclide	*param;
	t_cut		*cuts;

	param = (t_cyclide *)cyclide->obj_param;
	param->origin.val[0] += dir.val[0] * fact;
	param->origin.val[1] += dir.val[1] * fact;
	param->origin.val[2] += dir.val[2] * fact;
	cuts = cyclide->cuts;
	while (cuts)
	{
		if (cuts->move && cuts->cut_type != CUT_STATIC)
			cuts->move(cuts, dir, fact);
		cuts = cuts->next;
	}
}

t_3vecf	get_origin_cyclide(t_obj *cyclide) // a degager
{
	return (((t_cyclide *)cyclide->obj_param)->origin);
}

t_3vecf	get_cyclide_origin(t_obj *cyclide, t_cyclide *param, int sp_id)
{
	if (sp_id)
		return (move_3vecf(param->origin, cyclide->motions, sp_id));
	return (param->origin);
}

t_3vecf	get_normal_intersect_cyclide(t_3vecf inter_point,
	t_obj *cyclide, int sp_id)
{
	t_cyclide	*param;
	t_3vecf		normal_inter;
	t_3vecf		cyclide_origin;
	double		c[3];

	param = (t_cyclide *)cyclide->obj_param;
	cyclide_origin = get_cyclide_origin(cyclide, param, sp_id);
	c[0] = inter_point.val[0] - cyclide_origin.val[0];
	c[1] = inter_point.val[1] - cyclide_origin.val[1];
	c[2] = inter_point.val[2] - cyclide_origin.val[2];
	normal_inter.val[0] = 2 * c[0] * c[2] + 2 * param->param * c[0];
	normal_inter.val[1] = 2 * c[1] * c[2] - 2 * param->param * c[1];
	normal_inter.val[2] = c[0] * c[0] + c[1] * c[1] + 3 * c[2] * c[2]
		- param->param * param->param;
	normalize_3vecf(&normal_inter);
	return (normal_inter);
}

t_4vecf	get_cyclide_cubic_cst(t_cyclide *param, t_leq l,
	t_3vecf cyclide_origin)
{
	t_4vecf cst;
	double	o[3];
	double	d[3];
	double	a;

	a = param->param;
	o[0] = l.orig.val[0] - cyclide_origin.val[0];
	o[1] = l.orig.val[1] - cyclide_origin.val[1];
	o[2] = l.orig.val[2] - cyclide_origin.val[2];
	d[0] = l.dir.val[0];
	d[1] = l.dir.val[1];
	d[2] = l.dir.val[2];
	cst.val[0] = (d[0] * d[0] * d[2]) + (d[1] * d[1] * d[2])
		+ (d[2] * d[2] * d[2]);
	cst.val[1] = (o[2] * d[0] * d[0]) + (2 * o[0] * d[0] * d[2])
		+ (o[2] * d[1] * d[1]) + (2 * o[1] * d[2] * d[1])
			+ (3 * o[2] * d[2] * d[2]) + (a * d[0] * d[0]) - (a * d[1] * d[1]);
	cst.val[2] = (2 * o[0] * o[2] * d[0]) + (o[0] * o[0] * d[2])
		+ (2 * o[1] * o[2] * d[1]) + (o[1] * o[1] * d[2])
			+ (3 * o[2] * o[2] * d[2]) - (a * a * d[2]) + (2 * a * o[0] * d[0])
				- (2 * a * o[1] * d[1]);
	cst.val[3] = (o[0] * o[0] * o[2]) + (o[1] * o[1] * o[2])
		+ (o[2] * o[2] * o[2]) - (a * a * o[2]) + (a * o[0] * o[0])
			- (a * o[1] * o[1]);
	return (cst);
}

int		check_cyclide_bnd(t_leq l, t_3vecf cyclide_origin, double root)
{
	double	x;
	double	y;
	double	z;

	x = l.orig.val[0] - cyclide_origin.val[0] + l.dir.val[0] * root;
	y = l.orig.val[1] - cyclide_origin.val[1] + l.dir.val[1] * root;
	z = l.orig.val[2] - cyclide_origin.val[2] + l.dir.val[2] * root;
	if (x > -1.5 && x < 1.5 && y > -2 && y < 2 && z > -1 && z < 1)
		return (1);
	return (0);
}

int		ray_intersect_cyclide(t_leq l, t_obj *cyclide, t_dist dist, int sp_id)
{
	t_4vecf		cst;
	t_3vecf		roots;
	int			check;
	t_3vecf		cyclide_origin;
	int			i;

	i = -1;
	check = 0;
	cyclide_origin = get_cyclide_origin(
		cyclide,
		(t_cyclide *)cyclide->obj_param,
		sp_id);
	cst = get_cyclide_cubic_cst((t_cyclide *)cyclide->obj_param,
		l, cyclide_origin);
	roots = solve_cubic(cst.val[0], cst.val[1], cst.val[2], cst.val[3]);
	while (++i < 3)
		if (check_cyclide_bnd(l, cyclide_origin, roots.val[i]))
			check |= is_closest_intersect(dist, roots.val[i]);
	return (check);
}

void	assign_cyclide_function(t_obj *cyclide)
{
	cyclide->obj_type = OBJ_CYCLIDE;
	cyclide->check_inside = &check_inside_cyclide;
	cyclide->ray_intersect = &ray_intersect_cyclide;
	cyclide->get_normal_inter = &get_normal_intersect_cyclide;
	cyclide->get_origin = &get_origin_cyclide;
	cyclide->move = &move_cyclide;
	cyclide->rotate = NULL;
	cyclide->get_text_coordinate = &get_text_coordinate_cyclide;
}

void	generate_new_cyclide(t_data *data)
{
	t_obj		*cyclide;
	t_cyclide	*param;
	t_3vecf		dir;

	dir = mult_3vecf_33matf(mult_3vecf_33matf(window_to_view(0, 0,
		data->size.val[0], data->size.val[1]), data->rot_mat[1]),
			data->rot_mat[0]);
	normalize_3vecf(&dir);
	if (!(cyclide = ft_memalloc(sizeof(t_obj))))
		return ;
	if (!(param = ft_memalloc(sizeof(t_cyclide))))
		return ;
	param->origin = add_3vecf(data->camera->origin, product_c3vecf(dir, 2));
	param->param = (get_random_number((time(NULL) * 0xcacacaca) << 3) - 0.5)
		* 2;
	cyclide->obj_param = param;
	assign_cyclide_function(cyclide);
	cyclide->text = generate_random_texture(cyclide);
	set_bump_own(cyclide);
	add_object(cyclide, data);
	data->new_obj = 1;
}
