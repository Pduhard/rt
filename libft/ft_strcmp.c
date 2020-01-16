/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   ft_strcmp.c                                      .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: pduhard- <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/10/02 18:44:46 by pduhard-     #+#   ##    ##    #+#       */
/*   Updated: 2018/12/01 04:25:11 by pduhard-    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "libft.h"

int		ft_strcmp(const char *s1, const char *s2)
{
	int cpt;
	int	res;

	cpt = 0;
	while (s1[cpt] == s2[cpt] && s1[cpt] != '\0' && s2[cpt] != '\0')
	{
		cpt++;
		if (s1[cpt] == '\0' && s2[cpt] == '\0')
			return (0);
		if (s1[cpt] == '\0' || s2[cpt] == '\0')
			break ;
	}
	res = s1[cpt] - s2[cpt];
	return ((unsigned char)s1[cpt] - (unsigned char)s2[cpt]);
}
