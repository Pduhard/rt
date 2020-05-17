#include "rt.h"

static char	*get_image_file_name(char *scene_name)
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
	return (image_name);
}

int		check_pscreen_key_press(int keycode, t_data *data)
{
	SDL_Surface	*img_save;
	char		*image_file_name;

	if (keycode == IMPR_KEY || keycode == P_KEY)
	{
		img_save = SDL_CreateRGBSurfaceFrom(
			(void *)data->mlx->img_str,
			(int)data->size.val[0],
			(int)data->size.val[1],
			32, 4 * (int)data->size.val[0],
			0xff << 16, 0xff << 8, 0xff << 0, 0x0);
		image_file_name = get_image_file_name(data->scene_name);
		IMG_SavePNG(img_save, image_file_name);
		free(image_file_name);
		SDL_FreeSurface(img_save);
		return (1);
	}
	return (0);
}
