#include "rt.h"

int     syn_error(char *s1, char *s2, char *s3, char *s4)
{
    s3 = (!s3 ? "" : s3);
    s4 = (!s4 ? "" : s4);
    ft_fdprintf(2, "%s%s%s%s>\n", s1, s2, s3, s4);
    return (0);
}

int     error(char *s1, char *s2)
{
    s2 = (!s2 ? "" : s2);
    ft_fdprintf(2, "%s%s", s1, s2);
    return (0);
}