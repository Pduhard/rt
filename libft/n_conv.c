/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   n_conv.c                                         .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: pduhard- <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/11/10 19:32:00 by pduhard-     #+#   ##    ##    #+#       */
/*   Updated: 2018/11/11 13:50:50 by pduhard-    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "ft_printf.h"

void	make_n_conv(char **converted, t_conv *data, va_list ap)
{
	data->n_conv = va_arg(ap, long long*);
	data->size = 0;
	*converted = NULL;
}

void	get_n_value(int sum, t_conv *data)
{
	if (data->mod)
	{
		if (ft_strcmp(data->mod, "hh") == 0)
			*(data->n_conv) = (signed char)sum;
		else if (ft_strcmp(data->mod, "h") == 0)
			*(data->n_conv) = (short)sum;
		else if (ft_strcmp(data->mod, "l") == 0)
			*(data->n_conv) = (long)sum;
		else if (ft_strcmp(data->mod, "ll") == 0)
			*(data->n_conv) = (long long)sum;
		else if (ft_strcmp(data->mod, "j") == 0)
			*(data->n_conv) = (intmax_t)sum;
		else if (ft_strcmp(data->mod, "z") == 0)
			*(data->n_conv) = sum;
	}
	else
		*(data->n_conv) = sum;
}
