/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   init.c                                           .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: aplat <aplat@student.le-101.fr>            +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2019/12/21 22:19:28 by pduhard-     #+#   ##    ##    #+#       */
/*   Updated: 2020/01/27 18:47:58 by aplat       ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "rt.h"

static t_mlx	*init_mlx(void)
{
	t_mlx	*mlx;

	if (!(mlx = malloc(sizeof(t_mlx))))
		return (NULL);
	mlx->mlx_ptr = mlx_init();
	mlx->win_ptr = mlx_new_window(mlx->mlx_ptr,
			WIN_WIDTH, WIN_HEIGHT, "rtv1");
	mlx->img_ptr = mlx_new_image(mlx->mlx_ptr,
			WIN_WIDTH, WIN_HEIGHT);
	mlx->img_str = (int *)mlx_get_data_addr(mlx->img_ptr,
			&(mlx->bpp), &(mlx->s_l), &(mlx->endian));
	return (mlx);
}

t_data	*init_data(char *file_name)
{
	t_data	*data;
	//t_sphere	*param;

	(void)file_name;
	if (!(data = (t_data *)malloc(sizeof(t_data))))
		return (NULL);
	ft_bzero(data, sizeof(t_data));
	if (!(parse_rt_conf(file_name, data)))
		return (NULL); //free all
	printf("%p %p %p\n", data->lights, data->objs, data->camera);
	printf("%f %f %f\n", ((t_cylinder*)data->objs->obj_param)->center.val[0], ((t_cylinder*)data->objs->obj_param)->center.val[1], ((t_cylinder*)data->objs->obj_param)->center.val[2]);
	printf("%f %f \n", data->lights->param.val[2], data->lights->color.val[2]);
	if (!(data->mlx = init_mlx()))
	{
		free(data->objs);
		free(data);
		return (NULL);	
	}
	/*if (!(data->objs->obj_param = malloc(sizeof(t_sphere))))
	{
		free(data->objs);
		free(data);
		return (NULL);
	}*/
/*	if (!(data->lights = malloc(sizeof(t_light))))
	{
		free(data->objs);
		free(data);
		return (NULL);
	}
*///	data->lights->intensity = assign_3vecf(2,2,2);
//	data->lights->position = assign_3vecf(-4,-8, -17);
/*	param = (t_sphere *)data->objs->obj_param;
	param->x = -4;
	param->y = -8;
	param->z = 0;
	param->radius = 4;
	param = (t_sphere *)data->objs->next->obj_param;
	param->x = -4;
	param->y = -16;
	param->z = 0;
	param->radius = 4;
*/	//data->objs->color = 0xff0000;
	//data->objs->next->color = 0x0000ff;
//	init_camera_to_world_matrix(data->camera_to_world.val);
//	init_light_to_world_matrix(data->lights->l_to_world.val);
	data->rot_mat[0] = init_rotation_matrix_x(degree_to_radian(data->camera->rotation.val[0]));
	data->rot_mat[1] = init_rotation_matrix_y(degree_to_radian(data->camera->rotation.val[1]));
//	data->rot_mat[2] = init_rotation_matrix_z(degree_to_radian(data->camera->rotation.val[2]));
	//data->lights->color = assign_3vecf(1, 1, 1);
	//data->fov = 51.12;
	data->hooks = 0;
	data->fps = 0;
	data->delta_time = 0;
	data->anti_al = 1;
	return (data);
}
