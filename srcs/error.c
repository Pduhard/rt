/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aplat <aplat@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/15 18:05:34 by aplat             #+#    #+#             */
/*   Updated: 2020/05/15 18:11:30 by aplat            ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

void	ft_throw_error(char *message, ...)
{
	va_list	ap;

	va_start(ap, message);
	vdprintf(2, message, ap);
	va_end(ap);
}

int		ft_return_error(int ret, char *message, ...)
{
	va_list	ap;

	va_start(ap, message);
	vdprintf(2, message, ap);
	va_end(ap);
	return (ret);
}

int		ft_memalloc_error(int ret, size_t size)
{
	dprintf(2, ERRORMEM, size);
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
