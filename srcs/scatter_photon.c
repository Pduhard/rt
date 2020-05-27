/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scatter_photon.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aplat <aplat@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/18 05:46:33 by aplat             #+#    #+#             */
/*   Updated: 2020/05/18 05:47:12 by aplat            ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

static int		check_light_type(t_light *light)
{
	while (light)
	{
		if (light->light_type != LIGHT_AMBIENT)
			return (1);
		light = light->next;
	}
	return (0);
}

static int		check_end_of_scattering(int ind_i, int caus_i,
	t_photon **photon_tab)
{
	if ((ind_i == NB_INDIRECT_PHOTON && !caus_i)
		|| (caus_i == NB_CAUSTIC_PHOTON && ind_i == NB_INDIRECT_PHOTON))
	{
		if (!caus_i)
		{
			free(photon_tab[0]);
			photon_tab[0] = NULL;
		}
		return (1);
	}
	return (0);
}

static t_3vecf	get_photon_power(t_light *light)
{
	return (assign_3vecf(
		light->color.val[0] * DFLT_POWER,
		light->color.val[1] * DFLT_POWER,
		light->color.val[2] * DFLT_POWER));
}

void			scatter_photon(t_photon **photon_tab, t_data *data)
{
	t_light		*light;
	int			ind_i;
	int			caus_i;
	t_leq		l;
	int			rand_iter;

	if (!check_light_type(data->lights))
		return ;
	ind_i = 0;
	caus_i = 0;
	light = data->lights;
	rand_iter = (unsigned int)time(NULL);
	while (!check_end_of_scattering(ind_i, caus_i, photon_tab))
	{
		if (light->light_type != LIGHT_AMBIENT)
		{
			l = light->light_type == LIGHT_POINT ?
			(t_leq){light->param, get_random_dir(rand_iter)} :
			(t_leq){get_random_dir(rand_iter), light->param};
			// if (!(caus_i % 1000))
			// printf("cast rand %u indirect %d caus %d\n", rand_iter, ind_i, caus_i);
			cast_photon(l, (t_phtn_cast){data, &ind_i, &caus_i, photon_tab,
				PHOTON_DEPTH, rand_iter++, get_photon_power(light), 0});
		}
		light = light->next ? light->next : data->lights;
	}
}
