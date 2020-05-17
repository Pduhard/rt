#include "rt.h"

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
