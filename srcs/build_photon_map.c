/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_photon_map.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aplat <aplat@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/18 00:58:29 by aplat             #+#    #+#             */
/*   Updated: 2020/05/26 20:48:44 by aplat            ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

static void			photon_swap(t_photon *p1, t_photon *p2)
{
	t_photon	tmp;

	tmp = *p2;
	*p2 = *p1;
	*p1 = tmp;
}

static void			quick_sort_photon(t_photon *photon_tab, int lw, int hi,
	int ax)
{
	double		pivot;
	int			i;
	int			j;

	if (lw >= hi)
		return ;
	i = lw - 1;
	j = lw;
	pivot = photon_tab[hi].position.val[ax];
	while (j < hi)
	{
		if (photon_tab[j].position.val[ax] < pivot)
			photon_swap(&(photon_tab[++i]), &(photon_tab[j]));
		++j;
	}
	photon_swap(&(photon_tab[++i]), &(photon_tab[hi]));
	quick_sort_photon(photon_tab, lw, i - 1, ax);
	quick_sort_photon(photon_tab, i + 1, hi, ax);
}

static t_kd_tree	*alloc_photon(t_kd_tree *kd_tree, t_photon photon)
{
	kd_tree->photon->position = photon.position;
	kd_tree->photon->direction = photon.direction;
	kd_tree->photon->color = photon.color;
	return (kd_tree);
}

t_kd_tree			*build_tree(t_photon *phtn_tab, int lw, int hi, int ax)
{
	t_kd_tree	*tree;

	if (!(tree = ft_memalloc(sizeof(t_kd_tree))) ||
			!(tree->photon = ft_memalloc(sizeof(t_photon))))
		return (NULL);
	if (lw == hi)
		return (alloc_photon(tree, phtn_tab[lw]));
	else if (hi - lw == 1)
	{
		if (phtn_tab[lw].position.val[ax] < phtn_tab[hi].position.val[ax])
		{
			tree->right = build_tree(phtn_tab, hi, hi, (ax + 1) % 3);
			return (alloc_photon(tree, phtn_tab[lw]));
		}
		else
		{
			tree->left = build_tree(phtn_tab, lw, lw, (ax + 1) % 3);
			return (alloc_photon(tree, phtn_tab[hi]));
		}
	}
	quick_sort_photon(phtn_tab, lw, hi, ax);
	tree->left = build_tree(phtn_tab, lw, (hi - lw) / 2 + lw - 1, (ax + 1) % 3);
	tree->right = build_tree(phtn_tab, (hi - lw) / 2 + lw + 1, hi,
		(ax + 1) % 3);
	return (alloc_photon(tree, phtn_tab[(hi - lw) / 2 + lw]));
}
