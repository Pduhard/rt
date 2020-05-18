/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aplat <aplat@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/18 05:28:57 by aplat             #+#    #+#             */
/*   Updated: 2020/05/18 05:29:49 by aplat            ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

static int	get_aa_step(double aa_adapt)
{
	if (aa_adapt == MIN_AA)
		return (2);
	return (1);
}

void		*render_thread(void *param)
{
	t_leq		l;
	int			i;
	int			j;
	t_data		*data;
	t_thread	*thread;

	thread = (t_thread *)param;
	data = thread->data;
	i = thread->start;
	l.orig = data->camera->origin;
	while (i < thread->end)
	{
		j = -data->size.val[1] / 2;
		while (j < data->size.val[1] / 2)
		{
			if (data->stereoscopy)
				compute_stereoscopy(data, l, i, j);
			else
				compute_classic(data, l, i, j);
			j += get_aa_step(data->aa_adapt);
		}
		i += get_aa_step(data->aa_adapt);
	}
	pthread_exit(NULL);
	return (NULL);
}

void		render(t_data *data)
{
	int			ret;
	int			i;
	t_thread	threads_param[NB_THREADS];
	pthread_t	threads[NB_THREADS];

	init_threads(threads_param, data);
	init_frames_rot_mat(data);
	i = 0;
	while (i < NB_THREADS)
	{
		ret = pthread_create(&(threads[i]), NULL, render_thread,
			(void *)&(threads_param[i]));
		if (ret)
			exit(EXIT_FAILURE);
		++i;
	}
	i = 0;
	while (i < NB_THREADS)
		pthread_join(threads[i++], NULL);
}
