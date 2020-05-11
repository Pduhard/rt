/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   matrix.c                                         .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: pduhard- <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2019/12/21 22:36:28 by pduhard-     #+#   ##    ##    #+#       */
/*   Updated: 2020/02/25 20:43:46 by pduhard-         ###   ########lyon.fr   */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */
#include "rt.h"

t_3vecf	mult_3vecf_33matf(t_3vecf vect, t_33matf mat)
{
	t_3vecf	mult;

	mult.val[0] = vect.val[0] * mat.val[0][0]
							+ vect.val[1] * mat.val[0][1]
							+ vect.val[2] * mat.val[0][2];
	mult.val[1] = vect.val[0] * mat.val[1][0]
							+ vect.val[1] * mat.val[1][1]
							+ vect.val[2] * mat.val[1][2];
	mult.val[2] = vect.val[0] * mat.val[2][0]
							+ vect.val[1] * mat.val[2][1]
							+ vect.val[2] * mat.val[2][2];
	return (mult);
}

t_33matf	mult_33matf_33matf(t_33matf a, t_33matf b)
{
	t_33matf	mult;
	int		i;
	int		j;

	i = 0;
	while (++i < 3)
	{
		j = 0;
		while (++j < 3)
			mult.val[i][j] = a.val[i][0] * b.val[0][i]\
										 + a.val[i][1] * b.val[1][i]\
										 + a.val[i][2] * b.val[2][i];
	  i++;
	}
	return (mult);
}

t_33matf	init_rotation_matrix_x(double theta)
{
	t_33matf	mat;

	mat.val[0][0] = 1;
	mat.val[0][1] = 0;
	mat.val[0][2] = 0;
	mat.val[1][0] = 0;
	mat.val[1][1] = cos(theta);
	mat.val[1][2] = -sin(theta);
	mat.val[2][0] = 0;
	mat.val[2][1] = sin(theta);
	mat.val[2][2] = cos(theta);
	return (mat);
}

t_33matf	init_rotation_matrix_y(double theta)
{
	t_33matf	mat;

	mat.val[0][0] = cos(theta);
	mat.val[0][1] = 0;
	mat.val[0][2] = sin(theta);
	mat.val[1][0] = 0;
	mat.val[1][1] = 1;
	mat.val[1][2] = 0;
	mat.val[2][0] = -sin(theta);
	mat.val[2][1] = 0;
	mat.val[2][2] = cos(theta);
	return (mat);
}

t_33matf	init_rotation_matrix_z(double theta)
{
	t_33matf	mat;

	mat.val[0][0] = cos(theta);
	mat.val[0][1] = -sin(theta);
	mat.val[0][2] = 0;
	mat.val[1][0] = sin(theta);
	mat.val[1][1] = cos(theta);
	mat.val[1][2] = 0;
	mat.val[2][0] = 0;
	mat.val[2][1] = 0;
	mat.val[2][2] = 1;
	return (mat);
}

t_33matf	init_rotation_matrix_vec(t_3vecf vec, double theta)
{
	t_33matf	mat;
	double		c = cos(theta);
	double		s = sin(theta);

	mat.val[0][0] = vec.val[0] * vec.val[0] * (1 - c) + c;
	mat.val[0][1] = vec.val[0] * vec.val[1] * (1 - c) - vec.val[2] * s;
	mat.val[0][2] = vec.val[0] * vec.val[2] * (1 - c) + vec.val[1] * s;

	mat.val[1][0] = vec.val[0] * vec.val[1] * (1 - c) + vec.val[2] * s;
	mat.val[1][1] = vec.val[1] * vec.val[1] * (1 - c) + c;
	mat.val[1][2] = vec.val[1] * vec.val[2] * (1 - c) - vec.val[0] * s;

	mat.val[2][0] = vec.val[0] * vec.val[2] * (1 - c) - vec.val[1] * s;
	mat.val[2][1] = vec.val[1] * vec.val[2] * (1 - c) + vec.val[0] * s;
	mat.val[2][2] = vec.val[2] * vec.val[2] * (1 - c) + c;
	return (mat);
}
