/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   angles_utils.c                                   .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: aplat <aplat@student.le-101.fr>            +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2019/12/21 22:47:15 by pduhard-     #+#   ##    ##    #+#       */
/*   Updated: 2020/02/07 06:41:09 by aplat       ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "rt.h"

int		is_null(double value)
{
	return (value < 0.0001 && value > -0.0001);
}

double	degree_to_radian(double degree)
{
	return (degree * _M_PI_180);
}
