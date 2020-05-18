/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aplat <aplat@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/15 18:20:53 by aplat             #+#    #+#             */
/*   Updated: 2020/05/18 16:04:41 by aplat            ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

static void	unchain_data_list(t_data *data)
{
	t_data *start;

	start = data;
	while (data && data->next != start)
		data = data->next;
	if (data)
		data->next = NULL;
}

static void	free_mlx(t_mlx *mlx)
{
	if (!mlx)
		return ;
	if (mlx->img_ptr)
		mlx_destroy_image(mlx->mlx_ptr, mlx->img_ptr);
	if (mlx->win_ptr)
		mlx_destroy_window(mlx->mlx_ptr, mlx->win_ptr);
	free(mlx);
}

void		free_info(t_data *data)
{
	t_data	*first;
	int		check;
	t_mlx	*info;

	first = data;
	check = 0;
	info = data->info;
	while (data && (data != first || !check))
	{
		check = 1;
		data->info = NULL;
		data = data->next;
	}
	if (info)
		free(info);
}

void		free_all(t_data *data)
{
	t_data	*next;
	t_mlx	*mlx;

	if (!data)
		return ;
	mlx = data->mlx;
	free_info(data);
	free_mlx(mlx);
	unchain_data_list(data);
	while (data)
	{
		next = data->next;
		free_data(data);
		data = next;
	}
}
