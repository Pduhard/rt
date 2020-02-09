/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   color_filter.c                                   .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: pduhard- <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2020/02/09 23:31:11 by pduhard-     #+#   ##    ##    #+#       */
/*   Updated: 2020/02/09 23:50:52 by pduhard-    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "rt.h"

t_3vecf		apply_color_filter_sepia(t_3vecf color)
{
	t_3vecf	sepia;

	sepia.val[0] = 0.393 * color.val[0] + 0.769 * color.val[1] + 0.189 * color.val[2];
	sepia.val[1] = 0.349 * color.val[0] + 0.686 * color.val[1] + 0.168 * color.val[2];
	sepia.val[2] = 0.272 * color.val[0] + 0.534 * color.val[1] + 0.131 * color.val[2];
	return (sepia);
}
