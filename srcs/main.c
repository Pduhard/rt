/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aplat <aplat@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/15 19:38:40 by aplat             #+#    #+#             */
/*   Updated: 2020/05/23 19:07:05 by aplat            ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

int				close_cross(t_data *data)
{
	free_all(data);
	exit(0);
	return (0);
}

static int		close_info(t_data *data)
{
	free_info(data);
	return (0);
}

static t_data	*init_all_scene(char **argv)
{
	t_data		*data;
	t_data		*first;
	t_data		*data_lst;
	int			i;

	i = 0;
	data_lst = NULL;
	first = NULL;
	while (argv[++i])
	{
		if ((data = init_data(argv[i], data_lst ? data_lst->mlx : NULL)))
		{
			if (!first && (data_lst = data))
				first = data;
			else if ((data_lst->next = data))
				data_lst = data_lst->next;
		}
	}
	if (!data_lst)
		return (NULL);
	data_lst->next = first;
	return (first);
}

int       check_quality(void)
{
	if (QUALITY != Q_VERY_LOW && QUALITY != Q_LOW &&
			QUALITY != Q_MED && QUALITY != Q_HIGH)
	{
		ft_fdprintf(2, "Macro error QUALITY: possible value:\n\t");
		ft_fdprintf(2, "Q_VERY_LOW\n\tQ_LOW\n\tQ_MED\n\tQ_HIGH\n");
		return (0);
	}
	return (1);
}

int				check_macro(void)
{
	if (NB_THREADS < 1 || NB_THREADS > 16)
	{
		error(ERRORTHREAD, NULL);
		return (0);
	}
	if (!check_quality())
		return (0);
	if (RAY_DEPTH < 1)
	{
		ft_fdprintf(2, "Macro error RAY_DEPTH: must be greater than 0\n");
		return (0);
	}
	if (BIAS < 0.000001 || BIAS > 1)
	{
		ft_fdprintf(2, "Macro error BIAS: must be greater than 0.000001 and lower than 1\n");
		return (0);
	}
	if (MAX_VIEW < 100 || MAX_VIEW > FLT_MAX)
	{
		ft_fdprintf(2, "Macro error MAX_VIEW: must be greater than 100 and lower than FLT_MAX\n");
		return (0);
	}
	if (TRANSP_F < 0 || TRANSP_F > 1)
	{
		ft_fdprintf(2, "Macro error TRANSP_F: must be greater than 0 and lower than 1\n");
		return (0);
	}
	if (DEFAULT_SHININESS < 0)
	{
		ft_fdprintf(2, "Macro error DEFAULT_SHININESS: must be greater than 0\n");
		return (0);
	}
	if (DFLT_POWER < 0)
	{
		ft_fdprintf(2, "Macro error DEFAULT_POWER: must be greater than 0\n");
		return (0);
	}
	if (NN_CAUSTIC_PHOTON_MAX < 1)
	{
		ft_fdprintf(2, "Macro error NN_CAUSTIC_PHOTON_MAX: must be greater than 0\n");
		return (0);
	}
	if (NN_INDIRECT_PHOTON_MAX < 1)
	{
		ft_fdprintf(2, "Macro error NN_INDIRECT_PHOTON_MAX: must be greater than 0\n");
		return (0);
	}
	if (NB_INDIRECT_PHOTON < NN_INDIRECT_PHOTON_MAX)
	{
		ft_fdprintf(2, "Macro error NB_INDIRECT_PHOTON: must be greater than NN_INDIRECT_PHOTON_MAX\n");
		return (0);
	}
	if (NB_CAUSTIC_PHOTON < NN_CAUSTIC_PHOTON_MAX)
	{
		ft_fdprintf(2, "Macro error NB_CAUSTIC_PHOTON: must be greater than NN_CAUSTIC_PHOTON_MAX\n");
		return (0);
	}
	if (PHOTON_DEPTH < 1)
	{
		ft_fdprintf(2, "Macro error PHOTON_DEPTH: must be greater than 0\n");
		return (0);
	}
	if (ROUGHCAST_LIMIT < 1)
	{
		ft_fdprintf(2, "Macro error ROUGHCAST_LIMIT: must be greater than 0\n");
		return (0);
	}
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

int				main(int argc, char **argv)
{
	t_data		*data;

	if (argc < 2 || !check_macro())
		return (error(ERRORARG, NULL));
	else if (!(data = init_all_scene(argv)))
		return (error(ERRORARG, NULL));
	mlx_hook(data->mlx->win_ptr, 2, (1L << 0), key_press, (void *)data);
	mlx_hook(data->mlx->win_ptr, 3, (1L << 1), key_release, (void *)data);
	mlx_hook(data->mlx->win_ptr, 4, (1L << 2), mouse_hook, (void *)data);
	mlx_hook(data->mlx->win_ptr, 17, (1L << 17), close_cross, (void *)data);
	mlx_expose_hook(data->info->win_ptr, open_info, (void *)data);
	if (data->info)
		mlx_hook(data->info->win_ptr, 17, (1L << 17), close_info, (void *)data);
	mlx_loop_hook(data->mlx->mlx_ptr, &print_loop_image, (void *)&data);
	mlx_loop(data->mlx->mlx_ptr);
	return (0);
}
