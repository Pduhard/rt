/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   ft_strmap.c                                      .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: pduhard- <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/10/03 18:56:27 by pduhard-     #+#   ##    ##    #+#       */
/*   Updated: 2018/10/04 21:11:01 by pduhard-    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strmap(char const *s, char (*f)(char))
{
	char	*str;
	int		i;
	int		len;

	if (s != NULL)
	{
		len = (int)ft_strlen((char*)s);
		if (!(str = malloc(len + 1)))
			return (NULL);
		i = -1;
		while (++i < len)
			str[i] = (*f)(s[i]);
		str[i] = '\0';
		return (str);
	}
	return (NULL);
}
