/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   ft_strrchr.c                                     .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: pduhard- <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/10/03 15:03:35 by pduhard-     #+#   ##    ##    #+#       */
/*   Updated: 2018/10/05 23:58:36 by pduhard-    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	char	chr;
	int		i;

	i = 0;
	while (s[i] != '\0')
		i++;
	chr = (char)c;
	while (s[i] != chr && i > 0)
		i--;
	if (s[i] != chr)
		return (NULL);
	return ((char*)&s[i]);
}
