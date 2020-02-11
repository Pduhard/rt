/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   random.c                                         .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: pduhard- <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2020/02/11 17:45:04 by pduhard-     #+#   ##    ##    #+#       */
/*   Updated: 2020/02/11 18:17:55 by pduhard-    ###    #+. /#+    ###.fr     */
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
