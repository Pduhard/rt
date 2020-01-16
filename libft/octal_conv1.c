/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   octal_conv1.c                                    .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: pduhard- <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/10/27 03:00:59 by pduhard-     #+#   ##    ##    #+#       */
/*   Updated: 2018/11/08 01:44:55 by pduhard-    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "ft_printf.h"

char	*ull_o_conv(unsigned long long n, t_conv *data)
{
	char	*converted;
	char	*tmp;

	if (ft_strchr(data->flags, '-') != NULL)
		if ((tmp = ft_strchr(data->flags, '0')) != NULL)
			*tmp = '9';
	if ((tmp = ft_strchr(data->flags, '+')) != NULL)
		*tmp = '9';
	if ((tmp = ft_strchr(data->flags, ' ')) != NULL)
		*tmp = '9';
	converted = ft_baseconvert("01234567", n);
	return (converted);
}

char	*b_conv(unsigned long long n, t_conv *data, int size)
{
	char	*converted;
	char	*tmp;
	int		len;

	if ((tmp = ft_strchr(data->flags, '+')) != NULL)
		*tmp = '9';
	if ((tmp = ft_strchr(data->flags, ' ')) != NULL)
		*tmp = '9';
	converted = ft_baseconvert("01", n);
	if ((len = (int)ft_strlen(converted)) < size)
	{
		tmp = ft_strfillnew('0', size - len);
		converted = ft_strfjoin(tmp, converted);
	}
	return (converted);
}
