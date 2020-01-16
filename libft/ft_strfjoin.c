/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   ft_strfjoin.c                                    .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: pduhard- <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/10/28 05:25:06 by pduhard-     #+#   ##    ##    #+#       */
/*   Updated: 2018/12/09 22:47:34 by sbedene     ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strfjoin(char *s1, char *s2)
{
	char *str;

	if (!(str = ft_strjoin(s1, s2)))
		return (ft_strnew(0));
	ft_strdel(&s1);
	ft_strdel(&s2);
	return (str);
}
