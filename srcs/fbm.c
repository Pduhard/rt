#include "rt.h"

double	fbm_rand(double p)
{
	p *= 0.010101;
	p = p - (int)p;
	p *= p + 7.5;
	p *= p + p;
	return (p - (int)p);
}

double	get_fbm_noise(t_3vecf point)
{
	t_3vecf int_part;
	t_3vecf	f_part;
	t_3vecf	u;
	double	n;
	double	l[6];

	int_part = assign_3vecf((int)point.val[0], (int)point.val[1],
		(int)point.val[2]);
	point.val[0] < 0 ? int_part.val[0]-- : int_part.val[0];
	point.val[1] < 0 ? int_part.val[1]-- : int_part.val[1];
	point.val[2] < 0 ? int_part.val[2]-- : int_part.val[2];
	f_part = sub_3vecf(point, int_part);
	n = dot_product_3vecf(int_part, assign_3vecf(110, 241, 171));
	u.val[0] = f_part.val[0] * f_part.val[0] * (3. - 2. * f_part.val[0]);
	u.val[1] = f_part.val[1] * f_part.val[1] * (3. - 2. * f_part.val[1]);
	u.val[2] = f_part.val[2] * f_part.val[2] * (3. - 2. * f_part.val[2]);
	l[0] = linear_interpolate(fbm_rand(n), fbm_rand(n + 110), u.val[0]);
	l[1] = linear_interpolate(fbm_rand(n + 241), fbm_rand(n + 351), u.val[0]);
	l[2] = linear_interpolate(fbm_rand(n + 171), fbm_rand(n + 281), u.val[0]);
	l[3] = linear_interpolate(fbm_rand(n + 412), fbm_rand(n + 522), u.val[0]);
	l[4] = linear_interpolate(l[0], l[1], u.val[1]);
	l[5] = linear_interpolate(l[2], l[3], u.val[1]);
	return (linear_interpolate(l[4], l[5], u.val[2]));
}

double	compute_3dfbm_factor(t_3vecf point, double scale)
{
	double	fact;
	double	ampl;
	double	shift;
	int		iter;

	shift = 100.;
	ampl = 0.5;
	fact = 0.;
	iter = 0;
	point.val[0] *= scale;
	point.val[1] *= scale;
	point.val[2] *= scale;
	while (iter < FBM_ITER)
	{
		fact += ampl * get_fbm_noise(point);
		point.val[0] = point.val[0] * FBM_LACUNARITY + shift;
		point.val[1] = point.val[1] * FBM_LACUNARITY + shift;
		point.val[2] = point.val[2] * FBM_LACUNARITY + shift;
		ampl *= FBM_GAIN;
		iter++;
	}
	return (fact);
	(void)scale;
}
