#include "rt.h"

/* fresnel_ratio:	SCHLICK approximation	3 times faster*/

double	compute_fresnel_ratio(t_3vecf dir, t_3vecf normal_inter, double refraction_index)
{
	double	r0;
	t_3vecf	inv_dir;
	double	cos_theta;
	double	inv_ctheta;

	inv_dir = assign_3vecf(-dir.val[0], -dir.val[1], -dir.val[2]);
	cos_theta = dot_product_3vecf(inv_dir, normal_inter);
	r0 = (1 - refraction_index) / (1 + refraction_index);
	r0 *= r0;
	inv_ctheta = 1 - cos_theta;
	inv_ctheta = inv_ctheta * inv_ctheta * inv_ctheta * inv_ctheta * inv_ctheta;
	return (r0 + (1 - r0) * (1 - cos_theta) * inv_ctheta);
}
