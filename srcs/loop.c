/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   loop.c                                           .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: pduhard- <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2019/12/30 20:56:52 by pduhard-     #+#   ##    ##    #+#       */
/*   Updated: 2020/02/09 21:58:42 by pduhard-    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "rt.h"
/*
void	loop_manage_speed(t_data *data)
{
	if (data->params.p_speed)
		data->speed += 0.01;
	else if (data->params.m_speed)
		data->speed -= 0.01;
	if (data->speed < 0.01)
		data->speed = 0.01;
	if (data->params.p_cam_speed)
		data->cam_speed += 0.1;
	else if (data->params.m_cam_speed)
		data->cam_speed -= 0.1;
	if (data->cam_speed < 0.1)
		data->cam_speed = 0.1;
}

void	loop_manage_render(t_data *data)
{
	if (get_magnitude(sub_points(data->cam.from, set_new_point(0, 0, 0)))
		< (data->line_size + data->line_nbr) / 2)
		data->fill = 0;
	data->fun_factor += data->params.fun_on ? 0.1 : -data->fun_factor + 1.0;
}
*/

void	translate_and_rotate(t_data *data)
{
	t_obj *obj;

	obj = data->objs;
	data->rot_mat[1] = init_rotation_matrix_y(degree_to_radian(3));
	while (obj)
	{
		t_sphere *param = (t_sphere *)obj->obj_param;
		param->origin = mult_3vecf_33matf(param->origin, data->rot_mat[1]);
		obj = obj->next;
	}
}

