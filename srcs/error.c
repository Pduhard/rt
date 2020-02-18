#include "rt.h"

int     syn_error(char *s1, char *s2, char *s3, char *s4, char *s5)
{
    if (!s5)
        s5 = "";
    ft_fdprintf(2, "%s%s%s%s%s>\n", s1, s2, s3, s4, s5);
    return (0);
}

int     error(char *s1)
{
    ft_fdprintf(2, "%s", s1);
    return (0);
}