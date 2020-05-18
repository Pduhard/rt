/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hyperboloid_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aplat <aplat@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/18 02:08:02 by aplat             #+#    #+#             */
/*   Updated: 2020/05/18 02:08:39 by aplat            ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

int		check_inside_hyperboloid(t_3vecf point, t_obj *hyperboloid)
{
	t_hyperboloid	*param;

	param = (t_hyperboloid *)hyperboloid->obj_param;
	point = sub_3vecf(point, param->origin);
	point.val[0] = (point.val[0] * point.val[0])
		/ (param->x_fact * param->x_fact);
	point.val[1] = (point.val[1] * point.val[1])
		/ (param->y_fact * param->y_fact);
	point.val[2] = (point.val[2] * point.val[2])
		/ (param->z_fact * param->z_fact);
	if (point.val[0] - point.val[1] + point.val[2] + param->surface > 0)
		return (0);
	return (1);
}

void	move_hyperboloid(t_obj *hyperboloid, t_3vecf dir, double fact)
{
	t_hyperboloid	*param;
	t_cut			*cuts;

	param = (t_hyperboloid *)hyperboloid->obj_param;
	param->origin.val[0] += dir.val[0] * fact;
	param->origin.val[1] += dir.val[1] * fact;
	param->origin.val[2] += dir.val[2] * fact;
	cuts = hyperboloid->cuts;
	while (cuts)
	{
		if (cuts->move && cuts->cut_type != CUT_STATIC)
			cuts->move(cuts, dir, fact);
		cuts = cuts->next;
	}
}

void	assign_hyperboloid_function(t_obj *hyperboloid)
{
	hyperboloid->obj_type = OBJ_HYPERBOLOID;
	hyperboloid->check_inside = &check_inside_hyperboloid;
	hyperboloid->ray_intersect = &ray_intersect_hyperboloid;
	hyperboloid->get_normal_inter = &get_normal_intersect_hyperboloid;
	hyperboloid->get_origin = &get_origin_hyperboloid;
	hyperboloid->move = &move_hyperboloid;
	hyperboloid->rotate = NULL;
	hyperboloid->get_text_coordinate = &get_text_coordinate_hyperboloid;
}

void	generate_new_hyperboloid(t_data *data)
{
	t_obj			*hyperboloid;
	t_hyperboloid	*param;
	t_3vecf			dir;

	dir = mult_3vecf_33matf(mult_3vecf_33matf(window_to_view(0, 0,
		data->size.val[0], data->size.val[1]), data->rot_mat[1]),
			data->rot_mat[0]);
	normalize_3vecf(&dir);
	if (!(hyperboloid = ft_memalloc(sizeof(t_obj))))
		return ;
	if (!(param = ft_memalloc(sizeof(t_hyperboloid))))
		return ;
	param->origin = add_3vecf(data->camera->origin, product_c3vecf(dir, 2));
	param->x_fact = get_random_number((time(NULL) * 0xcacacaca) << 16) * 2.5;
	param->y_fact = get_random_number((time(NULL) * 0xabcdef99) << 4) * 2.5;
	param->z_fact = get_random_number((time(NULL) * 0xff3672ff) << 3) * 2.5;
	param->surface = get_random_number(time(NULL)) > 0.5 ? -1 : 1;
	hyperboloid->obj_param = param;
	assign_hyperboloid_function(hyperboloid);
	hyperboloid->text = generate_random_texture(hyperboloid);
	set_bump_own(hyperboloid);
	add_object(hyperboloid, data);
	data->new_obj = 1;
}
