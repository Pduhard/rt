/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   horse_saddle_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aplat <aplat@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/18 02:07:29 by aplat             #+#    #+#             */
/*   Updated: 2020/05/18 02:07:30 by aplat            ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

void	move_horse_saddle(t_obj *horse_saddle, t_3vecf dir, double fact)
{
	t_horse_saddle	*param;
	t_cut			*cuts;

	param = (t_horse_saddle *)horse_saddle->obj_param;
	param->origin.val[0] += dir.val[0] * fact;
	param->origin.val[1] += dir.val[1] * fact;
	param->origin.val[2] += dir.val[2] * fact;
	cuts = horse_saddle->cuts;
	while (cuts)
	{
		if (cuts->move && cuts->cut_type != CUT_STATIC)
			cuts->move(cuts, dir, fact);
		cuts = cuts->next;
	}
}

int		check_inside_horse_saddle(t_3vecf point, t_obj *horse_saddle)
{
	t_horse_saddle	*param;

	param = (t_horse_saddle *)horse_saddle->obj_param;
	point = sub_3vecf(point, param->origin);
	if (point.val[0] < -param->x_fact || point.val[0] > param->x_fact
		|| point.val[1] < -param->y_fact || point.val[1] > param->y_fact)
		return (0);
	point.val[0] = (point.val[0] * point.val[0])
		/ (param->x_fact * param->x_fact);
	point.val[1] = (point.val[1] * point.val[1])
		/ (param->y_fact * param->y_fact);
	if (!is_null(point.val[0] + point.val[1] + point.val[2]))
		return (0);
	return (1);
}

void	assign_horse_saddle_function(t_obj *horse_saddle)
{
	horse_saddle->obj_type = OBJ_HORSE_SADDLE;
	horse_saddle->check_inside = &check_inside_horse_saddle;
	horse_saddle->ray_intersect = &ray_intersect_horse_saddle;
	horse_saddle->get_normal_inter = &get_normal_intersect_horse_saddle;
	horse_saddle->get_origin = &get_origin_horse_saddle;
	horse_saddle->move = &move_horse_saddle;
	horse_saddle->rotate = NULL;
	horse_saddle->get_text_coordinate = &get_text_coordinate_horse_saddle;
}

void	generate_new_horse_saddle(t_data *data)
{
	t_obj			*horse_saddle;
	t_horse_saddle	*param;
	t_3vecf			dir;

	dir = mult_3vecf_33matf(mult_3vecf_33matf(window_to_view(0, 0,
		data->size.val[0], data->size.val[1]), data->rot_mat[1]),
			data->rot_mat[0]);
	normalize_3vecf(&dir);
	if (!(horse_saddle = ft_memalloc(sizeof(t_obj))))
		return ;
	if (!(param = ft_memalloc(sizeof(t_horse_saddle))))
		return ;
	param->origin = add_3vecf(data->camera->origin, product_c3vecf(dir, 2));
	param->x_fact = get_random_number((time(NULL) * 0xcacacaca) << 16) * 2.5;
	param->y_fact = get_random_number((time(NULL) * 0xabcdef99) << 4) * 2.5;
	horse_saddle->obj_param = param;
	assign_horse_saddle_function(horse_saddle);
	horse_saddle->text = generate_random_texture(horse_saddle);
	set_bump_own(horse_saddle);
	add_object(horse_saddle, data);
	data->new_obj = 1;
}
