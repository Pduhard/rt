/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   motion.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aplat <aplat@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/15 19:52:13 by aplat             #+#    #+#             */
/*   Updated: 2020/05/15 19:52:31 by aplat            ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

t_3vecf		move_3vecf(t_3vecf vec, t_motion *moves, int sp_id)
{
	if (!moves)
		return (vec);
	while (moves && sp_id > 0)
	{
		if (moves->spf > sp_id)
		{
			vec = add_3vecf(vec, product_c3vecf(moves->dir, (double)sp_id
					* moves->speed_fact * (double)MOTION_STEP));
			return (vec);
		}
		else
		{
			vec = add_3vecf(vec, product_c3vecf(moves->dir, (double)moves->spf
					* moves->speed_fact * (double)MOTION_STEP));
			sp_id -= moves->spf;
		}
		moves = moves->next;
	}
	return (vec);
}

void		move_objects(t_obj *objs)
{
	while (objs)
	{
		if (objs->motions)
			objs->move(objs, objs->motions->dir,
				objs->motions->speed_fact * MOTION_STEP);
		objs = objs->next;
	}
}

t_3vecf		motion_trace(t_3vecf orig, t_3vecf dir, t_data *data)
{
	t_3vecf	color;
	t_3vecf	ray_color;
	int		i;
	double	tt_vel;
	double	vel;

	i = -1;
	vel = 0;
	tt_vel = 0;
	color = assign_3vecf(0, 0, 0);
	while (++i < MOTION_SPP)
	{
		ray_color = ray_trace((t_leq){orig, dir}, BIAS, MAX_VIEW, data, 6, i);
		vel = i + 1 == MOTION_SPP ? MOTION_FVEL : 1.;
		color.val[0] += ray_color.val[0] * vel;
		color.val[1] += ray_color.val[1] * vel;
		color.val[2] += ray_color.val[2] * vel;
		tt_vel += vel;
	}
	color.val[0] /= tt_vel;
	color.val[1] /= tt_vel;
	color.val[2] /= tt_vel;
	return (color);
}
