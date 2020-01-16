/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   ft_strncmp.c                                     .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: pduhard- <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/10/02 18:45:42 by pduhard-     #+#   ##    ##    #+#       */
/*   Updated: 2018/12/12 08:52:02 by pduhard-    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "libft.h"

int		ft_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t cpt;

	cpt = 1;
	if (n < 1)
		return (0);
	while (*s1 == *s2 && cpt < n && *s1 != '\0')
	{
		cpt++;
		s1++;
		s2++;
	}
	return ((unsigned char)*s1 - *s2);
}
