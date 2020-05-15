/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fermat.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aplat <aplat@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/15 18:12:08 by aplat             #+#    #+#             */
/*   Updated: 2020/05/15 18:19:46 by aplat            ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

int		check_inside_fermat(t_3vecf point, t_obj *fermat)
{
	return (0);
	(void)point;
	(void)fermat;
}

t_2vecf	get_text_coordinate_fermat(t_3vecf inter_point, t_3vecf normal_inter,
	t_obj *fermat)
{
	return ((t_2vecf){{0, 0}});
	(void)normal_inter;
	(void)inter_point;
	(void)fermat;
}

void	move_fermat(t_obj *fermat, t_3vecf dir, double fact)
{
	t_fermat	*param;
	t_cut		*cuts;

	param = (t_fermat *)fermat->obj_param;
	param->origin.val[0] += dir.val[0] * fact;
	param->origin.val[1] += dir.val[1] * fact;
	param->origin.val[2] += dir.val[2] * fact;
	cuts = fermat->cuts;
	while (cuts)
	{
		if (cuts->move && cuts->cut_type != CUT_STATIC)
			cuts->move(cuts, dir, fact);
		cuts = cuts->next;
	}
}

t_3vecf	get_origin_fermat(t_obj *fermat) // a degager
{
	return (((t_fermat *)fermat->obj_param)->origin);
}

t_3vecf	get_fermat_origin(t_obj *fermat, t_fermat *param, int sp_id)
{
	if (sp_id)
		return (move_3vecf(param->origin, fermat->motions, sp_id));
	return (param->origin);
}

t_3vecf	get_normal_intersect_fermat(t_3vecf inter_point, t_obj *fermat,
	int sp_id)
{
	t_fermat	*param;
	t_3vecf		normal_inter;
	t_3vecf		cst;
	t_3vecf		fermat_origin;

	param = (t_fermat *)fermat->obj_param;
	fermat_origin = get_fermat_origin(fermat, param, sp_id);
	// sp_id ? move_3vecf(param->origin, fermat->motions, sp_id) : param->origin;
	cst.val[0] = inter_point.val[0] - fermat_origin.val[0];
	cst.val[1] = inter_point.val[1] - fermat_origin.val[1];
	cst.val[2] = inter_point.val[2] - fermat_origin.val[2];
	normal_inter.val[0] = 3 * cst.val[0] * cst.val[0];
	normal_inter.val[1] = 3 * cst.val[1] * cst.val[1];
	normal_inter.val[2] = 3 * cst.val[2] * cst.val[2];
	normalize_3vecf(&normal_inter);
	return (normal_inter);
}

t_4vecf	get_fermat_cubic_cst(t_leq l, t_3vecf fermat_origin)
{
	double		o[3];
	double		d[3];
	t_4vecf		cst;

	o[0] = l.orig.val[0] - fermat_origin.val[0];
	o[1] = l.orig.val[1] - fermat_origin.val[1];
	o[2] = l.orig.val[2] - fermat_origin.val[2];
	d[0] = l.dir.val[0];
	d[1] = l.dir.val[1];
	d[2] = l.dir.val[2];
	cst.val[0] = d[2] * d[2] * d[2] + d[0] * d[0] * d[0] + d[1]
		* d[1] * d[1];
	cst.val[1] = 3 * o[2] * d[2] * d[2] + 3 * o[0] * d[0] * d[0]
		+ 3 * o[1] * d[1] * d[1];
	cst.val[2] = 3 * o[2] * o[2] * d[2] + 3 * o[1] * o[1] * d[1]
		+ 3 * o[0] * o[0] * d[0];
	cst.val[3] = o[2] * o[2] * o[2] + o[0] * o[0] * o[0] + o[1]
		* o[1] * o[1] - 1;
	return (cst);
}

int		check_fermat_bnd(t_leq l, t_3vecf fermat_origin, double root)
{
	double	x;
	double	y;
	double	z;

	x = l.orig.val[0] - fermat_origin.val[0] + l.dir.val[0] * root;
	y = l.orig.val[1] - fermat_origin.val[1] + l.dir.val[1] * root;
	z = l.orig.val[2] - fermat_origin.val[2] + l.dir.val[2] * root;
	if (x > -3 && x < 3 && y > -3 && y < 3 && z > -3 && z < 3)
		return (1);
	return (0);
}

int		ray_intersect_fermat(t_leq l, t_obj *fermat, t_dist dist, int sp_id)
{
	t_4vecf		cst;
	t_3vecf		roots;
	int			check;
	t_3vecf		fermat_origin;
	int			i;

	i = -1;
	check = 0;
	fermat_origin = get_fermat_origin(fermat, (t_fermat *)fermat->obj_param,
		sp_id);
	cst = get_fermat_cubic_cst(l, fermat_origin);
	roots = solve_cubic(cst.val[0], cst.val[1], cst.val[2], cst.val[3]);
	while (++i < 3)
		if (check_fermat_bnd(l, fermat_origin, roots.val[i]))
			check |= is_closest_intersect(dist, roots.val[i]);
	return (check);
}

void	assign_fermat_function(t_obj *fermat)
{
	fermat->obj_type = OBJ_FERMAT;
	fermat->check_inside = &check_inside_fermat;
	fermat->ray_intersect = &ray_intersect_fermat;
	fermat->get_normal_inter = &get_normal_intersect_fermat;
	fermat->get_origin = &get_origin_fermat;
	fermat->move = &move_fermat;
	fermat->rotate = NULL;
	fermat->get_text_coordinate = &get_text_coordinate_fermat;
}

void	generate_new_fermat(t_data *data)
{
	t_obj		*fermat;
	t_fermat	*param;
	t_3vecf		dir;

	dir = mult_3vecf_33matf(mult_3vecf_33matf(window_to_view(0, 0,
		data->size.val[0], data->size.val[1]), data->rot_mat[1]),
			data->rot_mat[0]);
	normalize_3vecf(&dir);
	if (!(fermat = ft_memalloc(sizeof(t_obj))))
		return ;
	if (!(param = ft_memalloc(sizeof(t_fermat))))
		return ;
	param->origin = add_3vecf(data->camera->origin, product_c3vecf(dir, 2));
	fermat->obj_param = param;
	assign_fermat_function(fermat);
	fermat->text = generate_random_texture(fermat);
	set_bump_own(fermat);
	add_object(fermat, data);
	data->new_obj = 1;
}
