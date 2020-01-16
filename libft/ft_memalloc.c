/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   ft_memalloc.c                                    .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: pduhard- <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/10/03 18:13:46 by pduhard-     #+#   ##    ##    #+#       */
/*   Updated: 2018/10/03 18:22:01 by pduhard-    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memalloc(size_t size)
{
	void	*mem;
	char	*tmp;

	if (!(mem = malloc(size)))
		return (NULL);
	tmp = (char*)mem;
	while (size-- > 0)
		*(tmp++) = '\0';
	return (mem);
}
