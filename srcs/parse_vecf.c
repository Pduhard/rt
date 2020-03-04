#include "rt.h"

int		parse_3vecf(char *line, int i, t_3vecf *vec)
{
	int		cpt;

	cpt = 0;
	if (line[i] == '(')
		i++;
	while (cpt < 3)
	{
		vec->val[cpt++] = ft_atold(&(line[i]));
		while (line[i] && line[i] != ')' && line[i] != ',' && line[i] != '>')
			++i;
		if (line[i] == ')')
			return (i);
		if (!line[i] || line[i] == '>')
			return (-1);
		++i;
	}
	return (i);
}

int		parse_2vecf(char *line, int i, t_2vecf *vec)
{
	int		cpt;

	cpt = 0;
	if (line[i] == '(')
		i++;
	while (cpt < 2)
	{
		vec->val[cpt++] = ft_atold(&(line[i]));
		while (line[i] && ((line[i] != ',' && cpt < 2)
			|| (line[i] != ')' && cpt == 2 && line[i] != '>')))
			++i;
		if (!line[i] || line[i] == '>')
			return (-1);
		++i;
	}
	return (i);
}

int		parse_4vecf(char *line, int i, t_4vecf *vec)
{
	int		cpt;

	cpt = 0;
	++i;
	if (line[i] == '(')
		i++;
	while (cpt < 4)
	{
		vec->val[cpt++] = ft_atold(&(line[i]));
		while (line[i] && line[i] != ')' && line[i] != ',' && line[i] != '>')
			++i;
		if (!line[i])
			return (-1);
		++i;
	}
	while (line[i] && line[i] != ')')
		++i;
	if (line[i] != ')')
		return (-1);
	return (i);
}

int		parse_origin(char **line, t_3vecf *t, int i)
{
	char	*s;

	s = *line;
	while (ft_isspace(s[i]))
		++i;
	if (s[i] != '(' || (i = parse_3vecf(s, i, t)) == -1)
		return (0);
	if (goto_next_element(line) != '>')
		return (error("Error parse_origin\n", NULL));
	return (1);
}

int		parse_rotation(char **line, t_2vecf *t, int i)
{
	char	*s;

	s = *line;
	while (ft_isspace(s[i]))
		++i;
	if (s[i] != '(' || (i = parse_2vecf(s, i, t)) == -1)
		return (0);
	if (goto_next_element(line) != '>')
		return (error("Error parse_rotation\n", NULL));
	return (1);
}
