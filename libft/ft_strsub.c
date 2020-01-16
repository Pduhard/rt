/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   ft_strsub.c                                      .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: pduhard- <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/10/03 19:47:50 by pduhard-     #+#   ##    ##    #+#       */
/*   Updated: 2019/01/15 04:23:16 by pduhard-    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strsub(char const *s, unsigned int start, size_t len)
{
	unsigned int	end;
	char			*str;
	int				i;

	if (s == NULL || len > 4294967295)
		return (ft_strnew(0));
	end = start + (unsigned int)len;
	i = 0;
	if (!(str = (char *)malloc(len + 1)))
		return (ft_strnew(0));
	while (start < end && s[start])
		str[i++] = s[start++];
	str[i] = '\0';
	return (str);
}
