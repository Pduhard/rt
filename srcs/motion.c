/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   motion.c                                         .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: pduhard- <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2020/02/04 00:42:41 by pduhard-     #+#   ##    ##    #+#       */
/*   Updated: 2020/02/04 08:01:13 by pduhard-    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "rt.h"

t_3vecf		move_3vecf(t_3vecf vec, t_motion *moves, int sp_id)
{
//	int	spf;

	if (!moves)
		return (vec);
//	spf = moves->spf;
	while (moves && sp_id > 0)
	{
//		printf("%d %d\n", moves->spf, sp_id);
		if (moves->spf > sp_id)
		{
			vec.val[0] += moves->dir.val[0] * (double)sp_id * moves->speed_fact * (double)MOTION_STEP;
			vec.val[1] += moves->dir.val[1] * (double)sp_id * moves->speed_fact * (double)MOTION_STEP;
			vec.val[2] += moves->dir.val[2] * (double)sp_id * moves->speed_fact * (double)MOTION_STEP;
			return (vec);
		}
		else
		{
			vec.val[0] += moves->dir.val[0] * (double)moves->spf * moves->speed_fact * (double)MOTION_STEP;
			vec.val[1] += moves->dir.val[1] * (double)moves->spf * moves->speed_fact * (double)MOTION_STEP;
			vec.val[2] += moves->dir.val[2] * (double)moves->spf * moves->speed_fact * (double)MOTION_STEP;
			sp_id -= moves->spf;
		}
		moves = moves->next;
	}
	return (vec);
}

void		save_origin(t_obj *objs)
{
	while (objs)
	{
		if (objs->motions)
		{
			//objs->get_origin(objs);
		}
		objs = objs->next;
	}
}

void		move_objects(t_obj *objs)
{
	while (objs)
	{
		if (objs->motions)
			objs->move(objs, objs->motions->dir, objs->motions->speed_fact * MOTION_STEP);
		objs = objs->next;
	}
}

/*void		reset_objs_position(t_obj *objs)
{
	while (objs)
	{
		if (objs->motions)
			objs->move(objs, assign_3vecf(-objs->motions->dir.val[0], -objs->motions->dir.val[1], -objs->motions->dir.val[2]), objs->motions->speed_fact * MOTION_STEP * MOTION_SPP);
		objs = objs->next;	
	}
}*/

t_3vecf		motion_trace(t_3vecf orig, t_3vecf dir, t_data *data)
{
	t_3vecf	color;
	t_3vecf	ray_color;
	int		i;

	i = -1;
	double	tt_vel;
	double	vel;

	vel = 0;
	tt_vel = 0;
/*	data->objs->motions->spf = 16;
	data->objs->motions->next->spf = 16;
	data->objs->next->motions->spf = 32;
	data->objs->next->motions->next->spf = 0;
	color = assign_3vecf(0, 0, 0);
*/
//	color = ray_trace(orig, dir, BIAS, MAX_VIEW, data, 6);
//	move_objects(data->objs);
	while (++i < MOTION_SPP)
	{
		ray_color = ray_trace(orig, dir, BIAS, MAX_VIEW, data, 6, i);
	//	move_objects(data->objs);
		vel = i + 1 == MOTION_SPP ? 8. : 1.;
		color.val[0] += ray_color.val[0] * vel;
		color.val[1] += ray_color.val[1] * vel;
		color.val[2] += ray_color.val[2] * vel;
		tt_vel += vel;
	}
	color.val[0] /= tt_vel;
	color.val[1] /= tt_vel;
	color.val[2] /= tt_vel;
//	reset_objs_position(data->objs);
	return (color);
}
