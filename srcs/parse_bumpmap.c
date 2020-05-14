#include "rt.h"

int		parse_bump_mapping(char **line, t_obj *obj)
{
	char	stripe;
	int		ret;

	ret = 1;
	stripe = 0;
	while (stripe != '>' && ret != 0)
	{
		stripe = goto_next_element(line);
		if (!(ft_strncmp_case(*line, "own", 3)))
		{
			set_bump_own(obj);
			ret = parse_double(line, 3, &obj->text.bump_fact);
		}
		else if (!(ft_strncmp_case(*line, "independent", 11)))
			ret = parse_bump_inde(line, obj, 11);
		else if (stripe == '<')
			return (syn_error(SERROR, BUMPINDE, "\nOr\n", BUMPOWN));
	}
	return (ret);
}

void	set_bump_own(t_obj *obj)
{
	if (obj->text.text_type == TEXT_PERLIN)
	{
		obj->get_bump_mapping = &get_bump_mapping_perlin;
		obj->text.bump_type = BUMP_PERLIN;
	}
	else if (obj->text.text_type == TEXT_MARBLE)
	{
		obj->get_bump_mapping = &get_bump_mapping_marble;
		obj->text.bump_type = BUMP_MARBLE;
	}
	else if (obj->text.text_type == TEXT_FBM)
	{
		obj->get_bump_mapping = &get_bump_mapping_fbm;
		obj->text.bump_type = BUMP_FBM;
	}
	else if (obj->text.text_type == TEXT_WOOD)
	{
		obj->get_bump_mapping = &get_bump_mapping_wood;
		obj->text.bump_type = BUMP_WOOD;
	}
}

int		parse_bump_inde(char **line, t_obj *obj, int index)
{
	int		i;
	int		start;
	char	*s;
	char	*tmp;

	tmp = *line;
	i = index;
	while (ft_isspace(tmp[i]))
		++i;
	if (tmp[i] != '(')
		return (0);
	start = ++i;
	while (tmp[i] && tmp[i] != ')')
		++i;
	if (tmp[i] != ')')
		return (0);
	s = ft_strsub(tmp, start, i - start);
	set_bump_inde(s, obj);
	while (tmp[i] && tmp[i] != '(')
		++i;
	*line += i;
	return (parse_double(line, 0, &obj->text.bump_fact));
}

void	set_bump_inde(char *s, t_obj *obj)
{
	if (!(ft_strncmp_case(s, "PERLIN", 6)))
	{
		obj->get_bump_mapping = &get_bump_mapping_perlin;
		obj->text.bump_type = BUMP_PERLIN;
	}
	else if (!(ft_strncmp_case(s, "MARBLE", 6)))
	{
		obj->get_bump_mapping = &get_bump_mapping_marble;
		obj->text.bump_type = BUMP_MARBLE;
	}
	else if (!(ft_strncmp_case(s, "WOOD", 4)))
	{
		obj->get_bump_mapping = &get_bump_mapping_wood;
		obj->text.bump_type = BUMP_WOOD;
	}
	else if (!(ft_strncmp_case(s, "FBM", 3)))
	{
		obj->get_bump_mapping = &get_bump_mapping_fbm;
		obj->text.bump_type = BUMP_FBM;
	}
	else if (!(ft_strncmp_case(s, "WATER", 5)))
	{
		obj->get_bump_mapping = &get_bump_mapping_water;
		obj->text.bump_type = BUMP_WATER;
	}
}