void	loop_manage_cam(t_data *data)
{
//	if (data->hooks & F_KEY)
//	{
	if (data->hooks & (ARR_LEFT_KEY | ARR_RIGHT_KEY))
	{
		/*double	add_y = 4 * data->mouse_x;
		if (add_y > 1 || add_y < -1)
		{*/
		if (data->hooks & ARR_LEFT_KEY)
			data->camera->rotation.val[1] -= 3;
		if (data->hooks & ARR_RIGHT_KEY)
			data->camera->rotation.val[1] += 3;
		//4 * data->mouse_x;
			data->rot_mat[1] = init_rotation_matrix_y(degree_to_radian(data->camera->rotation.val[1]));
	}
	if (data->hooks & (ARR_UP_KEY | ARR_DOWN_KEY))
	{
		/*double	add_y = 4 * data->mouse_x;
		if (add_y > 1 || add_y < -1)
		{*/
		if (data->hooks & ARR_UP_KEY && data->camera->rotation.val[0] < 90)
			data->camera->rotation.val[0] += 3;
		if (data->hooks & ARR_DOWN_KEY && data->camera->rotation.val[0] > -90)
			data->camera->rotation.val[0] -= 3;
		//4 * data->mouse_x;
	//	data->rot_mat[0] = init_rotation_matrix_vec(mult_3vecf_33matf(assign_3vecf(1, 0, 0), data->rot_mat[1]), degree_to_radian(data->camera->rotation.val[0]));
	}
//	t_3vecf tm = mult_3vecf_33matf(assign_3vecf(1, 0, 0), data->rot_mat[1]);
//	printf("initial : 1, 0, 0 => after rot y: %f %f %f\n", tm.val[0], tm.val[1], tm.val[2]);
	/*	double	add_x = -4 * data->mouse_y;
		if (add_x > 1 || add_x < -1)
		{
			data->camera->rotation.val[0] += add_x;
		//4 * data->mouse_x;
			data->rot_mat[0] = init_rotation_matrix_x(degree_to_radian(data->camera->rotation.val[0]));
		}
*///	if (data->hooks & G_KEY)
//	{
		//data->camera->rotation.val[1] -= 2;
		//data->rot_mat[1] = init_rotation_matrix_y(degree_to_radian(data->camera->rotation.val[1]));
//	}
	if (data->hooks & (W_KEY | S_KEY))
	{
		t_3vecf dir = mult_3vecf_33matf(assign_3vecf(0, 0, 0.2), data->rot_mat[1]);
//		printf("%f %f %f\n", dir.val[0], dir.val[1], dir.val[2]);
		if (data->hooks & W_KEY)
		{
			data->camera->origin.val[0] += dir.val[0];
			data->camera->origin.val[1] += dir.val[1];
			data->camera->origin.val[2] += dir.val[2];
		}
		if (data->hooks & S_KEY)
		{
			data->camera->origin.val[0] -= dir.val[0];
			data->camera->origin.val[1] -= dir.val[1];
			data->camera->origin.val[2] -= dir.val[2];
		}

		//	data->camera->origin.val[2] -= 0.2;
	}
	if (data->hooks & (A_KEY | D_KEY))
	{
		t_3vecf dir = mult_3vecf_33matf(assign_3vecf(0.2, 0, 0), data->rot_mat[1]);
//		printf("%f %f %f\n", dir.val[0], dir.val[1], dir.val[2]);
		if (data->hooks & A_KEY)
		{
			data->camera->origin.val[0] -= dir.val[0];
			data->camera->origin.val[1] -= dir.val[1];
			data->camera->origin.val[2] -= dir.val[2];
		}

		//	data->camera->origin.val[0] -= 0.2;
		if (data->hooks & D_KEY)
		{
			data->camera->origin.val[0] += dir.val[0];
			data->camera->origin.val[1] += dir.val[1];
			data->camera->origin.val[2] += dir.val[2];
		}

		//	data->camera->origin.val[0] += 0.2;
	}
	//	t_3vecf dir = mult_3vecf_33matf(assign_3vecf(0.2, 0, 0), data->rot_mat[1]);
//		printf("%f %f %f\n", dir.val[0], dir.val[1], dir.val[2]);
		if (data->hooks & SPACE_KEY)
			data->camera->origin.val[1] -= 0.2;
		if (data->hooks & SHIFT_KEY)
			data->camera->origin.val[1] += 0.2;
/*		{
			data->camera->origin.val[0] -= dir.val[0];
			dir.val[1];
			data->camera->origin.val[2] -= dir.val[2];
		}
*/
		//	data->camera->origin.val[0] -= 0.2;
	/*	if (data->hooks & D_KEY)
		{
			data->camera->origin.val[0] += dir.val[0];
			data->camera->origin.val[1] += dir.val[1];
			data->camera->origin.val[2] += dir.val[2];
		}
*/
		//	data->camera->origin.val[0] += 0.2;
//		translate_and_rotate(data);
}
/*
void	loop_manage_rot_matrix(t_data *data)
{
	if (data->params.p_rotcam_z && !data->params.m_rotcam_z)
		data->mats.teta_z += data->speed;
	else if (data->params.m_rotcam_z && !data->params.p_rotcam_z)
		data->mats.teta_z -= data->speed;
	if (data->params.p_rotcam_x && !data->params.m_rotcam_x)
		data->mats.teta_x += data->speed;
	else if (data->params.m_rotcam_x && !data->params.p_rotcam_x)
		data->mats.teta_x -= data->speed;
	free_matrix(&data->mats.rot_x);
	free_matrix(&data->mats.rot_z);
	free_matrix(&data->mats.world_mat);
	free_matrix(&data->mats.proj_mat);
	data->mats.rot_x = get_rot_x(data->mats.teta_x);
	data->mats.rot_z = get_rot_z(data->mats.teta_z);
	data->mats.world_mat = get_world_matrix(data->cam.from, data);
	if (data->proj == PERSP)
		data->mats.proj_mat = get_projection_matrix(data->cam.fov,
			data->cam.near, data->cam.far);
	else if (data->proj == ORTO)
		data->mats.proj_mat = get_ortho_matrix(data);
}
*/
int		print_loop_image(void *param)
{
	t_data *data;
	data = (t_data *)param;
	Uint32	frame_start;

	frame_start = SDL_GetTicks();
	data->f += 0.01;
//	printf("%x\n", data->hooks);
//	loop_manage_speed(data);
//	loop_manage_render(data);
	loop_manage_cam(data);
//	loop_manage_rot_matrix(data);
//	clock_t	start;
//	clock_t	end;
	mlx_clear_window(data->mlx->mlx_ptr, data->mlx->win_ptr);
//	data->mlx->img_ptr = mlx_new_image(data->mlx->mlx_ptr, WIN_WIDTH, WIN_HEIGHT);
//	data->mlx->img_str = (int *)mlx_get_data_addr(data->mlx->img_ptr,
//			&(data->mlx->bpp), &(data->mlx->s_l), &(data->mlx->endian));
//	display_points(data->pix_map, data->map, data);
//	link_pixels(data->pix_map, data);
	//write(1, "lol\n", 4);
//	start = clock();
	render(data);
//	end = clock();
	mlx_put_image_to_window(data->mlx->mlx_ptr,
		data->mlx->win_ptr, data->mlx->img_ptr, 0, 0);
//	printf("%d %d, %d\n", frame_start, SDL_GetTicks(), (SDL_GetTicks() - frame_start));
	data->delta_time += (SDL_GetTicks() - frame_start);
	//printf("%lu \n", data->delta_time);
	data->fps++;
	if (data->delta_time >= 1000)
	{
		ft_printf("camera position : x %f y %f z %f\nfps: %d\n", data->camera->origin.val[0], data->camera->origin.val[1], data->camera->origin.val[2], data->fps);
		data->fps = 0;
		data->delta_time = 0;
	}
	return (1);
}
