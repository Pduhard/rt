#include "rt.h"

double	dot_product_3vecf(t_3vecf a, t_3vecf b)
{
	return (a.val[0] * b.val[0] + a.val[1] * b.val[1] + a.val[2] * b.val[2]);
}

t_3vecf	sub_3vecf(t_3vecf a, t_3vecf b)
{
	t_3vecf	vec;

	vec.val[0] = a.val[0] - b.val[0];
	vec.val[1] = a.val[1] - b.val[1];
	vec.val[2] = a.val[2] - b.val[2];
	return (vec);
}

t_3vecf	add_3vecf(t_3vecf a, t_3vecf b)
{
	t_3vecf	vec;

	vec.val[0] = a.val[0] + b.val[0];
	vec.val[1] = a.val[1] + b.val[1];
	vec.val[2] = a.val[2] + b.val[2];
	return (vec);
}

t_3vecf	product_3vecf(t_3vecf a, t_3vecf b)
{
	t_3vecf	vec;

	vec.val[0] = (a.val[1] * b.val[2] - a.val[2] * b.val[1]);
	vec.val[1] = (a.val[2] * b.val[0] - a.val[0] * b.val[2]);
	vec.val[2] = (a.val[0] * b.val[1] - a.val[1] * b.val[0]);
	return (vec);
}

double	get_length_3vecf(t_3vecf vec)
{
	return (sqrtf(dot_product_3vecf(vec, vec)));
}
