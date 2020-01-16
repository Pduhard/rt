/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   hex_conv1.c                                      .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: pduhard- <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/10/27 03:00:59 by pduhard-     #+#   ##    ##    #+#       */
/*   Updated: 2018/11/07 05:55:04 by pduhard-    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "ft_printf.h"

char	*ull_x_conv(unsigned long long n, int *neg, t_conv *data)
{
	char	*converted;
	char	*tmp;

	if ((tmp = ft_strchr(data->flags, '+')) != NULL)
		*tmp = '9';
	if ((tmp = ft_strchr(data->flags, ' ')) != NULL)
		*tmp = '9';
	if (data->type == 'X')
		converted = ft_baseconvert("0123456789ABCDEF", n);
	else
		converted = ft_baseconvert("0123456789abcdef", n);
	*neg = n == 0 ? 2 : 0;
	return (converted);
}
