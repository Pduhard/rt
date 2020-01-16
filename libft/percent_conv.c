/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   percent_conv.c                                   .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: pduhard- <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/11/02 04:13:46 by pduhard-     #+#   ##    ##    #+#       */
/*   Updated: 2018/11/02 04:20:39 by pduhard-    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "ft_printf.h"

void	make_percent_conv(char **converted, t_conv *data)
{
	*converted = ft_strdup("%");
	display_str_flags(converted, data);
	data->size = ft_strlen(*converted);
}
