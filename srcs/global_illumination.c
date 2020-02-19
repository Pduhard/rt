/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   global_illumination.c                            .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: pduhard- <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2020/02/13 16:21:04 by pduhard-     #+#   ##    ##    #+#       */
/*   Updated: 2020/02/19 19:53:39 by pduhard-         ###   ########lyon.fr   */
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

double		get_max_bbox(double a, double b, double c)
{
	if (a > b)
	{
		if (a > c)
			return a;
		return c;
	}
	else if (b > c)
		return b;
	return c;
}

double		get_dist_from_bbox(t_3vecf point, t_cube bbox)
{
	double	dx;
	double	dy;
	double	dz;

	dx = get_max_bbox(bbox.x_range.val[0] - point.val[0], 0, point.val[0] - bbox.x_range.val[1]);
	dy = get_max_bbox(bbox.y_range.val[0] - point.val[1], 0, point.val[1] - bbox.y_range.val[1]);
	dz = get_max_bbox(bbox.z_range.val[0] - point.val[2], 0, point.val[2] - bbox.z_range.val[1]);
	return (sqrt(dx * dx + dy * dy + dz * dz));
}

void	update_bbox(t_cube *bbox, int axis, double val, int min_max)// min == 0 max == 1
{
	if (axis == 0)
		bbox->x_range.val[min_max ? 1 : 0] = val;
	else if (axis == 1)
		bbox->y_range.val[min_max ? 1 : 0] = val;
	else if (axis == 2)
		bbox->z_range.val[min_max ? 1 : 0] = val;
}

void	get_nearest_neighbors(t_3vecf inter_point, t_kd_tree *kd_tree, t_photon *tab[NN_PHOTON_MAX], double closest, double farest, int axis, t_cube bbox)
{
	double	dist;
	t_cube	n_bbox;

//	printf("flute\n");
	if (!kd_tree)
		return ;
	dist = get_length_3vecf(sub_3vecf(inter_point, kd_tree->photon->position));
//	printf("a\n");
//	printf("closest %f farest %f\n", closest, farest);

//	if (dist < GL_RADIUS)
//	{	
		if (dist < closest)
			closest = dist;
		if (dist < farest)
			farest = add_photon_to_nn(tab, kd_tree->photon, inter_point, dist);
//	}
//	printf("b\n");
	n_bbox = bbox;
	if (inter_point.val[axis] < kd_tree->photon->position.val[axis])
	{
		update_bbox(&n_bbox, axis, kd_tree->photon->position.val[axis], 1);
		get_nearest_neighbors(inter_point, kd_tree->left, tab, closest, farest, (axis + 1) % 3, n_bbox);
		n_bbox = bbox;
		update_bbox(&n_bbox, axis, kd_tree->photon->position.val[axis], 0);
		if (farest >= get_dist_from_bbox(inter_point, n_bbox))
			get_nearest_neighbors(inter_point, kd_tree->right, tab, closest, farest, (axis + 1) % 3, n_bbox);
	}
	else
	{
		update_bbox(&n_bbox, axis, kd_tree->photon->position.val[axis], 0);
		get_nearest_neighbors(inter_point, kd_tree->right, tab, closest, farest, (axis + 1) % 3, n_bbox);
		n_bbox = bbox;
		update_bbox(&n_bbox, axis, kd_tree->photon->position.val[axis], 1);
		if (farest >= get_dist_from_bbox(inter_point, n_bbox))
			get_nearest_neighbors(inter_point, kd_tree->left, tab, closest, farest, (axis + 1) % 3, n_bbox);
	}
}

t_3vecf		compute_global_illumination(t_3vecf inter_point, t_3vecf normal_inter, t_data *data)
{
	t_photon	*nearest_n[NN_PHOTON_MAX];

//	return (assign_3vecf(0, 0,0));
	ft_bzero(nearest_n, sizeof(t_photon *) * NN_PHOTON_MAX);
//	printf("inter_point %f %f %f\n", inter_point.val[0], inter_point.val[1], inter_point.val[2]);
//printf("azd");
	get_nearest_neighbors(inter_point, data->photon_map, nearest_n, MAX_VIEW, MAX_VIEW, 0, data->bbox_photon);
	t_3vecf	light_fact;

	light_fact = assign_3vecf(0, 0, 0);

	//printf("POI");
//	printf("inter_point %f %f %f\n", inter_point.val[0], inter_point.val[1], inter_point.val[2]);
	int		activ_photon = NN_PHOTON_MAX;
	for (int i = 0; i < NN_PHOTON_MAX; i++)
	{
		if (nearest_n[i])
		{
			double	dp = dot_product_3vecf(normal_inter, nearest_n[i]->direction);
			double	l_len = get_length_3vecf(nearest_n[i]->direction);

			if (dp > 0)
			{
				light_fact.val[0] += nearest_n[i]->color.val[0] * dp / l_len;
				light_fact.val[1] += nearest_n[i]->color.val[1] * dp / l_len;
				light_fact.val[2] += nearest_n[i]->color.val[2] * dp / l_len;
			}
		}
			//printf("photon %d %f %f %f dist : %f\n", i, nearest_n[i]->position.val[0], nearest_n[i]->position.val[1], nearest_n[i]->position.val[2], get_length_3vecf(sub_3vecf(inter_point, nearest_n[i]->position)));
		else
			activ_photon--;
	}
	light_fact.val[0] /= (double)activ_photon;
	light_fact.val[1] /= (double)activ_photon;
	light_fact.val[2] /= (double)activ_photon;
	return (light_fact);
	(void)inter_point;
	(void)data;
	(void)normal_inter;
}
