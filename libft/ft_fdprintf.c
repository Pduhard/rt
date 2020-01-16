/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   ft_fdprintf.c                                    .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: pduhard- <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/11/08 07:19:32 by pduhard-     #+#   ##    ##    #+#       */
/*   Updated: 2018/11/11 17:29:55 by pduhard-    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "ft_printf.h"

int		ft_fdprintf(int fd, const char *format, ...)
{
	t_part	*wlst;
	t_part	*tmp;
	va_list	ap;
	int		ret;
	char	*str;

	va_start(ap, format);
	wlst = NULL;
	str = NULL;
	if (format)
		str = apply_font((char *)format, ap);
	if (format)
		wlst = parse_format(str, ap);
	tmp = wlst;
	ft_strdel(&str);
	while (wlst)
	{
		if (wlst->res == NULL)
			wlst->res = make_conv(wlst->conv, ap);
		wlst = wlst->next;
	}
	va_end(ap);
	ret = print_chain(tmp, fd);
	return (ret);
}
