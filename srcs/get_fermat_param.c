/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_fermat_param.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aplat <aplat@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/18 01:25:59 by aplat             #+#    #+#             */
/*   Updated: 2020/05/18 01:31:18 by aplat            ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

t_2vecf	get_text_coordinate_fermat(t_3vecf inter_point, t_3vecf normal_inter,
	t_obj *fermat)
{
	return ((t_2vecf){{0, 0}});
	(void)normal_inter;
	(void)inter_point;
	(void)fermat;
}

t_3vecf	get_origin_fermat(t_obj *fermat)
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
	cst.val[0] = inter_point.val[0] - fermat_origin.val[0];
	cst.val[1] = inter_point.val[1] - fermat_origin.val[1];
	cst.val[2] = inter_point.val[2] - fermat_origin.val[2];
	normal_inter.val[0] = 3 * cst.val[0] * cst.val[0];
	normal_inter.val[1] = 3 * cst.val[1] * cst.val[1];
	normal_inter.val[2] = 3 * cst.val[2] * cst.val[2];
	normalize_3vecf(&normal_inter);
	return (normal_inter);
}
