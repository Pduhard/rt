#include "rt.h"

int		clip_color(double color)
{
	if (color > 255)
		return (255);
	else if (color < 0)
		return (0);
	else
		return ((int)color);
}

t_3vecf	window_to_view(double x, double y, double win_w, double win_h)
{
	t_3vecf	vec;

	vec.val[0] = x / win_w;
	vec.val[1] = y / win_h;
	if (win_w > win_h)
		vec.val[0] *= win_w / win_h;
	else if (win_h > win_w)
		vec.val[1] *= win_h / win_w;
	vec.val[2] = 1;
	return (vec);
}

void	ray_put_pixel(int i, int j, int *img, t_3vecf color, t_data *data)
{
	int		rgb_color;

	i = (int)data->size.val[0] / 2 + i;
	j = (int)data->size.val[1] / 2 + j;
	if (data->apply_color_filter)
		color = data->apply_color_filter(color);
	rgb_color = (clip_color(color.val[0] * 255) << 16);
	rgb_color |= (clip_color(color.val[1] * 255) << 8);
	rgb_color |= clip_color(color.val[2] * 255);
	img[j * (int)data->size.val[0] + i] = rgb_color;
}

void  check_subsampling(t_data *data, int i, int j, t_3vecf color)
{
	if (data->aa_adapt < 1)
	{
		if ((i + 1) * 2 != data->size.val[0])
			ray_put_pixel(i + 1, j, data->mlx->img_str, color, data);
		if ((j + 1) * 2 != data->size.val[1])
			ray_put_pixel(i, j + 1, data->mlx->img_str, color, data);
		if ((j + 1) * 2 != data->size.val[1] && (i + 1) * 2 != data->size.val[0])
			ray_put_pixel(i + 1, j + 1, data->mlx->img_str, color, data);
	}
}
