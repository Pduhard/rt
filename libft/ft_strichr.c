/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   ft_strichr.c                                     .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: pduhard- <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/11/08 06:08:33 by pduhard-     #+#   ##    ##    #+#       */
/*   Updated: 2018/11/08 06:18:09 by pduhard-    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "libft.h"

int		ft_strichr(const char *s, int c)
{
	char	chr;
	int		i;

	i = 0;
	chr = (char)c;
	if (s == NULL)
		return (0);
	while (*s != chr && *s != '\0')
	{
		s++;
		i++;
	}
	if (*s != chr)
		return (-1);
	return (i);
}
