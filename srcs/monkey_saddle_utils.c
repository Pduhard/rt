/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monkey_saddle_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aplat <aplat@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/18 04:45:51 by aplat             #+#    #+#             */
/*   Updated: 2020/05/18 04:46:09 by aplat            ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

int		check_inside_monkey_saddle(t_3vecf point, t_obj *monkey_saddle)
{
	return (0);
	(void)point;
	(void)monkey_saddle;
}

void	move_monkey_saddle(t_obj *monkey_saddle, t_3vecf dir, double fact)
{
	t_monkey_saddle	*param;
	t_cut			*cuts;

	param = (t_monkey_saddle *)monkey_saddle->obj_param;
	param->origin.val[0] += dir.val[0] * fact;
	param->origin.val[1] += dir.val[1] * fact;
	param->origin.val[2] += dir.val[2] * fact;
	cuts = monkey_saddle->cuts;
	while (cuts)
	{
		if (cuts->move && cuts->cut_type != CUT_STATIC)
			cuts->move(cuts, dir, fact);
		cuts = cuts->next;
	}
}

void	assign_monkey_saddle_function(t_obj *monkey_saddle)
{
	monkey_saddle->obj_type = OBJ_MONKEY_SADDLE;
	monkey_saddle->check_inside = &check_inside_monkey_saddle;
	monkey_saddle->ray_intersect = &ray_intersect_monkey_saddle;
	monkey_saddle->get_normal_inter = &get_normal_intersect_monkey_saddle;
	monkey_saddle->get_origin = &get_origin_monkey_saddle;
	monkey_saddle->move = &move_monkey_saddle;
	monkey_saddle->rotate = NULL;
	monkey_saddle->get_text_coordinate = &get_text_coordinate_monkey_saddle;
}

void	generate_new_monkey_saddle(t_data *data)
{
	t_obj			*monkey_saddle;
	t_monkey_saddle	*param;
	t_3vecf			dir;

	dir = mult_3vecf_33matf(mult_3vecf_33matf(window_to_view(0, 0,
		data->size.val[0], data->size.val[1]), data->rot_mat[1]),
			data->rot_mat[0]);
	normalize_3vecf(&dir);
	if (!(monkey_saddle = ft_memalloc(sizeof(t_obj))))
		return ;
	if (!(param = ft_memalloc(sizeof(t_monkey_saddle))))
		return ;
	param->origin = add_3vecf(data->camera->origin, product_c3vecf(dir, 2));
	monkey_saddle->obj_param = param;
	assign_monkey_saddle_function(monkey_saddle);
	monkey_saddle->text = generate_random_texture(monkey_saddle);
	set_bump_own(monkey_saddle);
	add_object(monkey_saddle, data);
	data->new_obj = 1;
}
