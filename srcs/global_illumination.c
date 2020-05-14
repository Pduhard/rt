/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   global_illumination.c                            .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: pduhard- <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2020/02/13 16:21:04 by pduhard-     #+#   ##    ##    #+#       */
/*   Updated: 2020/02/26 06:26:47 by pduhard-         ###   ########lyon.fr   */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "rt.h"

int	get_insertion_index(t_photon **tab, int i, int j, int *far)
{
	if (j == i)
		*far = j;
	while (j != i)
	{
		tab[j] = tab[j - 1];
		if (tab[j] && !*far)
			*far = j;
		j--;
	}
	return (j);
}

double	add_photon_to_nn(t_photon **tab, t_photon *photon, t_3vecf inter_point,
		t_add_pht_p param)
{
	int		i;
	int		j;
 	int		far;

 	far = 0;
	i = -1;
	j = param.nn_photon - 1;
	while (++i < param.nn_photon)
	{
		if (!tab[i] && (tab[i] = photon))
			return (get_length_3vecf(sub_3vecf(tab[i]->position, inter_point)));
		if (param.dist < get_length_3vecf(sub_3vecf(inter_point, tab[i]->position)))
		{
			tab[get_insertion_index(tab, i, j, &far)] = photon;
			return (get_length_3vecf(sub_3vecf(tab[far]->position, inter_point)));
		}
	}
	return (get_length_3vecf(sub_3vecf(tab[param.nn_photon - 1]->position,
			inter_point)));
}

void  add_if_closer(double dist, t_nn_param p, t_kd_tree *kd_tree)
{
	// printf("farest %f dist %f\n", *p.farest, dist);
	if (dist < *(p.closest))
		*(p.closest) = dist;
	if (dist < *(p.farest) || (p.actual_nn < p.nn_photon && (++p.actual_nn)))
		*(p.farest) = add_photon_to_nn(p.tab, kd_tree->photon,
				p.inter_point, (t_add_pht_p){dist, p.nn_photon});
}

void  get_nearest_neighbors(t_nn_param p, t_kd_tree *kd_tree, int axis)
{
	double	dist;
	t_3vecf	photon_pos;

	if (!kd_tree)
		return ;
	photon_pos = kd_tree->photon->position;
	dist = get_length_3vecf(sub_3vecf(p.inter_point, photon_pos));
	add_if_closer(dist, p, kd_tree);
	if (p.inter_point.val[axis] < photon_pos.val[axis])
	{
		get_nearest_neighbors(p, kd_tree->left, (axis + 1) % 3);
		if (*(p.farest) >= fabs(photon_pos.val[axis] - p.inter_point.val[axis]))
			get_nearest_neighbors(p, kd_tree->right, (axis + 1) % 3);
	}
	else
	{
		get_nearest_neighbors(p, kd_tree->right, (axis + 1) % 3);
		if (*(p.farest) >= fabs(photon_pos.val[axis] - p.inter_point.val[axis]))
			get_nearest_neighbors(p, kd_tree->left, (axis + 1) % 3);
	}
}

t_3vecf		compute_radiance_estimation(t_photon **nearest_n, t_3vecf inter_point,
	double farest, int nn_photon)
{
	t_3vecf	radiance;
	int			i;
	double	k;
	double	dist;
	double	cone_f;

 	k = 1.05;
	i = -1;
	radiance = assign_3vecf(0, 0, 0);
	while (++i < nn_photon)
		if (nearest_n[i])
		{
			dist = get_length_3vecf(sub_3vecf(inter_point, nearest_n[i]->position));
			cone_f = 1 - dist / (k * farest);
			radiance = add_3vecf(radiance, product_c3vecf(nearest_n[i]->color, cone_f));
		}
	radiance.val[0] /= M_PI * farest * farest * (1. - 2. / (3. * k));
	radiance.val[1] /= M_PI * farest * farest * (1. - 2. / (3. * k));
	radiance.val[2] /= M_PI * farest * farest * (1. - 2. / (3. * k));

	// clamp_val(&(radiance.val[0]), 0, 1);
	// clamp_val(&(radiance.val[1]), 0, 1);
	// clamp_val(&(radiance.val[2]), 0, 1);
	return (radiance);
}

t_3vecf		compute_global_illumination(t_3vecf inter_point, t_3vecf normal_inter,
	t_kd_tree *photon_map, int nn_photon)
{
	t_photon	**nearest_n;
	double		closest;
	double		farest;
	t_3vecf		radiance;

 	farest = MAX_VIEW;
	closest = MAX_VIEW;
	if (!photon_map || !(nearest_n = malloc(sizeof(t_photon *) * nn_photon)))
		return (assign_3vecf(0, 0, 0));
	ft_bzero(nearest_n, sizeof(t_photon *) * nn_photon);
	// printf("START\n\n");
	get_nearest_neighbors((t_nn_param){inter_point, normal_inter, nearest_n,
			&closest, &farest, nn_photon, 0}, photon_map, 0);
			// printf("\n\END\n\n");
	radiance = compute_radiance_estimation(nearest_n, inter_point, farest, nn_photon);
	free(nearest_n);
	return (radiance);
}
