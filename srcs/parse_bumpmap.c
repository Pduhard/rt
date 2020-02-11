#include "rt.h"

int		parse_bump_mapping(char **line, t_obj *obj)
{
//	t_text *text;
	char	stripe;
	int		ret;

//	text = obj
	ret = 1;
	stripe = 0;
	while (stripe != '>' && ret != 0)
	{
		printf("Parse_BUMP BEFORE ==> %s\n", *line);
		stripe = goto_next_element(line);
		printf("Parse_BUMP ==> %s\n", *line);
		if (!(ft_strncmp_case(*line, "own", 3)))
		{
			set_bump_own(obj);
			ret = parse_double2(line, 3, &obj->text.bump_fact);
		}
		else if (!(ft_strncmp_case(*line, "independent", 11)))
		{
			ret = parse_bump_inde(line, obj, 11);
		}
/*		else
		{
			ft_printf("PAs un bon param de bumpmapping ==> %s\n", *line);
			return (0);
		}*/
	}
	return (ret);
}

void	set_bump_own(t_obj *obj)//t_text *text)
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
	else if (obj->text.text_type == TEXT_WOOD)
	{
		obj->get_bump_mapping = &get_bump_mapping_wood;
		obj->text.bump_type = BUMP_WOOD;
	}
	else if (obj->text.text_type == TEXT_IMAGE)
	{
		obj->get_bump_mapping = &get_bump_mapping_image;
		obj->text.bump_type = BUMP_IMAGE;
	}
}

int		parse_bump_inde(char **line, t_obj *obj, /*t_text *text, */int	index)
{
	int	i;
	int	start;
	char	*s;
	char	*tmp;

	tmp = *line;
	printf("\n\n\nTEST\n\n\n%s\n\n\n", *line);
	i = index;
	while (ft_isspace(tmp[i]))
		++i;
	if (tmp[i] != '(')
	{
		printf("\n\n\n\nhbjviudvdsv\n\n\n==> %s\n", &tmp[i]);
		return (0);
	}
	start = ++i;
	while (tmp[i] && tmp[i] != ')')
		++i;
	printf("Into Parse_Bump inde ==> %s\n", &tmp[i]);
	if (tmp[i] != ')')
		return (0);
	s =	ft_strsub(tmp, start, i - start);
	set_bump_inde(s, obj);//text);
	printf("Type bump Inde ==> %s\n", s);
	while (tmp[i] && tmp[i] != '(')
		++i;
	*line += i;
	printf("Line apres bump ==> %s\n", *line);
	return (parse_double2(line, 0, &obj->text.bump_fact));
}

void	set_bump_inde(char *s, t_obj *obj)//t_text *text)
{
	if (!(ft_strncmp_case(s, "PERLIN", 6)))
	{
		obj->get_bump_mapping = &get_bump_mapping_perlin;
		obj->text.bump_type = BUMP_PERLIN;
	}

	//	text->bump_type = BUMP_PERLIN;
	if (!(ft_strncmp_case(s, "MARBLE", 6)))
	{
		obj->get_bump_mapping = &get_bump_mapping_marble;
		obj->text.bump_type = BUMP_MARBLE;
	}

	//	text->bump_type = BUMP_MARBLE;
	if (!(ft_strncmp_case(s, "WOOD", 4)))
	{
		obj->get_bump_mapping = &get_bump_mapping_wood;
		obj->text.bump_type = BUMP_WOOD;
	}

//		text->bump_type = BUMP_WOOD;
	if (!(ft_strncmp_case(s, "IMAGE", 5)))
	{
		obj->get_bump_mapping = &get_bump_mapping_image;
		obj->text.bump_type = BUMP_IMAGE;
	}

	//	text->bump_type = BUMP_IMAGE;
	if (!(ft_strncmp_case(s, "SINUS", 5)))
	{
		obj->get_bump_mapping = &get_bump_mapping_sinus;
		obj->text.bump_type = BUMP_SINUS;
	}

	//	text->bump_type = BUMP_SINUS;
}