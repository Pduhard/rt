#include "rt.h"

static int   parse_monkey_saddle_element(char **line, int *ret,
	t_monkey_saddle *monkey_saddle_param)
{
	char stripe;

	*ret = 1;
	stripe = 0;
	while (stripe != '>' && *ret != 0)
	{
		stripe = goto_next_element(line);
		if (!ft_strncmp_case(*line, "origin", 6))
			*ret = parse_origin(line, &monkey_saddle_param->origin, 6);
		else if (stripe == '<')
			return (0);
	}
	return (1);
}

int		parse_monkey_saddle(char **line, t_obj *monkey_saddle)
{
	int		ret;
	t_monkey_saddle	*monkey_saddle_param;

	if (monkey_saddle->obj_param)
		return (error(ALREADYOBJ, NULL));
	if (!(monkey_saddle_param = ft_memalloc(sizeof(t_monkey_saddle))))
		return (0);
	if (!parse_monkey_saddle_element(line, &ret, monkey_saddle_param) || ret == 0)
		return (syn_error(SERROR, MONKEY, ORIGIN, NULL));
	monkey_saddle->obj_param = monkey_saddle_param;
	assign_monkey_saddle_function(monkey_saddle);
	return (ret);
}
