/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cut_move.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aplat <aplat@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/18 01:12:29 by aplat             #+#    #+#             */
/*   Updated: 2020/05/18 01:12:30 by aplat            ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

void		move_cut_plane(t_cut *cut, t_3vecf dir, double fact)
{
	t_plane	*param;

	param = (t_plane *)cut->cut_param;
	param->origin.val[0] += dir.val[0] * fact;
	param->origin.val[1] += dir.val[1] * fact;
	param->origin.val[2] += dir.val[2] * fact;
}

void		move_cut_sphere(t_cut *cut, t_3vecf dir, double fact)
{
	t_sphere	*param;

	param = (t_sphere *)cut->cut_param;
	param->origin.val[0] += dir.val[0] * fact;
	param->origin.val[1] += dir.val[1] * fact;
	param->origin.val[2] += dir.val[2] * fact;
}

void		rotate_cut_plane(t_cut *cut, t_3vecf orig, t_33matf rot_mat[2])
{
	t_plane	*param;

	param = (t_plane *)cut->cut_param;
	param->origin = sub_3vecf(param->origin, orig);
	param->origin = mult_3vecf_33matf(param->origin, rot_mat[1]);
	param->origin = mult_3vecf_33matf(param->origin, rot_mat[0]);
	param->normal = mult_3vecf_33matf(param->normal, rot_mat[1]);
	param->normal = mult_3vecf_33matf(param->normal, rot_mat[0]);
	param->origin = add_3vecf(param->origin, orig);
}
