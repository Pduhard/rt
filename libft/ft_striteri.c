/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   ft_striteri.c                                    .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: pduhard- <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/10/03 18:52:22 by pduhard-     #+#   ##    ##    #+#       */
/*   Updated: 2018/10/05 14:05:04 by pduhard-    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "libft.h"

void	ft_striteri(char *s, void (*f)(unsigned int, char*))
{
	size_t len;
	size_t i;

	if (s != NULL && (*f) != NULL)
	{
		len = ft_strlen(s);
		i = -1;
		while (++i < len)
			(*f)((unsigned int)i, &s[i]);
	}
}
