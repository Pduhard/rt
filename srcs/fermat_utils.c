/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fermat_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aplat <aplat@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/18 01:17:54 by aplat             #+#    #+#             */
/*   Updated: 2020/05/18 01:17:56 by aplat            ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

int		check_inside_fermat(t_3vecf point, t_obj *fermat)
{
	return (0);
	(void)point;
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
