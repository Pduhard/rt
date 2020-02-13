/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   global_illumination.c                            .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: pduhard- <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2020/02/13 16:21:04 by pduhard-     #+#   ##    ##    #+#       */
/*   Updated: 2020/02/13 20:50:31 by pduhard-    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "rt.h"

double	add_photon_to_nn(t_photon *tab[NN_PHOTON_MAX], t_photon *photon, t_3vecf inter_point, double dist)
{
	int		i;
//	double	dist;

	i = 0;
//	dist = get_length_3vecf(sub_3vecf(inter_point, kd_tree->photon->position));
	while (i < NN_PHOTON_MAX)
	{
		if (!tab[i])
		{
			tab[i] = photon;
			return (get_length_3vecf(sub_3vecf(tab[i]->position, inter_point)));
		}
		else if (dist < get_length_3vecf(sub_3vecf(inter_point, tab[i]->position)))
		{
			int		j = NN_PHOTON_MAX - 1;
			int		far = 0;

			while (j != i)
			{
				tab[j] = tab[j - 1];
				if (tab[j] && !far)
					far = j;
				j--;
			}
			tab[j] = photon;
			return (get_length_3vecf(sub_3vecf(tab[far]->position, inter_point)));
		}
		i++;
	}
//	printf("wefwef\n");
//	i = 0;
//	printf("%p %f\n", photon, get_length_3vecf(sub_3vecf(photon->position, inter_point)));
/*	for (; i < NN_PHOTON_MAX ; ++i)
	{
		printf("%p %f\n", tab[i], tab[i] ? get_length_3vecf(sub_3vecf(tab[i]->position, inter_point)) : -1);
	}
*/
	return (get_length_3vecf(sub_3vecf(tab[NN_PHOTON_MAX - 1]->position, inter_point)));
}

void	get_nearest_neighbors(t_3vecf inter_point, t_kd_tree *kd_tree, t_photon *tab[NN_PHOTON_MAX], double closest, double farest, int axis)
{
	double	dist;

//	printf("flute\n");
	if (!kd_tree)
		return ;
	dist = get_length_3vecf(sub_3vecf(inter_point, kd_tree->photon->position));
//	printf("a\n");
//	printf("closest %f farest %f\n", closest, farest);
	if (dist < closest)
		closest = dist;
	if (dist < farest)
		farest = add_photon_to_nn(tab, kd_tree->photon, inter_point, dist);
//	printf("b\n");
	
	if (inter_point.val[axis] < kd_tree->photon->position.val[axis])
//	if (kd_tree->left)
		get_nearest_neighbors(inter_point, kd_tree->left, tab, closest, farest, (axis + 1) % 3);
	else
//	if (kd_tree->right)
		get_nearest_neighbors(inter_point, kd_tree->right, tab, closest, farest, (axis + 1) % 3);




}

t_3vecf		compute_global_illumination(t_3vecf inter_point, t_3vecf normal_inter, t_data *data)
{
	t_photon	*nearest_n[NN_PHOTON_MAX];

	return (assign_3vecf(0, 0,0));
	ft_bzero(nearest_n, sizeof(t_photon *) * NN_PHOTON_MAX);
//	printf("inter_point %f %f %f\n", inter_point.val[0], inter_point.val[1], inter_point.val[2]);
	get_nearest_neighbors(inter_point, data->photon_map, nearest_n, MAX_VIEW, MAX_VIEW, 0);
//	printf("inter_point %f %f %f\n", inter_point.val[0], inter_point.val[1], inter_point.val[2]);
/*	for (int i = 0; i < NN_PHOTON_MAX; i++)
	{
		if (nearest_n[i])
			;//printf("photon %f %f %f dist : %f\n", nearest_n[i]->position.val[0], nearest_n[i]->position.val[1], nearest_n[i]->position.val[2], get_length_3vecf(sub_3vecf(inter_point, nearest_n[i]->position)));
		else
		;//	printf("nearest_ergerg\n");
	}
*/	

	return (assign_3vecf(0, 0, 0));
	(void)inter_point;
	(void)data;
	(void)normal_inter;
}
