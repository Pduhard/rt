/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   ft_strdup.c                                      .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: pduhard- <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/10/03 15:56:05 by pduhard-     #+#   ##    ##    #+#       */
/*   Updated: 2018/11/02 00:26:06 by pduhard-    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include <stdlib.h>

char	*ft_strdup(const char *src)
{
	int		index;
	char	*res;

	if (src == NULL)
		return (NULL);
	index = 0;
	while (src[index])
		index++;
	if (!(res = malloc(index + 1)))
		return (NULL);
	index = 0;
	while (*src != '\0')
		res[index++] = *(src++);
	res[index] = '\0';
	return (res);
}
