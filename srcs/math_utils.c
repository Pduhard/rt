/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   math_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aplat <aplat@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/15 17:12:37 by aplat             #+#    #+#             */
/*   Updated: 2020/05/18 02:40:13 by aplat            ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

int		is_null(double value)
{
	return (value < 0.0001 && value > -0.0001);
}

int		is_null_3vecf(t_3vecf vec)
{
	if (is_null(vec.val[0]) && is_null(vec.val[1]) && is_null(vec.val[2]))
		return (1);
	return (0);
}

double	degree_to_radian(double degree)
{
	return (degree * _M_PI_180);
}
