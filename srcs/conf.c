#include "rt.h"

int		return_update(char *error, int ret, int skip)
{
	ft_fdprintf(skip, error);
	return (ret);
}

int		clamp_val(double *val, double min, double max)
{
	if (*val < min)
		*val = min;
	else if (*val > max)
		*val = max;
	return (0);
}

char	goto_next_element(char **line)
{
	char	*s;
	char	c;

	s = *line;
	while (*s && *s != '<' && *s != '>')
		++s;
	c = *s;
	s++;
	while (ft_isspace(*s))
		s++;
	if (*s == '\0')
		return ('>');
	*line = s;
	return (c);
}

int		parse_int(char **line, int i, int *val)
{
	char *s;

	s = *line;
	while (ft_isspace(s[i]))
		i++;
	if (s[i] == '(')
		i++;
	*val = ft_atoi(&(s[i]));
	while (s[i] && s[i] != ')')
			++i;
	if (!s[i])
		return (0);
	if (goto_next_element(line) != '>')
	{
		printf("Error parse_int\n");
		return(0);
	}
	return (1);
}

int		parse_double(char **line, int i, double *val)
{
	char *s;

	s = *line;
	while (ft_isspace(s[i]))
		i++;
	if (s[i] == '(')
		i++;
	*val = ft_atold(&(s[i]));
	while (s[i] && s[i] != ')' && s[i] != '>')
			++i;
	if (!s[i])
		return (0);
	if (goto_next_element(line) != '>')
		return (error("Error parse double\n", NULL));
	return (1);
}

int		check_normal(t_3vecf *t)
{
	if (t->val[0] == 0 && t->val[1] == 0 && t->val[2] == 0)
		return (error("Normal Nul\n", NULL));
	return (1);
}

int		isequal_3vecf(t_3vecf *t1, t_3vecf *t2)
{
	if (t1->val[0] == t2->val[0] && t1->val[1] == t2->val[1] && t1->val[2] == t2->val[2])
		return (error("Same Origin and Tip\n", NULL));
	return (1);
}
