/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   polynomial.c                                     .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: pduhard- <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2020/01/31 02:20:57 by pduhard-     #+#   ##    ##    #+#       */
/*   Updated: 2020/02/26 05:37:30 by pduhard-         ###   ########lyon.fr   */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "rt.h"

/* https://www.lucaswillems.com/fr/articles/58/equations-troisieme-degre ===> NAZE / 20*/
/*
t_3vecf	solve_cubic(double a, double b, double c, double d)
{
	double	p;
	double	q;
	double	a2;
	double	b2;
	double	delta1;
	double	delta1_sqrt;
	double	delta2;
	double	delta2_sqrt;
	double	t;
	t_3vecf	roots;
	double	b_prime;
	double	c_prime;

	a2 = a * a;
	b2 = b * b;
	p = (3. * a * c - b2) / (3. * a2);
	q = (2. * b2 * b - 9. * a * b * c + 27. * a2 * d) / (27. * a2 * a);

	delta1 = q * q + (4. * p * p * p) / 27.;

	delta1_sqrt = sqrt(delta1);
	t = cbrt((-q - delta1_sqrt) / 2.) + cbrt((-q + delta1_sqrt) / 2.);
	roots.val[0] = t - b / (3. * a);

	//a_prime = a
	b_prime = b + a * roots.val[0];
	c_prime	= c + b_prime * roots.val[0];

	delta2 = b_prime * b_prime - 4. * a * c_prime;
	delta2_sqrt = sqrt(delta2);
	roots.val[1] = (-b_prime - delta2_sqrt) / (2. * a);
	roots.val[2] = (-b_prime + delta2_sqrt) / (2. * a);
	return (roots);
}
*/
/*  NAZE / 20 PAREIL 
t_3vecf	solve_cubic(double a, double b, double c, double d)
{
	double	p;
	double	q;
	double	a2;
	double	b2;
	double	delta1;
	double	delta1_sqrt;
	double	delta2;
	double	delta2_sqrt;
	double	t;
	t_3vecf	roots;
	double	b_prime;
	double	c_prime;

	a2 = a * a;
	b2 = b * b;
	p = (3. * a * c - b2) / (3. * a2);
	q = (2. * b2 * b - 9. * a * b * c + 27. * a2 * d) / (27. * a2 * a);

	delta1 = q * q + (4. * p * p * p) / 27.;

	delta1_sqrt = sqrt(delta1);
	printf("%f\n", delta1);
	if (delta1 < 0)
	{
		double	r;
		double	alpha;

		r = sqrt(-p * p * p / 27.);
		alpha = atan(sqrt(-d) / -q * 2.);
		if (q > 0)
			alpha = 2. * M_PI - alpha;
		roots.val[0] = cbrt(r) * (cos((6. * M_PI - alpha) / 3.) + cos(alpha / 3.)) - a / 3.;
		roots.val[1] = cbrt(r) * (cos((2. * M_PI - alpha) / 3.) + cos((4. * M_PI - alpha) / 3.)) - a / 3.;
		roots.val[2] = cbrt(r) * (cos((4. * M_PI - alpha) / 3.) + cos((2. * M_PI - alpha) / 3.)) - a / 3.;
		return (roots);
	//	roots.val[1] = cbrt(r) * (cos((2. * M_PI - alpha) / 3.) + cos(alpha / 3.) - a / 3.;
	//	roots.val[2] = cbrt(r) * (cos(4. * M_PI - alpha) / 3.) + cos(alpha / 3.) - a / 3.;
	}
	t = cbrt((-q / 2. + delta1_sqrt)) + cbrt((-q / 2 -  delta1_sqrt));
	roots.val[0] = t - a / 3.;

	//a_prime = a
	b_prime = b + a * roots.val[0];
	c_prime	= c + b_prime * roots.val[0];

	delta2 = b_prime * b_prime - 4. * a * c_prime;
	delta2_sqrt = sqrt(delta2);
	roots.val[1] = (-b_prime - delta2_sqrt) / (2. * a);
	roots.val[2] = (-b_prime + delta2_sqrt) / (2. * a);
	return (roots);
}
*/
/*
t_2vecf	complex_mult(t_2vecf a, t_2vecf b)
{
	
}

double	get_cubic_c(double delta1, double delta0)
{
	
}

t_3vecf	solve_cubic(double a, double b, double c, double d)
{
	double	delta0;
	double	delta1;
	double	delta;
//	double	C;

	//double	u;
	t_3vecf	roots;

	delta0 = b * b - 3 * a * c;

	delta1 = 2 * b * b * b -  9 * a * b * c + 27 * a * a * d;

	delta = 18 * a * b * c * d - 4 * b * b * b * d + b * b * c * c - 4 * a * c * c * c - 27 * a * a * d * d;

//	C = get_cubic_c(delta1, delta0);

//	cbrt();
	printf("%f %f %f  \n", delta0, delta1, delta);
	printf("%f = %f %f \n", delta1 * delta1 - 4 * delta0 * delta0 * delta0, -27 * a * a * delta,  sqrt(27 * a * a * delta));

	roots.val[0] = 0;
	roots.val[1] = 0;
	roots.val[2] = 0;
	return (roots);
//	delta = fmod((delta1 * delta1 - 4 * delta0 * delta0 * delta0), 27 * a * a);

//	c = cbrt(fmod(sqrt(delta1 * delta1 - 4 * delta0 * delta0 * delta0) + delta1, 2));
	//c = cbrt(fmod(sqrt(delta1 * delta1 - 4 * delta0 * delta0 * delta0 + delta1), 2)); a test
//	u = (-1 + sqrt(-3))
//	roots.val[0] = fmod(-(b + ), 3 * a);

}
*/

