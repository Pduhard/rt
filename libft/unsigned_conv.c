/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unsigned_conv.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aplat <aplat@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/27 02:26:04 by pduhard-          #+#    #+#             */
/*   Updated: 2020/05/20 10:46:00 by aplat            ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

void	make_unsigned_conv(char **conv, t_conv *data, va_list ap)
{
	int	neg;

	neg = 0;
	if (data->type == 'u' && data->mod)
	{
		if (ft_strcmp(data->mod, "hh") == 0)
			*conv = ull_conv((unsigned char)va_arg(ap, unsigned int), data);
		else if (ft_strcmp(data->mod, "h") == 0)
			*conv = ull_conv((unsigned short)va_arg(ap, unsigned int), data);
		else if (ft_strcmp(data->mod, "l") == 0)
			*conv = ull_conv(va_arg(ap, unsigned long), data);
		else if (ft_strcmp(data->mod, "ll") == 0)
			*conv = ull_conv(va_arg(ap, unsigned long long), data);
		else if (ft_strcmp(data->mod, "j") == 0)
			*conv = ull_conv(va_arg(ap, unsigned long long), data);
		else if (ft_strcmp(data->mod, "z") == 0)
			*conv = ull_conv(va_arg(ap, size_t), data);
	}
	else if (data->type == 'U')
		*conv = ull_conv(va_arg(ap, unsigned long long), data);
	else
		*conv = ull_conv(va_arg(ap, unsigned int), data);
	display_flags(conv, data, &neg);
	data->size = ft_strlen(*conv);
}
