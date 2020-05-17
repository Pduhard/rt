#include "rt.h"

void			write_cmd_strings(void *mp, void *wp)
{
	mlx_string_put(mp, wp, 150, 10, 0xFFFFFF, "Controls");
	mlx_string_put(mp, wp, 10, 60, 0xFFFFFF, "Cam Mode :");
	mlx_string_put(mp, wp, 5, 100, 0xFFFFFF, ESC);
	mlx_string_put(mp, wp, 5, 125, 0xFFFFFF, WS);
	mlx_string_put(mp, wp, 5, 150, 0xFFFFFF, AD);
	mlx_string_put(mp, wp, 5, 175, 0xFFFFFF, UPDOWN);
	mlx_string_put(mp, wp, 5, 200, 0xFFFFFF, LEFTRIGHT);
	mlx_string_put(mp, wp, 5, 225, 0xFFFFFF, "SPACE        ==> Up");
	mlx_string_put(mp, wp, 5, 250, 0xFFFFFF, "SHIFT        ==> Down");
	mlx_string_put(mp, wp, 5, 275, 0xFFFFFF, "LEFT CLICK   ==> Select Object");
	mlx_string_put(mp, wp, 10, 325, 0xFFFFFF, "Mode Move_Object :");
	mlx_string_put(mp, wp, 5, 375, 0xFFFFFF, WASD);
	mlx_string_put(mp, wp, 5, 400, 0xFFFFFF, "ARROWS       ==> Rotate Object");
	mlx_string_put(mp, wp, 5, 425, 0xFFFFFF, "SPACE        ==> Up Object");
	mlx_string_put(mp, wp, 5, 450, 0xFFFFFF, "SHIFT        ==> Down Object");
	mlx_string_put(mp, wp, 5, 475, 0xFFFFFF, LEFTCLICK);
}

int				open_info(t_data *data)
{
	if (!data->info)
	{
		if (!(data->info = malloc(sizeof(t_mlx))))
			return (0);
		data->info->win_ptr = mlx_new_window(data->mlx->mlx_ptr,
			400, 500, "Informations");
		write_cmd_strings(data->mlx->mlx_ptr, data->info->win_ptr);
	}
	else
		write_cmd_strings(data->mlx->mlx_ptr, data->info->win_ptr);
	return (1);
}
