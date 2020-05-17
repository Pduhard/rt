#include "rt.h"

int			parse_composed_model(char **line, t_data *data)
{
	char		stripe;
	int			ret;
	t_composed	*composed;

	stripe = 0;
	ret = 1;
	if (!(composed = ft_memalloc(sizeof(t_composed))))
		return (0);
	while (stripe != '>' && ret != 0)
	{
		stripe = goto_next_element(line);
		if (!(ft_strncmp_case(*line, "objects", 7)))
			ret = parse_objects(line, data, composed);
		else if (!(ft_strncmp_case(*line, "name", 4)))
			ret = parse_name(line, &composed->name, 4);
	}
	if (data->composed_objs)
		composed->next = data->composed_objs;
	else
		composed->next = NULL;
	data->composed_objs = composed;
	return (ret);
}
