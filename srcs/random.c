/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   random.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aplat <aplat@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/15 20:57:30 by aplat             #+#    #+#             */
/*   Updated: 2020/05/15 20:57:31 by aplat            ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

double		get_random_number(unsigned int x)
{
	x ^= x << 17;
	x ^= x >> 5;
	x ^= x << 13;
	return ((double)x / (double)0xffffffff);
}

t_4vecf		generate_random_color(unsigned int x, double transp_f)
{
	t_4vecf	color;

	color.val[0] = get_random_number(x << 6);
	color.val[1] = get_random_number(x >> 4);
	color.val[2] = get_random_number(x << 7);
	color.val[3] = get_random_number(x << 7) * transp_f * TRANSP_F;
	return (color);
}

int			generate_random_enum(int e_max)
{
	return ((int)(get_random_number(time(NULL)) * e_max));
}
