/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   ft_ulltoa.c                                      .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: pduhard- <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/10/27 05:58:07 by pduhard-     #+#   ##    ##    #+#       */
/*   Updated: 2018/11/03 02:24:46 by pduhard-    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "libft.h"

static void	ft_getnumber(unsigned long long n,
		unsigned long long div, int size, char **array)
{
	char	*nb;
	int		i;

	i = 0;
	if (!(nb = malloc(size + 1)))
		array = NULL;
	while (div > 0)
	{
		nb[i++] = n / div + '0';
		n %= div;
		div /= 10;
	}
	nb[i] = '\0';
	*array = nb;
}

int			ft_ulltoa(unsigned long long n, char **array)
{
	unsigned long long		div;
	int						size;

	size = 0;
	div = 1;
	while (n / div > 9)
	{
		size++;
		div *= 10;
	}
	ft_getnumber(n, div, size, array);
	return (0);
}
