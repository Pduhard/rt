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
	return (get_length_3vecf(sub_3vecf(tab[param.nn_photon - 1]->position, inter_point)));
}

void  get_nearest_neighbors(t_nn_param p, t_kd_tree *kd_tree, int axis)
{
	double	dist;

	if (!kd_tree)
		return ;
	dist = get_length_3vecf(sub_3vecf(p.inter_point, kd_tree->photon->position));
	if (dot_product_3vecf(p.normal_inter, kd_tree->photon->direction) > 0)
	{
		if (dist < *(p.closest))
			*(p.closest) = dist;
		if (dist < *(p.farest))
			*(p.farest) = add_photon_to_nn(p.tab, kd_tree->photon,
					p.inter_point, (t_add_pht_p){dist, p.nn_photon});
	}
	if (p.inter_point.val[axis] < kd_tree->photon->position.val[axis])
	{
		get_nearest_neighbors(p, kd_tree->left, (axis + 1) % 3);
		if (*(p.farest) >= fabs(kd_tree->photon->position.val[axis] - p.inter_point.val[axis]))
			get_nearest_neighbors(p, kd_tree->right, (axis + 1) % 3);
	}
	else
	{
		get_nearest_neighbors(p, kd_tree->right, (axis + 1) % 3);
		if (*(p.farest) >= fabs(kd_tree->photon->position.val[axis] - p.inter_point.val[axis]))
			get_nearest_neighbors(p, kd_tree->left, (axis + 1) % 3);
	}
}

t_3vecf		compute_global_illumination(t_3vecf inter_point, t_3vecf normal_inter, t_kd_tree *photon_map, int nn_photon)
{
	t_photon	**nearest_n;

	if (!photon_map || !(nearest_n = malloc(sizeof(t_photon *) * nn_photon)))
		return (assign_3vecf(0, 0, 0));
//	return (assign_3vecf(0, 0,0));
	ft_bzero(nearest_n, sizeof(t_photon *) * nn_photon);
//	printf("inter_point %f %f %f\n", inter_point.val[0], inter_point.val[1], inter_point.val[2]);
//printf("azd");
	double	closest = MAX_VIEW;
	double	farest = MAX_VIEW;
//	printf("Saliutttt\n");
// get_nearest_neighbors(inter_point, normal_inter, photon_map, nearest_n, &closest, &farest, 0, nn_photon);
get_nearest_neighbors((t_nn_param){inter_point, normal_inter, nearest_n, &closest, &farest, nn_photon}, photon_map, 0);
	t_3vecf	light_fact;
	light_fact = assign_3vecf(0, 0, 0);

	// (void)max_radius;
//	if (farest > max_radius)
//		return (assign_3vecf(0, 0, 0));
	//printf("POI");
//	printf("inter_point %f %f %f\n", inter_point.val[0], inter_point.val[1], inter_point.val[2]);
//	double	activ_photon = NN_PHOTON_MAX;
/*	double	f = 0;
	for (int i = 0; i < NN_PHOTON_MAX; i++)
	{
		if (nearest_n[i])
		{
			double	dist = get_length_3vecf(sub_3vecf(inter_point, nearest_n[i]->position));
			if (dist > f)
				f = dist;
		}
	}
*/
	double f = farest;
	double	k = 1.05;
	for (int i = 0; i < nn_photon; i++)
	{
		if (nearest_n[i])
		{
			double	dist = get_length_3vecf(sub_3vecf(inter_point, nearest_n[i]->position));
		//	printf("photon %d %f %f %f dist : %f\n", i, nearest_n[i]->position.val[0], nearest_n[i]->position.val[1], nearest_n[i]->position.val[2], get_length_3vecf(sub_3vecf(inter_point, nearest_n[i]->position)));
		//	normalize_3vecf(&(nearest_n[i]->direction));
		//	double	dp = dot_product_3vecf(normal_inter, nearest_n[i]->direction);
			//printf("dp : %f, %f %f %f\n", dp, nearest_n[i]->color.val[0], nearest_n[i]->color.val[1], nearest_n[i]->color.val[2]);
			//double	l_len = get_length_3vecf(nearest_n[i]->direction);

		//	if (dp > 0)
		//	{
				double	cone_f;
				cone_f = 1 - dist / (k * f);
	//			printf("conef %f farest %f closest %f\n", cone_f, f, closest);
	//			if (cone_f > 0)
	//			{
					light_fact.val[0] += nearest_n[i]->color.val[0] * cone_f;
					light_fact.val[1] += nearest_n[i]->color.val[1] * cone_f;
					light_fact.val[2] += nearest_n[i]->color.val[2] * cone_f;
	//			}
				/* circle filter ...
				light_fact.val[0] += ((nearest_n[i]->color.val[0])) / (M_PI * f * f);// / (M_PI * farest * farest);
				light_fact.val[1] += ((nearest_n[i]->color.val[1])) / (M_PI * f * f);// / (M_PI * farest * farest);
				light_fact.val[2] += ((nearest_n[i]->color.val[2])) / (M_PI * f * f);// / (M_PI * farest * farest);
				*/
		//	}
			//printf("salut %d\n", i);
		}
	//	printf("lfact : %f %f %f\n", light_fact.val[0], light_fact.val[1], light_fact.val[2]);
	}
	free(nearest_n);
	if (light_fact.val[0] < 0 || light_fact.val[1] < 0 || light_fact.val[2] < 0)
		return (assign_3vecf(0, 0, 0));
	light_fact.val[0] /= M_PI * f * f * (1. - 2. / (3. * k));
	light_fact.val[1] /= M_PI * f * f * (1. - 2. / (3. * k));
	light_fact.val[2] /= M_PI * f * f * (1. - 2. / (3. * k));
//	printf("lfact : %f %f %f\n", light_fact.val[0], light_fact.val[1], light_fact.val[2]);
//	printf("enddddddddd\n");
	//activ_photon = M_PI * farest * farest;

//	light_fact.val[0] /= (double)activ_photon;
//	light_fact.val[1] /= (double)activ_photon;
//	light_fact.val[2] /= (double)activ_photon;
//	light_fact.val[0] /= M_PI * f * f;
//	light_fact.val[1] /= M_PI * f * f;
//	light_fact.val[2] /= M_PI * f * f;

//	printf("%f %f %f\n", light_fact.val[0], light_fact.val[1], light_fact.val[2]);
//	light_fact.val[0] /= (double)activ_photon;
//	light_fact.val[1] /= (double)activ_photon;
//	light_fact.val[2] /= (double)activ_photon;
//	light_fact.val[0] /= NN_PHOTON_MAX / 0.02;
//	light_fact.val[1] /= NN_PHOTON_MAX / 0.02;
//	light_fact.val[2] /= NN_PHOTON_MAX / 0.02;

//	if (get_length_3vecf(light_fact) < 0.4)
//		return (assign_3vecf(0, 0, 0));
	return (light_fact);
	(void)inter_point;
	(void)normal_inter;
}
