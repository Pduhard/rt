/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   unsigned_conv1.c                                 .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: pduhard- <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/10/27 03:00:59 by pduhard-     #+#   ##    ##    #+#       */
/*   Updated: 2018/11/02 21:50:36 by pduhard-    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "ft_printf.h"

char	*ull_conv(unsigned long long n, t_conv *data)
{
	char	*converted;
	char	*tmp;

	if ((tmp = ft_strchr(data->flags, ' ')) != NULL)
		*tmp = '9';
	if ((tmp = ft_strchr(data->flags, '+')) != NULL)
		*tmp = '9';
	ft_ulltoa(n, &converted);
	return (converted);
}
