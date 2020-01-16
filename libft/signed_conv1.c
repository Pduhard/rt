/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   signed_conv1.c                                   .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: pduhard- <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/10/27 03:00:59 by pduhard-     #+#   ##    ##    #+#       */
/*   Updated: 2018/11/12 15:51:33 by pduhard-    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "ft_printf.h"

int		display_separator(char **converted)
{
	int		len;
	int		new_len;
	char	*new;
	int		i;
	int		j;

	i = 0;
	len = (int)ft_strlen(*converted);
	new_len = len + len / 3 - (len % 2 == 0 ? 1 : 0);
	if (!(new = (char *)malloc(len + 1)))
		return (0);
	j = 0;
	while (j < len)
	{
		new[i++] = converted[0][j++];
		if ((len - j) % 3 == 0 && len - j != 0)
			new[i++] = ' ';
	}
	new[i] = '\0';
	new = ft_strfjoin(new, ft_strdup(&converted[0][j]));
	ft_strdel(converted);
	*converted = new;
	return (0);
}

char	*long_long_conv(long long n, int *neg, t_conv *data)
{
	char	*converted;
	char	*tmp;

	if (ft_strchr(data->flags, '-') != NULL)
		if ((tmp = ft_strchr(data->flags, '0')) != NULL)
			*tmp = '9';
	*neg = ft_lltoa(n, &converted);
	if (ft_strchr(data->flags, '\'') != NULL)
		display_separator(&converted);
	return (converted);
}
