/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   texture.c                                        .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: pduhard- <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2020/01/19 17:18:27 by pduhard-     #+#   ##    ##    #+#       */
/*   Updated: 2020/01/21 09:47:22 by pduhard-    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "rt.h"

t_3vecf	get_uni_color(t_3vecf inter_point, t_3vecf normal_inter, t_obj *obj)
{
	return (obj->text.color_1);
	(void)inter_point; // warning
	(void)normal_inter; // warning
}

t_2vecf		get_random_gradient(const int permutation[512], const t_2vecf gradient[8], int x, int y)
{
	return (gradient[permutation[abs(x) + permutation[abs(y)]]]);
}
double	linear_interpolate(double a, double b, double val)
{
	return (a + (b - a) * val);
}

double	quintic_poly(float val)
{
	return (val * val * val * (val * (val * 6.0 - 15.0) + 10.0));
}

t_3vecf	get_perlin_color(t_3vecf inter_point, t_3vecf normal_inter, t_obj *obj)
{
	const int permutation[512] = {151,160,137,91,90,15,131,13,201,95,96,53,194,233,7,225,140,
		36,103,30,69,142,8,99,37,240,21,10,23,190,6,148,247,120,234,
		75,0,26,197,62,94,252,219,203,117,35,11,32,57,177,33,88,237,
		149,56,87,174,20,125,136,171,168,68,175,74,165,71,134,139,
		48,27,166,77,146,158,231,83,111,229,122,60,211,133,230,220,
		105,92,41,55,46,245,40,244,102,143,54,65,25,63,161,1,216,80,
		73,209,76,132,187,208,89,18,169,200,196,135,130,116,188,159,
		86,164,100,109,198,173,186,3,64,52,217,226,250,124,123,5,
		202,38,147,118,126,255,82,85,212,207,206,59,227,47,16,58,17,
		182,189,28,42,223,183,170,213,119,248,152,2,44,154,163,70,
		221,153,101,155,167,43,172,9,129,22,39,253,19,98,108,110,79,
		113,224,232,178,185,112,104,218,246,97,228,251,34,242,193,
		238,210,144,12,191,179,162,241,81,51,145,235,249,14,239,107,
		49,192,214,31,181,199,106,157,184,84,204,176,115,121,50,45,
		127,4,150,254,138,236,205,93,222,114,67,29,24,72,243,141,
		128,195,78,66,215,61,156,180,151,160,137,91,90,15,131,13,201,95,96,53,194,233,7,225,140,
		36,103,30,69,142,8,99,37,240,21,10,23,190,6,148,247,120,234,
		75,0,26,197,62,94,252,219,203,117,35,11,32,57,177,33,88,237,
		149,56,87,174,20,125,136,171,168,68,175,74,165,71,134,139,
		48,27,166,77,146,158,231,83,111,229,122,60,211,133,230,220,
		105,92,41,55,46,245,40,244,102,143,54,65,25,63,161,1,216,80,
		73,209,76,132,187,208,89,18,169,200,196,135,130,116,188,159,
		86,164,100,109,198,173,186,3,64,52,217,226,250,124,123,5,
		202,38,147,118,126,255,82,85,212,207,206,59,227,47,16,58,17,
		182,189,28,42,223,183,170,213,119,248,152,2,44,154,163,70,
		221,153,101,155,167,43,172,9,129,22,39,253,19,98,108,110,79,
		113,224,232,178,185,112,104,218,246,97,228,251,34,242,193,
		238,210,144,12,191,179,162,241,81,51,145,235,249,14,239,107,
		49,192,214,31,181,199,106,157,184,84,204,176,115,121,50,45,
		127,4,150,254,138,236,205,93,222,114,67,29,24,72,243,141,
		128,195,78,66,215,61,156,180};

	const t_2vecf gradient[8] =
		{assign_2vecf(1,1), assign_2vecf(-1,1)
		, assign_2vecf(1,-1), assign_2vecf(-1,-1)
		, assign_2vecf(_SQRT_2, 0), assign_2vecf(0, _SQRT_2)
		, assign_2vecf(-_SQRT_2, 0), assign_2vecf(0, -_SQRT_2)};

	t_2vecf text_coord = obj->get_text_coordinate(inter_point, normal_inter, obj);
	t_2vecf floating_part = assign_2vecf(fmod(text_coord.val[0], 1), fmod(text_coord.val[1], 1));
	t_2vecf	int_part = assign_2vecf((int)(text_coord.val[0] - floating_part.val[0]) & 0xff, (int)(text_coord.val[1] - floating_part.val[1]) & 0xff);

	if (text_coord.val[0] < 0)
		int_part.val[0] -= 1;
	if (text_coord.val[1] < 0)
		int_part.val[1] -= 1;
	double	g00 = dot_product_2vecf(get_random_gradient(permutation, gradient, (int)int_part.val[0], (int)int_part.val[1]), floating_part);
	double	g01 = dot_product_2vecf(get_random_gradient(permutation, gradient, (int)int_part.val[0], (int)int_part.val[1] + 1), assign_2vecf(floating_part.val[0], floating_part.val[1] - 1.0));
	double	g10 = dot_product_2vecf(get_random_gradient(permutation, gradient, (int)int_part.val[0] + 1, (int)int_part.val[1]), assign_2vecf(floating_part.val[0] - 1.0, floating_part.val[1]));
	double	g11 = dot_product_2vecf(get_random_gradient(permutation, gradient, (int)int_part.val[0] + 1, (int)int_part.val[1] + 1), assign_2vecf(floating_part.val[0] - 1.0, floating_part.val[1] - 1.0));

	
	double	inter_up = linear_interpolate(g00, g01, quintic_poly(floating_part.val[1]));
	double	inter_down = linear_interpolate(g10, g11, quintic_poly(floating_part.val[1]));

	double	final_inter = linear_interpolate(inter_up, inter_down, quintic_poly(floating_part.val[0]));
	if (final_inter < 0)
		final_inter *= -1;
	return (assign_3vecf(obj->text.color_1.val[0] * final_inter, obj->text.color_1.val[1] * final_inter, obj->text.color_1.val[2] * final_inter));
}

t_3vecf	get_grid_color(t_3vecf inter_point, t_3vecf normal_inter, t_obj *obj)
{
	t_2vecf	text_coord;

	text_coord = obj->get_text_coordinate(inter_point, normal_inter, obj);
	text_coord.val[0] -= text_coord.val[0] < 0 ? (1 + fmod(text_coord.val[0], 1)) : fmod(text_coord.val[0], 1);
	text_coord.val[1] -= text_coord.val[1] < 0 ? (1 + fmod(text_coord.val[1], 1)) : fmod(text_coord.val[1], 1);
	int		u_grid;
	int		v_grid;

	u_grid = (fmod(text_coord.val[0], 2) == 0);
	v_grid = (fmod(text_coord.val[1], 2) == 0);
	if ((u_grid ^ v_grid))
		return (obj->text.color_1);
	return (obj->text.color_2);
}

int		parse_texture(char *line, int i, t_obj *obj)
{
	if (line[i] == '(')
		++i;
	while (ft_isspace(line[i]))
		++i;
	if (!(ft_strncmp(&(line[i]), "UNI", 3)))
	{
		obj->get_text_color = &get_uni_color;
		obj->text.text_type = TEXT_UNI;
		i += 3;
	}
	else if (!(ft_strncmp(&(line[i]), "GRID", 4)))
	{
		obj->get_text_color = &get_grid_color;
		obj->text.text_type = TEXT_GRID;
		i += 4;
	}
	else if (!(ft_strncmp(&(line[i]), "PERLIN", 6)))
	{
		obj->get_text_color = &get_perlin_color;
		obj->text.text_type = TEXT_PERLIN;
		i += 6;
	}
	else
	{
		ft_printf("Unrecognized texture\n");
		return (-1);
	}
	while (ft_isspace(line[i]))
		++i;
	if (line[i] != '(' || (i = parse_3vecf(line, i, &(obj->text.color_1))) == -1)
	{
		return (-1);
	}
	while (ft_isspace(line[i]))
		++i;
	if (line[i] != '(' || (i = parse_3vecf(line, i, &(obj->text.color_2))) == -1)
	{
		return (-1);
	}
	while (ft_isspace(line[i]))
		++i;
	if (line[i] != '(' || (i = parse_3vecf(line, i, &(obj->text.color_3))) == -1)
	{
		return (-1);
	}
	while (ft_isspace(line[i]))
		++i;
	if (line[i] == ')')
		++i;
	obj->color = obj->text.color_1;
	return (i);
}
