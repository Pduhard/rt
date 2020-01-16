/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   ft_strmapi.c                                     .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: pduhard- <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/10/03 19:02:56 by pduhard-     #+#   ##    ##    #+#       */
/*   Updated: 2018/10/04 21:12:12 by pduhard-    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strmapi(char const *s, char (*f)(unsigned int, char))
{
	int		len;
	int		i;
	char	*str;

	if (s != NULL)
	{
		len = (int)ft_strlen((char*)s);
		if (!(str = malloc(len + 1)))
			return (NULL);
		i = -1;
		while (++i < len)
			str[i] = (*f)((unsigned int)i, s[i]);
		str[i] = '\0';
		return (str);
	}
	return (NULL);
}
