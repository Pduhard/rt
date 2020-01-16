/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   signed_flags.c                                   .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: pduhard- <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/11/10 16:05:14 by pduhard-     #+#   ##    ##    #+#       */
/*   Updated: 2018/11/10 16:08:56 by pduhard-    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "ft_printf.h"

void	eof_signed_flags(char **conv, t_conv *data, int neg)
{
	int		i;

	i = 0;
	if (conv[0][i] != '0' && ft_isdigit(conv[0][i]))
	{
		if (ft_strchr(data->flags, '+'))
			make_c_join(neg == 1 ? "-" : "+", conv, 0);
		else
			make_c_join(neg == 1 ? "-" : " ", conv, 0);
	}
	else
	{
		while (conv[0][i + 1] == ' ')
			i++;
		if (ft_strchr(data->flags, '+'))
			conv[0][i] = neg == 1 ? '-' : '+';
		else
			conv[0][i] = neg == 1 ? '-' : ' ';
	}
}

void	display_signed_flags(char **conv, t_conv *data, int neg)
{
	display_str_flags(conv, data);
	if ((ft_strchr(data->flags, '+') || neg == 1
				|| ft_strchr(data->flags, ' ')) && ft_strichr(*conv, '.') == 1)
	{
		if (ft_strchr(data->flags, '+'))
			make_c_join(neg == 1 ? "-" : "+", conv, 0);
		else
			make_c_join(neg == 1 ? "-" : " ", conv, 0);
		if (conv[0][ft_strlen(*conv) - 1] == ' ')
			conv[0][ft_strlen(*conv) - 1] = '\0';
	}
	else if (ft_strchr(data->flags, '+') || neg == 1
			|| ft_strchr(data->flags, ' '))
		eof_signed_flags(conv, data, neg);
}
