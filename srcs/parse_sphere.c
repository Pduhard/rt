#include "rt.h"

static int   check_sphere_param(t_sphere *sphere_param, int ret)
{
	if (sphere_param->radius <= 0 || is_null(sphere_param->radius))
		ft_fdprintf(2, "Parse error: Sphere: radius must be greater than 0\n");
	else if (ret != 0)
		return (1);
	return (0);
}

static int   parse_sphere_element(char **line, int *ret, t_sphere *sphere_param)
{
	char stripe;

	stripe = 0;
	*ret = 1;
	while (stripe != '>' && *ret != 0)
	{
		stripe = goto_next_element(line);
		if (!ft_strncmp_case(*line, "origin", 6))
			*ret = parse_origin(line, &sphere_param->origin, 6);
		else if (!ft_strncmp_case(*line, "radius", 6))
			*ret = parse_double(line, 6, &sphere_param->radius);
		else if (stripe == '<')
			return (0);
	}
	return (1);
}

int		parse_sphere(char **line, t_obj *sphere)
{
	int		ret;
	t_sphere	*sphere_param;

	if (sphere->obj_param)
		return (error(ALREADYOBJ, NULL));
	if (!(sphere_param = ft_memalloc(sizeof(t_sphere))))
		return (0);
	if (!parse_sphere_element(line, &ret, sphere_param) ||
			!check_sphere_param(sphere_param, ret))
		return (syn_error(SERROR, SPHERE, RADIUS, NULL));
	sphere->obj_param = sphere_param;
	assign_sphere_function(sphere);
	return (ret);
}