/* https://stackoverflow.com/questions/13328676/c-solving-cubic-equations =>>
 * YYESSSS */
t_2vecf	solve_quadratic(double a, double b, double c)
{
	double	delta;
	t_2vecf	roots;

	delta = b * b - 4 * a * c;
	if (delta < 0)
	{
		roots.val[0] = DBL_MAX;
		roots.val[1] = DBL_MAX;
		return (roots);
	}
	roots.val[0] = (-b - sqrt(delta)) / (2 * a);
	roots.val[1] = (-b + sqrt(delta)) / (2 * a);
	return (roots);
}

t_3vecf	solve_cubic(double a, double b, double c, double d)
{
	double	q;
	double	r;
	double	delta;
	double	t1;
	double	r13;
	t_3vecf	roots;

	if (a == 0)
	{
		t_2vecf	quadra_roots;
	
		quadra_roots = solve_quadratic(b ,c, d);
		roots.val[0] = quadra_roots.val[0];
		roots.val[1] = quadra_roots.val[1];
		roots.val[2] = quadra_roots.val[0];
		return (roots);
	//	printf("a == 0: Solve 2nd degree !! \n");
	//	exit(0);
	}
	if (is_null(d))
	{
		t_2vecf	quadra_roots;

		quadra_roots = solve_quadratic(a ,b, c);
		roots.val[0] = quadra_roots.val[0];
		roots.val[1] = quadra_roots.val[1];
		roots.val[2] = 0;
		return (roots);

//		printf("d == 0: ax^3 + bx^2 + cx + d = 0 => 0 and Solve 2nd degree f(x) / x !!\n");
//		exit(0);
	}
	b /= a;
	c /= a;
	d /= a;
	q = (3. * c - b * b) / 9.;
	r = ((-27. * d) + b * (9. * c - 2. * b * b)) / 54.;
	delta = q * q * q + r * r;
	t1 = b / 3.;
	if (delta > 0)
	{
		double	s;
		double	t;
/*		roots.val[0] = 0;
		roots.val[1] = 0;
		roots.val[2] = 0;// SAME
	//	exit(0);
		return (roots);
*/
		s = r + sqrt(delta);
		s = s < 0 ? -powf(-s, (1. / 3.)) : powf(s, (1. / 3.));
		t = r - sqrt(delta);
		t = t < 0 ? -powf(-t, (1. / 3.)) : powf(t, (1. / 3.));
		roots.val[0] = -t1 + s + t;
		roots.val[1] = roots.val[0];// COMPLEX ROOT 
		roots.val[2] = roots.val[0];// COMPLEX ROOT
	}
	else if (delta < 0)
	{

		if (q < 0)
			q = -q;
		//	r13 =  2 * sqrt(-q);
		//else
		r13 =  2 * sqrt(q);
		double	q3 = q * q * q;
		q3 = acos(r / sqrt(q3));
		roots.val[0] = -t1 + r13 * cos(q3 / 3.);
		roots.val[1] = -t1 + r13 * cos((q3 + (2. * M_PI)) / 3.);
		roots.val[2] = -t1 + r13 * cos((q3 + (4. * M_PI)) / 3.);
	}
	else
	{
		r13 = r < 0 ? - powf(-r, 1. / 3.) : powf(r, 1. / 3.);
		roots.val[0] = -t1 + 2. * r13;
		roots.val[1] = -r13 - t1;
		roots.val[2] = -r13 - t1;// SAME
	}
	return (roots);
}
