#include "rt.h"

t_3vecf init_ray_dir(int i, int j, t_anti_al a, t_data *data)
{
	t_3vecf dir;
	t_3vecf view;

	i = i * a.aa + a.offset / a.aa;
	j = j * a.aa + a.offset % a.aa;
	view = window_to_view(i, j, (int)data->size.val[0] * a.aa,
		(int)data->size.val[1] * a.aa);
	dir = mult_3vecf_33matf(mult_3vecf_33matf(view, data->rot_mat[1]),
		data->rot_mat[0]);
	normalize_3vecf(&dir);
	return (dir);
}

t_anti_al	init_anti_al(int aa, int offset, int anti_al_iter)
{
	t_anti_al	a;

	a.offset = offset;
	if (anti_al_iter < 1)
	{
		a.aa = 1;
		a.anti_al_iter = 1;
	}
	else
	{
		a.aa = aa;
		a.anti_al_iter = anti_al_iter;
	}
	return (a);
}

void	init_threads(t_thread threads[NB_THREADS], t_data *data)
{
	int		index = -1;

	while (++index < NB_THREADS)
	{
		threads[index].start = (int)-data->size.val[0] / 2
												 + (int)data->size.val[0] * index / NB_THREADS;
		threads[index].end = (int)-data->size.val[0] / 2
											 + (int)data->size.val[0] * (index + 1) / NB_THREADS;
		threads[index].data = data;
	}
}

void  init_frames_rot_mat(t_data *data)
{
	t_3vecf tm;
	double  rd;

	tm = mult_3vecf_33matf(assign_3vecf(1, 0, 0), data->rot_mat[1]);
	rd =degree_to_radian(data->camera->rotation.val[0]);
	data->rot_mat[0] = init_rotation_matrix_vec(tm, rd);
}
