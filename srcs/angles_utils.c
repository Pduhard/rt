/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   angles_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aplat <aplat@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/15 17:12:37 by aplat             #+#    #+#             */
/*   Updated: 2020/05/15 17:12:39 by aplat            ###   ########lyon.fr   */
/*                                                                            */
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
