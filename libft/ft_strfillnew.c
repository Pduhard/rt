/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   ft_strfillnew.c                                  .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: pduhard- <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/10/27 22:29:29 by pduhard-     #+#   ##    ##    #+#       */
/*   Updated: 2018/10/28 02:28:30 by pduhard-    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strfillnew(char c, int len)
{
	char	*str;
	int		i;

	i = 0;
	if (len <= 0)
		return (ft_strnew(0));
	if (!(str = (char *)malloc(len + 1)))
		return (NULL);
	while (i < len)
		str[i++] = c;
	str[i] = '\0';
	return (str);
}
