#include "rt.h"

void			manage_cam_rotation(t_data *data, int *ret)
{
	double radian;

	if (data->hooks & (ARR_LEFT_HOOK | ARR_RIGHT_HOOK))
	{
		if (data->hooks & ARR_LEFT_HOOK)
			data->camera->rotation.val[1] -= 3;
		if (data->hooks & ARR_RIGHT_HOOK)
			data->camera->rotation.val[1] += 3;
		radian = degree_to_radian(data->camera->rotation.val[1]);
		data->rot_mat[1] = init_rotation_matrix_y(radian);
		*ret = 1;
	}
	if (data->hooks & (ARR_UP_HOOK | ARR_DOWN_HOOK))
	{
		if (data->hooks & ARR_UP_HOOK && data->camera->rotation.val[0] < 90)
			data->camera->rotation.val[0] += 3;
		if (data->hooks & ARR_DOWN_HOOK && data->camera->rotation.val[0] > -90)
			data->camera->rotation.val[0] -= 3;
		*ret = 1;
	}
}

void			manage_cam_move(t_data *data, int *ret)
{
	t_3vecf dir;

	if ((data->hooks & (W_HOOK | S_HOOK)) && (*ret = 1))
	{
		dir = mult_3vecf_33matf(assign_3vecf(0, 0, 0.2), data->rot_mat[1]);
		if ((data->hooks & W_HOOK))
			data->camera->origin = add_3vecf(data->camera->origin, dir);
		if ((data->hooks & S_HOOK))
			data->camera->origin = sub_3vecf(data->camera->origin, dir);
	}
	if ((data->hooks & (A_HOOK | D_HOOK)) && (*ret = 1))
	{
		dir = mult_3vecf_33matf(assign_3vecf(0.2, 0, 0), data->rot_mat[1]);
		if (data->hooks & A_HOOK)
			data->camera->origin = sub_3vecf(data->camera->origin, dir);
		if (data->hooks & D_HOOK)
			data->camera->origin = add_3vecf(data->camera->origin, dir);
	}
	if ((data->hooks & SPACE_HOOK) && (*ret = 1))
		data->camera->origin.val[1] -= 0.2;
	if ((data->hooks & SHIFT_HOOK) && (*ret = 1))
		data->camera->origin.val[1] += 0.2;
}
