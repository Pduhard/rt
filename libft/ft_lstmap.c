/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   ft_lstmap.c                                      .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: pduhard- <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/10/04 16:05:18 by pduhard-     #+#   ##    ##    #+#       */
/*   Updated: 2018/10/04 16:19:31 by pduhard-    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstmap(t_list *lst, t_list *(*f)(t_list *elem))
{
	t_list	*res_lst;
	t_list	*beg_lst;

	if (lst != NULL)
	{
		if (!(res_lst = malloc(sizeof(t_list))))
			return (NULL);
		res_lst = (*f)(lst);
		beg_lst = res_lst;
		lst = lst->next;
		while (lst->next != NULL)
		{
			if (!(res_lst->next = malloc(sizeof(t_list))))
				return (0);
			res_lst->next = (*f)(lst);
			res_lst = res_lst->next;
			lst = lst->next;
		}
		res_lst->next = (*f)(lst);
		return (beg_lst);
	}
	else
		return (NULL);
}
