/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   ft_strncpy.c                                     .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: pduhard- <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/10/02 18:40:01 by pduhard-     #+#   ##    ##    #+#       */
/*   Updated: 2018/10/05 23:51:46 by pduhard-    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strncpy(char *dst, const char *src, size_t len)
{
	size_t cpt;

	cpt = 0;
	while (src[cpt] != '\0' && cpt < len)
	{
		dst[cpt] = src[cpt];
		cpt++;
	}
	while (cpt < len)
		dst[cpt++] = '\0';
	return (dst);
}
