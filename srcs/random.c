/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   random.c                                         .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: pduhard- <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2020/02/11 17:45:04 by pduhard-     #+#   ##    ##    #+#       */
/*   Updated: 2020/03/12 18:13:38 by pduhard-         ###   ########lyon.fr   */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

/*	https://en.wikipedia.org/wiki/Xorshift	*/

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
