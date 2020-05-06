#include "rt.h"
/*
   int		key_press2(int keycode, t_data *data)
   {
   if (keycode == 125)
   data->params.m_rotcam_y = 1;
   else if (keycode == 126)
   data->params.p_rotcam_y = 1;
   else if (keycode == 257)
   data->params.m_rotcam_x = 1;
   else if (keycode == 82)
   data->params.p_rotcam_x = 1;
   else if (keycode == 35)
   data->proj = data->proj == PERSP ? ORTO : PERSP;
   else if (keycode == 12)
   data->params.ym_from = 1;
   else if (keycode == 14)
   data->params.yp_from = 1;
   else if (keycode == 3)
   data->params.fun_on = data->params.fun_on ? 0 : 1;
   else if (keycode == 116)
   data->params.p_cam_speed = 1;
   else if (keycode == 121)
   data->params.m_cam_speed = 1;
   else if (keycode == 53)
   return (free_all(1, data));
   return (1);
   }
   */

char	*get_image_file_name(char *scene_name)
{
	char	*image_name;
	char	*tmp1;
	char	*tmp2;
	int		fd;
	int		nb_copy;

	nb_copy = 1;

	tmp1 = ft_strjoin(scene_name, ".png");
	image_name = ft_strjoin("image_save/", tmp1);
	free(tmp1);
	while ((fd = open(image_name, O_RDWR)) != -1)
	{
		close(fd);
		free(image_name);
		tmp1 = ft_itoa(nb_copy++);
		tmp2 = ft_strjoin(" (", tmp1);
		free(tmp1);
		tmp1 = ft_strjoin(tmp2, ").png");
		free(tmp2);
		tmp2 = ft_strjoin(scene_name, tmp1);
		free(tmp1);
		image_name = ft_strjoin("image_save/", tmp2);
		free(tmp2);
	}
	close(fd);
	return (image_name);
}

int		key_press(int keycode, void *param)
{
	t_data *data;

	data = (t_data *)param;
	printf("button %d\n", keycode);
	if (keycode == A_KEY) //
		data->hooks |= A_HOOK;
	//data->cam.origin[0] -= 0.2;
	else if (keycode == ESC_KEY)
	{
		free_all(data);
		exit(0);
	}
	else if (keycode == S_KEY) //s
		data->hooks |= S_HOOK;
	//data->cam.origin[2] -= 0.2;
	else if (keycode == D_KEY) //d
		data->hooks |= D_HOOK;
	//data->cam.origin[0] += 0.2;
	else if (keycode == W_KEY) //w
		data->hooks |= W_HOOK;
	else if (keycode == F_KEY) //w
		data->hooks |= F_HOOK;
	else if (keycode == G_KEY) //w
		data->hooks |= G_HOOK;
	else if (keycode == ARR_LEFT_KEY) //left
		data->hooks |= ARR_LEFT_HOOK;
	else if (keycode == ARR_RIGHT_KEY) // right
		data->hooks |= ARR_RIGHT_HOOK;
	else if (keycode == ARR_DOWN_KEY) // down
		data->hooks |= ARR_DOWN_HOOK;
	else if (keycode == ARR_UP_KEY) // up
		data->hooks |= ARR_UP_HOOK;
	else if (keycode == SPACE_KEY)
		data->hooks |= SPACE_HOOK;
	else if (keycode == SHIFT_KEY)
		data->hooks |= SHIFT_HOOK;
	else if (keycode == TAB_KEY)
		data->to_next = 1;
	else if (keycode == ALPHA_ONE_KEY)
		generate_new_sphere(data);
	else if (keycode == ALPHA_TWO_KEY)
		generate_new_plane(data);
	else if (keycode == ALPHA_THREE_KEY)
		generate_new_cone(data);
	else if (keycode == ALPHA_FOUR_KEY)
		generate_new_cylinder(data);
	else if (keycode == ALPHA_FIVE_KEY)
		generate_new_triangle(data);
	else if (keycode == ALPHA_SIX_KEY)
		generate_new_ellipsoid(data);
	else if (keycode == ALPHA_SEVEN_KEY)
		generate_new_hyperboloid(data);
	else if (keycode == ALPHA_EIGHT_KEY)
		generate_new_horse_saddle(data);
	else if (keycode == ALPHA_NINE_KEY)
		generate_new_monkey_saddle(data);
	else if (keycode == ALPHA_ZERO_KEY)
		generate_new_cyclide(data);
	else if (keycode == ALPHA_MINUS_KEY)
		generate_new_fermat(data);
	else if (keycode == ALPHA_PLUS_KEY)
		generate_new_moebius(data);
//	else if (keycode == 19)
//		generate_new_plane(data);
	else if ((keycode == SUPPR_KEY || keycode == DELETE_KEY) && data->selected_obj && data->selected_obj->obj_type != OBJ_SKYBOX)
	{
		delete_object(data, data->selected_obj);
		data->selected_obj = NULL;
		data->new_obj = 1;
	}
	else if (keycode == IMPR_KEY)
	{
	/*	SDL_Surface	*img_save = SDL_CreateRGBSurfaceFrom((void *)data->mlx->img_str, (int)data->size.val[0], (int)data->size.val[1], 32, 4 * (int)data->size.val[0], 0xff << 16, 0xff << 8, 0xff << 0, 0x0);
		char	*image_file_name;

		image_file_name = get_image_file_name(data->scene_name);
*/	/*	image_file_name = ft_strjoin(tmp, ".png");
		free(tmp);
		while ((img_fd  = open(image_file_name, O_RDWR)) != -1)
		{
			image_file_name = ft_strfjoin(image_file_)
		}
	*/ /* IMG_SavePNG(img_save, image_file_name);
		free(image_file_name);
		SDL_FreeSurface(img_save);
	*/
	}
	//	else if (keycode == 69 && data->anti_al < MAX_ANTI_AL)
	//		data->anti_al++;
	;/*		data->params.xp_from = 1;
			else if (keycode == 0)
			data->params.xm_from = 1;
			else if (keycode == 1)
			data->params.zp_from = 1;
			else if (keycode == 13)
			data->params.zm_from = 1;
			else if (keycode == 124)
			data->params.p_rotcam_z = 1;
			else if (keycode == 123)
			data->params.m_rotcam_z = 1;
			else if (keycode == 49 && data->line_nbr * data->line_size < 25000)
			data->fill = data->fill ? 0 : 1;
			else if (keycode == 69)
			data->params.p_speed = 1;
			else if (keycode == 78)
			data->params.m_speed = 1;
			else
			return (key_press2(keycode, data));
			*/
	return (1);
}
/*
   int		key_release2(int keycode, t_data *data)
   {
   if (keycode == 126)
   data->params.p_rotcam_y = 0;
   else if (keycode == 257)
   data->params.m_rotcam_x = 0;
   else if (keycode == 82)
   data->params.p_rotcam_x = 0;
   else if (keycode == 12)
   data->params.ym_from = 0;
   else if (keycode == 14)
   data->params.yp_from = 0;
   else if (keycode == 116)
   data->params.p_cam_speed = 0;
   else if (keycode == 121)
   data->params.m_cam_speed = 0;
   return (1);
   }
   */
