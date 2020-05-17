#include "rt.h"

int		get_pixel_addr(t_2vecf text_coord, t_text_img *text, t_obj *obj)
{
	int		row;
	int		col;

	if (text_coord.val[1] < 0)
		col = (int)((1 - ((-text_coord.val[1] - (int)-text_coord.val[1])))
			* (double)(text->width));
	else
		col = (int)((text_coord.val[1] - (int)text_coord.val[1])
			* (double)(text->width));
	if (text_coord.val[0] < 0)
		row = (int)((1 - (-text_coord.val[0] - (int)-text_coord.val[0]))
			* (double)(text->height));
	else
		row = (int)((text_coord.val[0] - (int)text_coord.val[0])
			* (double)(text->height));
	row += obj->text.offset.val[0] * (double)(text->height);
	col += (1 - obj->text.offset.val[1]) * (double)(text->width);
	row %= text->height;
	col %= text->width;
	return (row * text->width + col);
}

t_4vecf	get_image_color(t_3vecf inter_point, t_3vecf normal_inter, t_obj *obj)
{
	t_2vecf		text_coord;
	t_4vecf		color;
	t_text_img	*text;
	int			pixel_addr;

	text = (t_text_img *)obj->text.text_param;
	text_coord = obj->get_text_coordinate(inter_point, normal_inter, obj);
	text_coord.val[1] *= obj->text.scale.val[1];
	text_coord.val[0] *= obj->text.scale.val[0];
	pixel_addr = get_pixel_addr(text_coord, text, obj);
	color.val[0] = (double)(text->pixels[pixel_addr] >> 24 & 0xff) / 255.;
	color.val[1] = (double)(text->pixels[pixel_addr] >> 16 & 0xff) / 255.;
	color.val[2] = (double)(text->pixels[pixel_addr] >> 8 & 0xff) / 255.;
	color.val[3] = (255. - (double)(text->pixels[pixel_addr] & 0xff)) / 255.;
	return (color);
	(void)normal_inter;
}
