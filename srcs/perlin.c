/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   perlin.c                                         .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: pduhard- <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2020/01/22 04:26:42 by pduhard-     #+#   ##    ##    #+#       */
/*   Updated: 2020/01/29 21:11:22 by pduhard-    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "rt.h"

static t_3vecf		rand_g(const unsigned char permutation[512],
	const t_3vecf gradient[16], int x, int y, int z)
{
	return (gradient[permutation[z + permutation[y + permutation[x]]] & 15]);
}

static double	lin_i(double a, double b, double val)
{
	return (a + (b - a) * val);
}

static double	dp(t_3vecf a, t_3vecf b)
{
	return (a.val[0] * b.val[0] + a.val[1] * b.val[1] + a.val[2] * b.val[2]);
}

double	compute_3dperlin_factor(t_3vecf inter_point, double scale,
	const unsigned char p[512], const t_3vecf g[16])
{
	t_3vecf	i;
	t_3vecf f;
	double  q[3];
	double  d[8];

	handle_perlin_inter_point(&i, &f, inter_point, scale);
	set_quintic_factors(q, f);
	d[0] = dp(rand_g(p, g, i.val[0], i.val[1], i.val[2]), f);
	d[1] = dp(rand_g(p, g, i.val[0], i.val[1], i.val[2] + 1),
						assign_3vecf(f.val[0], f.val[1], f.val[2] - 1.0));
	d[2] = dp(rand_g(p, g, i.val[0], i.val[1] + 1, i.val[2]),
						assign_3vecf(f.val[0], f.val[1] - 1.0, f.val[2]));
	d[3] = dp(rand_g(p, g, i.val[0], i.val[1] + 1, i.val[2] + 1),
						assign_3vecf(f.val[0], f.val[1] - 1.0, f.val[2] - 1.0));
	d[4] = dp(rand_g(p, g, i.val[0] + 1, i.val[1], i.val[2]),
						assign_3vecf(f.val[0] - 1.0, f.val[1], f.val[2]));
	d[5] = dp(rand_g(p, g, i.val[0] + 1, i.val[1], i.val[2] + 1),
						assign_3vecf(f.val[0] - 1.0, f.val[1], f.val[2] - 1.0));
	d[6] = dp(rand_g(p, g, i.val[0] + 1, i.val[1] + 1, i.val[2]),
						assign_3vecf(f.val[0] - 1.0, f.val[1] - 1.0, f.val[2]));
	d[7] = dp(rand_g(p, g, i.val[0] + 1, i.val[1] + 1, i.val[2] + 1),
						assign_3vecf(f.val[0] - 1.0, f.val[1] - 1.0, f.val[2] - 1.0));
	return (lin_i(lin_i(lin_i(d[0], d[4], q[2]), lin_i(d[2], d[6], q[2]), q[1]),
		lin_i(lin_i(d[1], d[5], q[2]), lin_i(d[3], d[7], q[2]), q[1]), q[0]));
}
