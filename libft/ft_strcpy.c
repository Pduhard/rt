/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   ft_strcpy.c                                      .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: pduhard- <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/10/02 18:37:32 by pduhard-     #+#   ##    ##    #+#       */
/*   Updated: 2018/10/05 23:37:01 by pduhard-    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strcpy(char *dest, const char *src)
{
	int cpt;

	cpt = -1;
	while (src[++cpt] != '\0')
		dest[cpt] = src[cpt];
	dest[cpt] = '\0';
	return (dest);
}
