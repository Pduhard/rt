#include "rt.h"

double	dot_product_2vecf(t_2vecf a, t_2vecf b)
{
	return (a.val[0] * b.val[0] + a.val[1] * b.val[1]);
}

t_2vecf	assign_2vecf(double x, double y)
{
	t_2vecf	vec;

	vec.val[0] = x;
	vec.val[1] = y;
	return (vec);
}