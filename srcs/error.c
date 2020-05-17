/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aplat <aplat@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/15 18:05:34 by aplat             #+#    #+#             */
/*   Updated: 2020/05/15 20:17:56 by aplat            ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

void	ft_mem_error()
{
	ft_fdprintf(2, "Malloc error: exit\n");
	exit(0);
}

int		ft_memalloc_error(int ret, size_t size)
{
	dprintf(2,
		"internal error: too big allocation (try to allocate %zu bytes)\n",
			size);
	return (ret);
}

int		ft_usage_error(int ret, char *usage)
{
	dprintf(2, "Usage: %s\n", usage);
	return (ret);
}

int		syn_error(char *s1, char *s2, char *s3, char *s4)
{
	s3 = (!s3 ? "" : s3);
	s4 = (!s4 ? "" : s4);
	ft_fdprintf(2, "%s%s%s%s>\n", s1, s2, s3, s4);
	return (0);
}

int		error(char *s1, char *s2)
{
	s2 = (!s2 ? "" : s2);
	ft_fdprintf(2, "%s%s", s1, s2);
	return (0);
}
