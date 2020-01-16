/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   ft_lstdel.c                                      .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: pduhard- <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/10/04 15:24:11 by pduhard-     #+#   ##    ##    #+#       */
/*   Updated: 2018/10/04 21:36:27 by pduhard-    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstdel(t_list **alst, void (*del)(void*, size_t))
{
	t_list	*tmp;
	t_list	*ntmp;

	tmp = *alst;
	if (tmp->next != NULL)
		ntmp = tmp->next;
	else
		ntmp = NULL;
	while (ntmp != NULL)
	{
		(*del)(tmp->content, tmp->content_size);
		tmp = ntmp;
		ntmp = tmp->next;
		free(tmp);
	}
	(*del)(tmp->content, tmp->content_size);
	ft_memdel((void*)alst);
}
