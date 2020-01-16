/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   ft_countparts.c                                  .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: pduhard- <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/10/03 21:47:57 by pduhard-     #+#   ##    ##    #+#       */
/*   Updated: 2018/10/03 22:40:30 by pduhard-    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "libft.h"

int		ft_countparts(char del, char const *str)
{
	int	count;
	int	i;
	int word;

	i = 0;
	count = 0;
	if (str[0] == '\0' || (str[0] == del && str[1] == '\0'))
		return (0);
	while (str[i] != '\0')
	{
		word = 0;
		while (str[i] == del)
			i++;
		while (str[i] != del && str[i] != '\0')
		{
			i++;
			word = 1;
		}
		if (word == 1)
			count++;
	}
	return (count);
}
