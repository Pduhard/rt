/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   photon_map.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aplat <aplat@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/18 05:10:00 by aplat             #+#    #+#             */
/*   Updated: 2020/05/20 09:06:33 by aplat            ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

int		create_photon_map(t_data *data)
{
	t_photon	**photon_tab;

	if (!data->objs)
		return (0);
	if (!(photon_tab = malloc(sizeof(t_photon *) * 2)) ||
			!(photon_tab[0] = malloc(sizeof(t_photon) * NB_CAUSTIC_PHOTON)) ||
			!(photon_tab[1] = malloc(sizeof(t_photon) * NB_INDIRECT_PHOTON)))
		return (0);
	scatter_photon(photon_tab, data);
	if (photon_tab[0])
		data->caustic_map = build_tree(photon_tab[0], 0,
			NB_CAUSTIC_PHOTON - 1, 0);
	data->indirect_map = build_tree(photon_tab[1], 0,
		NB_INDIRECT_PHOTON - 1, 0);
	free(photon_tab[0]);
	free(photon_tab[1]);
	free(photon_tab);
	return (data->caustic_map || data->indirect_map ? 1 : 0);
}

void	check_photon_map(t_data *data)
{
	if ((((data->caustics_gi && !data->caustic_map)
		|| (data->indirect_gi && !data->indirect_map)))
			&& data->first_loop > 1 && !(create_photon_map(data)))
	{
		ft_fdprintf(2, "Internal error: Global Illumination\n");
		free_all(data);
		exit(0);
	}
}
