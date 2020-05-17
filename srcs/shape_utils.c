#include "rt.h"

int		is_closest_intersect(t_dist dist, double root)
{
	if (root < *(dist.dist) && root > dist.min_dist && root < dist.max_dist)
	{
		*(dist.dist) = root;
		return (1);
	}
	return (0);
}

void	get_uv_axis(t_3vecf axis[3], t_3vecf first_axis)
{
	axis[0] = assign_3vecf(0, 0, 0);
	axis[1] = first_axis;
	normalize_3vecf(&(axis[1]));
	if (axis[1].val[0] != 0)
	{
		axis[0] = assign_3vecf(0, 1, 1);
		axis[0].val[0] = (-axis[1].val[1] - axis[1].val[2])
			/ axis[1].val[0];
	}
	else if (axis[1].val[1] != 0)
	{
		axis[0] = assign_3vecf(1, 0, 1);
		axis[0].val[1] = (-axis[1].val[0] - axis[1].val[2])
			/ axis[1].val[1];
	}
	else if (axis[1].val[2] != 0)
	{
		axis[0] = assign_3vecf(1, 1, 0);
		axis[0].val[2] = (-axis[1].val[0] - axis[1].val[1])
			/ axis[1].val[2];
	}
	normalize_3vecf(&(axis[0]));
	axis[2] = product_3vecf(axis[0], axis[1]);
}
