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
/*
void	mult_vec_matrix(t_3vecf vect, t_44matf mat, t_3vecf *dst)
{ 
	double	a;
	double	b;
	double	c;
	double	w;

	a = vect.val[0] * mat.val[0][0] + vect.val[1] * mat.val[1][0] + vect.val[2] * mat.val[2][0] + mat.val[3][0];
	b = vect.val[0] * mat.val[0][1] + vect.val[1] * mat.val[1][1] + vect.val[2] * mat.val[2][1] + mat.val[3][1];
	c = vect.val[0] * mat.val[0][2] + vect.val[1] * mat.val[1][2] + vect.val[2] * mat.val[2][2] + mat.val[3][2];
	w = vect.val[0] * mat.val[0][3] + vect.val[1] * mat.val[1][3] + vect.val[2] * mat.val[2][3] + mat.val[3][3];

	dst->val[0] = a / w;
	dst->val[1] = b / w;
	dst->val[2] = c / w;
} 
*/

t_44matf	mult_44matf_44matf(t_44matf a, t_44matf b)
{
	t_44matf	mult;
	int		i = -1;
	int		j;
	while (++i < 4 && (j = -1))
		while (++j < 4)
			mult.val[i][j] = a.val[i][0] * b.val[0][i] + a.val[i][1] * b.val[1][i] + a.val[i][2] * b.val[2][i] + a.val[i][3] * b.val[3][i];
//	mult.val[1] = vect.val[0] * mat.val[1][0] + vect.val[1] * mat.val[1][1] + vect.val[2] * mat.val[1][2];
//	mult.val[2] = vect.val[0] * mat.val[2][0] + vect.val[1] * mat.val[2][1] + vect.val[2] * mat.val[2][2];
	return (mult);
}

t_44matf	build_translation_matrix(t_3vecf point, t_3vecf	x_axis, t_3vecf y_axis, t_3vecf z_axis)
{
	t_44matf	trans_mat;
	t_44matf	b_mat;

	ft_bzero(&trans_mat, sizeof(t_44matf));
	ft_bzero(&b_mat, sizeof(t_44matf));
	trans_mat.val[0][0] = 1;
	trans_mat.val[1][1] = 1;
	trans_mat.val[2][2] = 1;
	trans_mat.val[3][3] = 1;
	trans_mat.val[3][0] = point.val[0];
	trans_mat.val[3][1] = point.val[1];
	trans_mat.val[3][2] = point.val[2];

	b_mat.val[0][0] = x_axis.val[0];
	b_mat.val[1][0] = x_axis.val[1];
	b_mat.val[2][0] = x_axis.val[2];

	b_mat.val[0][1] = y_axis.val[0];
	b_mat.val[1][1] = y_axis.val[1];
	b_mat.val[2][1] = y_axis.val[2];

	b_mat.val[0][2] = z_axis.val[0];
	b_mat.val[1][2] = z_axis.val[1];
	b_mat.val[2][2] = z_axis.val[2];
	b_mat.val[3][3] = 1;

	trans_mat = mult_44matf_44matf(trans_mat, b_mat);

	ft_bzero(&b_mat, sizeof(t_44matf));
	b_mat.val[0][0] = get_length_3vecf(x_axis);
	b_mat.val[1][1] = get_length_3vecf(y_axis);
	b_mat.val[2][2] = get_length_3vecf(z_axis);
	b_mat.val[3][3] = 1;

	trans_mat = mult_44matf_44matf(trans_mat, b_mat);
	int		i = -1;
	int		j;
	while (++i < 4 && (j = -1))
	{
		while (++j < 4)
			printf("%f ", trans_mat.val[i][j]);
		printf("\n");
	}
	return (trans_mat);
}



t_3vecf	mult_3vecf_33matf(t_3vecf vect, t_33matf mat)
{
	t_3vecf	mult;

	mult.val[0] = vect.val[0] * mat.val[0][0] + vect.val[1] * mat.val[0][1] + vect.val[2] * mat.val[0][2];
	mult.val[1] = vect.val[0] * mat.val[1][0] + vect.val[1] * mat.val[1][1] + vect.val[2] * mat.val[1][2];
	mult.val[2] = vect.val[0] * mat.val[2][0] + vect.val[1] * mat.val[2][1] + vect.val[2] * mat.val[2][2];
	return (mult);
}

t_33matf	mult_33matf_33matf(t_33matf a, t_33matf b)
{
	t_33matf	mult;
	int		i = -1;
	int		j;
	while (++i < 3 && (j = -1))
		while (++j < 3)
			mult.val[i][j] = a.val[i][0] * b.val[0][i] + a.val[i][1] * b.val[1][i] + a.val[i][2] * b.val[2][i];
//	mult.val[1] = vect.val[0] * mat.val[1][0] + vect.val[1] * mat.val[1][1] + vect.val[2] * mat.val[1][2];
//	mult.val[2] = vect.val[0] * mat.val[2][0] + vect.val[1] * mat.val[2][1] + vect.val[2] * mat.val[2][2];
	return (mult);
}

/*
void	mult_dir_matrix(t_3vecf vect, t_44matf mat, t_3vecf *dst)
{ 
	dst->val[0] = vect.val[0] * mat.val[0][0] + vect.val[1] * mat.val[1][0] + vect.val[2] * mat.val[2][0];
	dst->val[1] = vect.val[0] * mat.val[0][1] + vect.val[1] * mat.val[1][1] + vect.val[2] * mat.val[2][1];
	dst->val[2] = vect.val[0] * mat.val[0][2] + vect.val[1] * mat.val[1][2] + vect.val[2] * mat.val[2][2];
} 
*/
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

void		init_light_to_world_matrix(double mat[4][4])
{
	mat[0][0] = 11.146836;
	mat[0][1] = -5.781569;
	mat[0][2] = -0.0605886;
	mat[0][3] = 0;
	mat[1][0] = -0.199534;
	mat[1][1] = -3.543982;
	mat[1][2] = -11.89;
	mat[1][3] = 0;
	mat[2][0] = 5.459;
	mat[2][1] = 10.568624;
	mat[2][2] = -4.0220;
	mat[2][3] = 0;
	mat[3][0] = 0;
	mat[3][1] = 0;
	mat[3][2] = 0;
	mat[3][3] = 1;
}

void		init_camera_to_world_matrix(double mat[4][4])
{
	mat[0][0] = 0.945519;
	mat[0][1] = 0;
	mat[0][2] = -0.325569;
	mat[0][3] = 0;
	mat[1][0] = -0.179534;
	mat[1][1] = 0.834209;
	mat[1][2] = -0.521403;
	mat[1][3] = 0;
	mat[2][0] = 0.271593;
	mat[2][1] = 0.551447;
	mat[2][2] = 0.78876;
	mat[2][3] = 0;
	mat[3][0] = 4.208271;
	mat[3][1] = 8.374532;
	mat[3][2] = 17.932925;
	mat[3][3] = 1;
}
