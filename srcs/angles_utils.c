/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   angles_utils.c                                   .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: aplat <aplat@student.le-101.fr>            +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2019/12/21 22:47:15 by pduhard-     #+#   ##    ##    #+#       */
/*   Updated: 2020/02/07 02:47:13 by aplat       ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "rt.h"

int		is_null(double value)
{
	return (value < 0.0001 && value > -0.0001);
}

int		is_null_3vecf(t_3vecf t)
{
	if (t.val[0] == 0 && t.val[1] == 0 && t.val[2] == 0)
		return (0);
	return (1);
}

double	degree_to_radian(double degree)
{
	return (degree * _M_PI_180);
}
