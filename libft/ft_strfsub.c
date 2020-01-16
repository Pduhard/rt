/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   ft_strfsub.c                                     .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: pduhard- <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/10/03 19:47:50 by pduhard-     #+#   ##    ##    #+#       */
/*   Updated: 2018/11/10 18:43:08 by pduhard-    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "libft.h"

void	ft_strfsub(char **s, unsigned int start, size_t len)
{
	unsigned int	end;
	char			*str;
	int				i;

	if (*s == NULL || len > 4294967295)
		*s = ft_strnew(0);
	else
	{
		end = start + (unsigned int)len;
		i = 0;
		if (!(str = (char *)malloc(len + 1)))
			*s = ft_strnew(0);
		while (start < end)
			str[i++] = s[0][start++];
		str[i] = '\0';
		ft_strdel(s);
		*s = str;
	}
}