int		key_release(int keycode, void *param)
{
	t_data *data;

	data = (t_data *)param;
	if (keycode == A_KEY && (data->hooks & A_HOOK)) //
		data->hooks -= A_HOOK;
	//data->cam.origin[0] -= 0.2;
	else if (keycode == S_KEY && (data->hooks & S_HOOK)) //s
		data->hooks -= S_HOOK;
	//data->cam.origin[2] -= 0.2;
	else if (keycode == D_KEY && (data->hooks & D_HOOK)) //d
		data->hooks -= D_HOOK;
	//data->cam.origin[0] += 0.2;
	else if (keycode == W_KEY && (data->hooks & W_HOOK)) //w
		data->hooks -= W_HOOK;
	else if (keycode == F_KEY && (data->hooks & F_HOOK)) //w
		data->hooks -= F_HOOK;
	else if (keycode == G_KEY && (data->hooks & G_HOOK)) //w
		data->hooks -= G_HOOK;
	else if (keycode == ARR_LEFT_KEY && (data->hooks & ARR_LEFT_HOOK)) //left
		data->hooks -= ARR_LEFT_HOOK;
	else if (keycode == ARR_RIGHT_KEY && (data->hooks & ARR_RIGHT_HOOK)) // right
		data->hooks -= ARR_RIGHT_HOOK;
	else if (keycode == ARR_DOWN_KEY && (data->hooks & ARR_DOWN_HOOK)) // down
		data->hooks -= ARR_DOWN_HOOK;
	else if (keycode == ARR_UP_KEY && (data->hooks & ARR_UP_HOOK)) // up
		data->hooks -= ARR_UP_HOOK;
	else if (keycode == SPACE_KEY  && (data->hooks & SPACE_HOOK))
		data->hooks -= SPACE_HOOK;
	else if (keycode == SHIFT_KEY && (data->hooks & SHIFT_HOOK))
		data->hooks -= SHIFT_HOOK;

//	if (keycode == 2)
		/*	data->params.xp_from = 0;
			else if (keycode == 0)
			data->params.xm_from = 0;
			else if (keycode == 1)
			data->params.zp_from = 0;
			else if (keycode == 13)
			data->params.zm_from = 0;
			else if (keycode == 124)
			data->params.p_rotcam_z = 0;
			else if (keycode == 123)
			data->params.m_rotcam_z = 0;
			else if (keycode == 69)
			data->params.p_speed = 0;
			else if (keycode == 78)
			data->params.m_speed = 0;
			else if (keycode == 125
			data->params.m_rotcam_y = 0;
			else
			return (key_release2(keycode, data));
			*/return (1);
}
