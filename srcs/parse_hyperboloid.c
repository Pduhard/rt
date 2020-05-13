#include "rt.h"

static int   parse_hyperboloid_element(char **line, int *ret,
	t_hyperboloid *hyperboloid_param)
{
	char stripe;

	stripe = 0;
	*ret = 1;
	while (stripe != '>' && *ret != 0)
	{
		stripe = goto_next_element(line);
		if (!ft_strncmp_case(*line, "origin", 6))
			*ret = parse_origin(line, &hyperboloid_param->origin, 6);
		else if (!ft_strncmp_case(*line, "x_axis", 6))
			*ret = parse_double(line, 6, &hyperboloid_param->x_fact);
		else if (!ft_strncmp_case(*line, "y_axis", 6))
			*ret = parse_double(line, 6, &hyperboloid_param->y_fact);
		else if (!ft_strncmp_case(*line, "z_axis", 6))
			*ret = parse_double(line, 6, &hyperboloid_param->z_fact);
		else if (!ft_strncmp_case(*line, "surface", 7))
			*ret = parse_int(line, 7, &hyperboloid_param->surface);
		else if (stripe == '<')
			return (0);
	}
	return (1);
}

static int   check_hyperboloid_param(t_hyperboloid *hyperboloid_param, int ret)
{
	if (is_null(hyperboloid_param->x_fact) || hyperboloid_param->x_fact <= 0)
		ft_fdprintf(2, "Parse error: Hyperboloid: x axis factor must be greater than 0\n");
	else if (is_null(hyperboloid_param->y_fact) || hyperboloid_param->y_fact <= 0)
		ft_fdprintf(2, "Parse error: Hyperboloid: y axis factor must be greater than 0\n");
	else if (is_null(hyperboloid_param->z_fact) || hyperboloid_param->z_fact <= 0)
		ft_fdprintf(2, "Parse error: Hyperboloid: z axis factor must be greater than 0\n");
	else if (ret != 0)
		return (1);
	return (0);
}

int		parse_hyperboloid(char **line, t_obj *hyperboloid)
{
	int		ret;
	t_hyperboloid	*hyperboloid_param;

	if (hyperboloid->obj_param)
		return (error(ALREADYOBJ, NULL));
	if (!(hyperboloid_param = ft_memalloc(sizeof(t_hyperboloid))))
		return (0);
	if (!parse_hyperboloid_element(line, &ret, hyperboloid_param) ||
			!check_hyperboloid_param(hyperboloid_param, ret))
		return (syn_error(SERROR, HYPERBOL, AXIS, SURFACE));
	if (!hyperboloid_param->surface)
		hyperboloid_param->surface = -1;
	else
		hyperboloid_param->surface = 1;
	hyperboloid->obj_param = hyperboloid_param;
	assign_hyperboloid_function(hyperboloid);
	return (ret);
}
