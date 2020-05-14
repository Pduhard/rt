#include "rt.h"

static int   parse_moebius_element(char **line, int *ret, t_moebius *moebius_param)
{
	char stripe;

	stripe = 0;
	*ret = 1;
	while (stripe != '>' && *ret != 0)
	{
		stripe = goto_next_element(line);
		if (!ft_strncmp_case(*line, "origin", 6))
			*ret = parse_origin(line, &moebius_param->origin, 6);
		else if (!ft_strncmp_case(*line, "radius", 6))
			*ret = parse_double(line, 6, &moebius_param->radius);
		else if (!ft_strncmp_case(*line, "half_width", 10))
			*ret = parse_double(line, 10, &moebius_param->half_width);
		else if (stripe == '<')
			return (0);
	}
	return (1);
}

static int   check_moebius_param(t_moebius *moebius_param, int ret)
{
	if (moebius_param->radius <= 0. || is_null(moebius_param->radius))
		ft_fdprintf(2, "Parse error: Moebius: radius must be greater than 0\n");
	else if (moebius_param->half_width <= 0. || is_null(moebius_param->half_width))
		ft_fdprintf(2, "Parse error: Moebius: half width must be greater than 0\n");
	else if (ret != 0)
		return (1);
	return (0);
}

int		parse_moebius(char **line, t_obj *moebius)
{
	char	stripe;
	int		ret;
	t_moebius	*moebius_param;

	stripe = 0;
	ret = 1;
	if (moebius->obj_param)
		return (error(ALREADYOBJ, NULL));
	if (!(moebius_param = ft_memalloc(sizeof(t_moebius))))
		return (0);
	if (!parse_moebius_element(line, &ret, moebius_param) ||
		!check_moebius_param(moebius_param, ret))
		return (syn_error(SERROR, MOEBIUS, RADIUS, HALFWIDTH));
	moebius->obj_param = moebius_param;
	assign_moebius_function(moebius);
	return (ret);
}
