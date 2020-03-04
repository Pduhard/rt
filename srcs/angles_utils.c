#include "rt.h"

int		is_null(double value)
{
	return (value < 0.0001 && value > -0.0001);
}

double	degree_to_radian(double degree)
{
	return (degree * _M_PI_180);
}
