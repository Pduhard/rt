#include "rt.h"

int		check_skybox(t_data *data)
{
	t_sphere	*param;
	t_text_img	*image;
	t_obj		*sky;

	if (!data->skybox_name)
		return (1);
	if (!(image = parse_img(data->skybox_name)) ||
			!(sky = ft_memalloc(sizeof(t_obj))) ||
			!(param = ft_memalloc(sizeof(t_sphere))))
		return (0);
	sky->text.text_type = TEXT_IMAGE;
	sky->text.scale.val[0] = 1;
	sky->text.scale.val[1] = 1;
	sky->text.text_param = image;
	param->origin = assign_3vecf(0, 0, 0);
	param->radius = 10000;
	sky->obj_param = param;
	assign_sphere_function(sky);
	sky->obj_type = OBJ_SKYBOX;
	sky->get_text_color = &get_image_color;
	add_object(sky, data);
	return (1);
}
