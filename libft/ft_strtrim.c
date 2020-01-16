/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   ft_strtrim.c                                     .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: pduhard- <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/10/03 20:09:56 by pduhard-     #+#   ##    ##    #+#       */
/*   Updated: 2019/01/15 05:28:35 by pduhard-    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strtrim(char const *s)
{
	size_t				end;
	unsigned int		start;
	char				*str;

	start = -1;
	if (!s || !s[0])
		return (ft_strdup(s));
	end = ft_strlen(s) - 1;
	while (s[++start] == ' ' || s[start] == '\n' || s[start] == '\t')
		if (start == end)
		{
			if (!(str = (char*)malloc(sizeof(*str))))
				return (NULL);
			str[0] = '\0';
			return (str);
		}
	while (s[end] == ' ' || s[end] == '\n' || s[end] == '\t')
		end--;
	end = end - start + 1;
	if (!(str = ft_strsub(s, start, end)))
		return (NULL);
	str[end] = '\0';
	return (str);
}
