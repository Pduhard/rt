/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   macro.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aplat <aplat@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/25 13:09:18 by aplat             #+#    #+#             */
/*   Updated: 2020/05/25 14:28:18 by aplat            ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

static int	check_macro_others(void)
{
	if (TRANSP_F < 0 || TRANSP_F > 1)
	{
		ft_fdprintf(2, "Macro error TRANSP_F:\
		must be greater than 0 and lower than 1\n");
		return (0);
	}
	if (DEFAULT_SHININESS < 0)
	{
		ft_fdprintf(2, "Macro error DEFAULT_SHININESS:\
		must be greater than 0\n");
		return (0);
	}
	if (DFLT_POWER < 0)
	{
		ft_fdprintf(2, "Macro error DEFAULT_POWER: must be greater than 0\n");
		return (0);
	}
	if (PHOTON_DEPTH < 1)
	{
		ft_fdprintf(2, "Macro error PHOTON_DEPTH: must be greater than 0\n");
		return (0);
	}
	return (1);
}

static int	check_macro_ray(void)
{
	if (RAY_DEPTH < 1)
	{
		ft_fdprintf(2, "Macro error RAY_DEPTH: must be greater than 0\n");
		return (0);
	}
	if (BIAS < 0.000001 || BIAS > 1)
	{
		ft_fdprintf(2, "Macro error BIAS:\
		must be greater than 0.000001 and lower than 1\n");
		return (0);
	}
	if (MAX_VIEW < 100 || MAX_VIEW > FLT_MAX)
	{
		ft_fdprintf(2, "Macro error MAX_VIEW:\
		must be greater than 100 and lower than FLT_MAX\n");
		return (0);
	}
	return (1);
}

static int	check_macro_motion(void)
{
	if (is_null(MOTION_STEP))
	{
		ft_fdprintf(2, "Macro error MOTION_STEP: can't be null\n");
		return (0);
	}
	if (MOTION_FVEL < 1)
	{
		ft_fdprintf(2, "Macro error MOTION_FVEL: must be greater than 0\n");
		return (0);
	}
	if (MOTION_SPP < 1)
	{
		ft_fdprintf(2, "Macro error MOTION_SPP: must be greater than 0\n");
		return (0);
	}
	return (1);
}

static int	check_macro_photon(void)
{
	if (NN_CAUSTIC_PHOTON_MAX < 1)
	{
		ft_fdprintf(2, "Macro error NN_CAUSTIC_PHOTON_MAX: %s",
		"must be greater than 0\n");
		return (0);
	}
	if (NN_INDIRECT_PHOTON_MAX < 1)
	{
		ft_fdprintf(2, "Macro error NN_INDIRECT_PHOTON_MAX: %s",
		"must be greater than 0\n");
		return (0);
	}
	if (NB_INDIRECT_PHOTON < NN_INDIRECT_PHOTON_MAX)
	{
		ft_fdprintf(2, "Macro error NB_INDIRECT_PHOTON: %s",
		"must be greater than NN_INDIRECT_PHOTON_MAX\n");
		return (0);
	}
	if (NB_CAUSTIC_PHOTON < NN_CAUSTIC_PHOTON_MAX)
	{
		ft_fdprintf(2, "Macro error NB_CAUSTIC_PHOTON: %s"
		"must be greater than NN_CAUSTIC_PHOTON_MAX\n");
		return (0);
	}
	return (1);
}

int			check_macro(void)
{
	if (NB_THREADS < 1 || NB_THREADS > 16)
	{
		error(ERRORTHREAD, NULL);
		return (0);
	}
	if (!check_quality())
		return (0);
	if (!check_macro_ray())
		return (0);
	if (!check_macro_others())
		return (0);
	if (!check_macro_photon())
		return (0);
	if (ROUGHCAST_LIMIT < 1)
	{
		ft_fdprintf(2, "Macro error ROUGHCAST_LIMIT: %s",
		"must be greater than 0\n");
		return (0);
	}
	if (!check_macro_motion())
		return (0);
	return (1);
}
