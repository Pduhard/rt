#include "rt.h"

static int   parse_horse_saddle_element(char **line, int *ret,
	t_horse_saddle *horse_saddle_param)
{
	char stripe;

	*ret = 1;
	stripe = 0;
	while (stripe != '>' && *ret != 0)
	{
		stripe = goto_next_element(line);
		if (!ft_strncmp_case(*line, "origin", 6))
			*ret = parse_origin(line, &horse_saddle_param->origin, 6);
		else if (!ft_strncmp_case(*line, "x_fact", 6))
			*ret = parse_double(line, 6, &horse_saddle_param->x_fact);
		else if (!ft_strncmp_case(*line, "y_fact", 6))
			*ret = parse_double(line, 6, &horse_saddle_param->y_fact);
		else if (stripe == '<')
			return (0);
	}
	return (1);
}

static int   check_horse_saddle_param(t_horse_saddle *horse_saddle_param, int ret)
{
	if (horse_saddle_param->x_fact <= 0 || is_null(horse_saddle_param->x_fact))
		ft_fdprintf(2, "Parse error: Horse saddle: x fact must be greater than 0\n");
	else if (horse_saddle_param->y_fact <= 0 || is_null(horse_saddle_param->y_fact))
		ft_fdprintf(2, "Parse error: Horse saddle: y fact must be greater than 0\n");
	else if (ret != 0)
		return (1);
	return (0);
}

int		parse_horse_saddle(char **line, t_obj *horse_saddle)
{
	int		ret;
	t_horse_saddle	*horse_saddle_param;

	if (horse_saddle->obj_param)
		return (error(ALREADYOBJ, NULL));
	if (!(horse_saddle_param = ft_memalloc(sizeof(t_horse_saddle))))
		return (0);
	if (!parse_horse_saddle_element(line, &ret, horse_saddle_param) ||
			!check_horse_saddle_param(horse_saddle_param, ret))
		return (syn_error(SERROR, HORSE, HORSEF, NULL));
	horse_saddle->obj_param = horse_saddle_param;
	assign_horse_saddle_function(horse_saddle);
	return (ret);
}
