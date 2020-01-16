/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   float_conv.c                                     .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: pduhard- <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/11/05 20:53:16 by pduhard-     #+#   ##    ##    #+#       */
/*   Updated: 2018/11/12 15:52:03 by pduhard-    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "ft_printf.h"

char	*ldfloat_conv(long double n, t_conv *data, int *neg)
{
	char	*converted;

	data->prec = data->prec == -1 ? 6 : data->prec;
	*neg = ft_ldtoa(n, &converted, data->prec + 1);
	round_float(&converted, data->prec);
	return (converted);
}

void	make_float_conv(char **conv, t_conv *data, va_list ap)
{
	int		neg;

	if (data->mod && ft_strcmp(data->mod, "ll") == 0)
		*conv = ldfloat_conv(va_arg(ap, long double), data, &neg);
	else
		*conv = ldfloat_conv(va_arg(ap, double), data, &neg);
	display_signed_flags(conv, data, neg);
	data->size = ft_strlen(*conv);
}
