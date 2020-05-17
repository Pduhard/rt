#include "rt.h"

int		parse_camera(char **line, t_data *data)
{
	char	stripe;
	int		ret;
	t_cam	*cam;

	stripe = 0;
	ret = 1;
	if (data->camera)
		return (error(ALREADYCAM, NULL));
	if (!(cam = ft_memalloc(sizeof(t_cam))))
		return (0);
	stripe = goto_next_element(line);
	while (stripe != '>' && ret != 0)
	{
		if (!ft_strncmp_case(*line, "origin", 6))
			ret = parse_origin(line, &cam->origin, 6);
		else if (!ft_strncmp_case(*line, "rotation", 8))
			ret = parse_rotation(line, &cam->rotation, 8);
		else if (stripe == '<')
			return (syn_error(SERROR, CAM, ORIGIN, ROTATION));
		stripe = goto_next_element(line);
	}
	data->camera = cam;
	if (!data->camera || ret == 0)
		return (syn_error(SERROR, CAM, ORIGIN, ROTATION));
	return (ret);
}
