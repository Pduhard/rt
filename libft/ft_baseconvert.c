/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   ft_baseconvert.c                                 .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: pduhard- <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/10/05 18:28:20 by pduhard-     #+#   ##    ##    #+#       */
/*   Updated: 2018/10/29 01:09:19 by pduhard-    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "libft.h"

char	*ft_baseconvert(char *base, unsigned long long n)
{
	int						size;
	int						bs;
	char					*res;
	unsigned long long		n_cp;

	n_cp = n;
	size = 0;
	bs = (int)ft_strlen(base);
	if (n == 0)
		return (ft_strfillnew('0', 1));
	while (n_cp != 0)
	{
		n_cp /= bs;
		size++;
	}
	if (!(res = malloc(size + 1)))
		return (NULL);
	res[size] = '\0';
	while (size-- > 0)
	{
		res[size] = base[n % bs];
		n /= bs;
	}
	return (res);
}
