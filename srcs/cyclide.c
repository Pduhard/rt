/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   cyclide.c                                        .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: pduhard- <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2020/01/31 18:29:04 by pduhard-     #+#   ##    ##    #+#       */
/*   Updated: 2020/02/28 23:53:46 by pduhard-         ###   ########lyon.fr   */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "rt.h"

int		check_inside_cyclide(t_3vecf point, t_obj *cyclide)
{
	t_cyclide	*param;
	double	v;
	double	u;
//	double	sin_v_2;

	return (0);
	/* ne need to do*/
	param = (t_cyclide *)cyclide->obj_param;
	v = atan2(point.val[1], point.val[0]);
	u = (point.val[0]);
	point.val[2] -= u + v;

	if (is_null(point.val[0] * point.val[0] + point.val[1] * point.val[1] + point.val[2] * point.val[2]))
		return (1);
	return (0);
}

t_2vecf	get_text_coordinate_cyclide(t_3vecf inter_point, t_3vecf normal_inter, t_obj *cyclide)
{
	t_2vecf	text_coord;
//	double	sin_v_2;
	t_cyclide	*param;

	param = (t_cyclide *)cyclide->obj_param;
	text_coord.val[1] = atan2(inter_point.val[1], inter_point.val[0]);
/*	if (!is_null((sin_v_2 = sin(text_coord.val[1] / 2))))
		text_coord.val[0] = inter_point.val[2] / sin_v_2;
	else if (!is_null(text_coord.val[1]))
		text_coord.val[0] = (inter_point.val[0] / cos(text_coord.val[1]) - param->radius) / cos(text_coord.val[1] / 2);
	else
		text_coord.val[0] = (inter_point.val[0] - param->radius);
*/
/* FALSE need to get from parametric equation*/

	//printf("%f %f\n", text_coord.val[0], text_coord.val[1]);
	text_coord.val[0] = 0 * (text_coord.val[0]+  1) / (2 * 1);
	text_coord.val[1] = 0 * M_PI * 2;
	return (text_coord);
	(void)normal_inter;
}

void	move_cyclide(t_obj *cyclide, t_3vecf dir, double fact)
{
	t_cyclide	*param;
	t_cut		*cuts;

	param = (t_cyclide *)cyclide->obj_param;
	param->origin.val[0] += dir.val[0] * fact;
	param->origin.val[1] += dir.val[1] * fact;
	param->origin.val[2] += dir.val[2] * fact;
	cuts = cyclide->cuts;
	while (cuts)
	{
		if (cuts->move && cuts->cut_type != CUT_STATIC)
			cuts->move(cuts, dir, fact);
		cuts = cuts->next;
	}
}

t_3vecf	get_origin_cyclide(t_obj *cyclide)
{
	return (((t_cyclide *)cyclide->obj_param)->origin);
}

t_3vecf	get_normal_intersect_cyclide(t_3vecf inter_point, t_obj *cyclide, int sp_id)
{
	t_cyclide	*param;
	t_3vecf		normal_inter;
	double		x;
	double		y;
	double		z;
	t_3vecf		cyclide_origin;

	param = (t_cyclide *)cyclide->obj_param;
	cyclide_origin = sp_id ? move_3vecf(param->origin, cyclide->motions, sp_id) : param->origin;
	x = inter_point.val[0] - cyclide_origin.val[0];
	y = inter_point.val[1] - cyclide_origin.val[1];
	z = inter_point.val[2] - cyclide_origin.val[2];
	double	a = param->param;
	normal_inter.val[0] = 2 * x * z + 2 * a * x;
	normal_inter.val[1] = 2 * y * z - 2 * a * y;
	normal_inter.val[2] = x * x + y * y + 3 * z * z - a * a;
	normalize_3vecf(&normal_inter);
	return (normal_inter);
}

int	ray_intersect_cyclide(t_3vecf orig, t_3vecf dir, t_obj *cyclide, double *dist, double min_dist, double max_dist, int sp_id)
{
	t_cyclide	*param;
	double		ox;
	double		oy;
	double		oz;
	double		dx;
	double		dy;
	double		dz;
	t_4vecf		t_fact;
	t_3vecf		roots;
	int			check;
	t_3vecf		cyclide_origin;

	(void)sp_id;
	check = 0;
	param = (t_cyclide *)cyclide->obj_param;
	cyclide_origin = sp_id ? move_3vecf(param->origin, cyclide->motions, sp_id) : param->origin;
	ox = orig.val[0] - cyclide_origin.val[0];
	oy = orig.val[1] - cyclide_origin.val[1];
	oz = orig.val[2] - cyclide_origin.val[2];
	dx = dir.val[0];
	dy = dir.val[1];
	dz = dir.val[2];

	double	a = param->param;
	t_fact.val[0] = (dx * dx * dz) + (dy * dy * dz) + (dz * dz * dz);//a
	t_fact.val[1] = (oz * dx * dx) + (2 * ox * dx * dz) + (oz * dy * dy) + (2 * oy * dz * dy) + (3 * oz * dz * dz) + (a * dx * dx) - (a * dy * dy);//b
	t_fact.val[2] = (2 * ox * oz * dx) + (ox * ox * dz) + (2 * oy * oz * dy) + (oy * oy * dz) + (3 * oz * oz * dz) - (a * a * dz) + (2 * a * ox * dx) - (2 * a * oy * dy);//c
	t_fact.val[3] = (ox * ox * oz) + (oy * oy * oz) + (oz * oz * oz) - (a * a * oz) + (a * ox * ox) - (a * oy * oy);//d

	roots = solve_cubic(t_fact.val[0], t_fact.val[1], t_fact.val[2], t_fact.val[3]);
	int		i = -1;
//	printf("%f %f %f\n", roots.val[0], roots.val[1], roots.val[2]);
	while (++i < 3)
	{
	//	t_3vecf	coord;
	//	t_3vecf	coord;
	//	coord.val[0] = orig.val[0] + dir.val[0] * roots.val[i] - cyclide_origin.val[0];
	//	coord.val[1] = orig.val[1] + dir.val[1] * roots.val[i] - cyclide_origin.val[1];
	//	coord.val[2] = orig.val[2] + dir.val[2] * roots.val[i] - cyclide_origin.val[2];
		double	x, y, z;
		x = ox + dx * roots.val[i];
		y = oy + dy * roots.val[i];
		z = oz + dz * roots.val[i];
		if (x > -1.5 && x < 1.5 && y > -2 && y < 2 && z > -1 && z < 1 && roots.val[i] < *dist && roots.val[i] > min_dist && roots.val[i] < max_dist)
		{
			check = 1;
			*dist = roots.val[i];
		}
	}
	return (check);
}

void	generate_new_cyclide(t_data *data)
{
	t_obj		*cyclide;
	t_cyclide	*param;
	t_3vecf		dir;

	dir = mult_3vecf_33matf(mult_3vecf_33matf(window_to_view(0, 0, data->size.val[0], data->size.val[1]), data->rot_mat[1]), data->rot_mat[0]);
	normalize_3vecf(&dir);
	if (!(cyclide = ft_memalloc(sizeof(t_obj))))
		return ;
	if (!(param = ft_memalloc(sizeof(t_cyclide))))
		return ;
	param->origin.val[0] = data->camera->origin.val[0] + dir.val[0] * 2;
	param->origin.val[1] = data->camera->origin.val[1] + dir.val[1] * 2;
	param->origin.val[2] = data->camera->origin.val[2] + dir.val[2] * 2;
	param->param = (get_random_number((time(NULL) * 0xcacacaca) << 3) - 0.5) * 2;
	// param->y_fact = get_random_number((time(NULL) * 0xabcdef99) << 4) * 2.5;
	// param->z_fact = get_random_number((time(NULL) * 0xff3672ff) << 3) * 2.5;

	//param->normal = assign_3vecf(get_random_number((time(NULL) * 0xcacacaca) << 16) - 0.5, get_random_number((time(NULL) * 0xfeabcdef) << 8) - 0.5, get_random_number((time(NULL) * 0x1056ffe) << 4) - 0.5);
	// normalize_3vecf(&param->normal);

/*	while (!is_null(dot_product_3vecf(param->normal, param->x2d_axis)))
	{
		param->x2d_axis = assign_3vecf(get_random_number(rd * 0xcacacaca << 16) - 0.5, get_random_number(rd * 0xfeabcdef << 8) - 0.5, get_random_number(rd * 0x1056ffe << 4) - 0.5);
		rd *= time(NULL);
		normalize_3vecf(&param->x2d_axis);
		printf("asad\n");
	} */
	cyclide->obj_param = param;
	cyclide->obj_type = OBJ_CYCLIDE;
	cyclide->check_inside = &check_inside_cyclide;
	cyclide->ray_intersect = &ray_intersect_cyclide;
	cyclide->get_normal_inter = &get_normal_intersect_cyclide;
	cyclide->get_origin = &get_origin_cyclide;
	cyclide->move = &move_cyclide;
	cyclide->rotate = NULL;
	cyclide->get_text_coordinate = &get_text_coordinate_cyclide;
	cyclide->get_text_color = &get_uni_color;
	cyclide->text = generate_random_texture();
	set_bump_own(cyclide);
	add_object(cyclide, data);
	data->new_obj = 1;
}
/*int		parse_cyclide(char *line, t_data *data)
{
	int			i;
	t_obj		*cyclide;
	t_cyclide	*cyclide_param;

	if (!(cyclide = malloc(sizeof(t_obj))) || !(cyclide_param = malloc(sizeof(t_cyclide))))
		return (0);
	i = 6;
	while (ft_isspace(line[i]))
		++i;
	if (line[i] != '(' || (i = parse_3vecf(line, i, &cyclide_param->origin)) == -1)
	{
		ft_printf("Syntax error: cyclide syntax: cyclide(origin)(radius)(color)(reflection)(refraction)\n");
		return (0);
	}
	while (ft_isspace(line[i]))
		++i;
	if (line[i] != '(' || (i = parse_double(line, i, &cyclide_param->radius)) == -1)
	{
		ft_printf("Syntax error: cyclide syntax: cyclide(origin)(radius)(color)(reflection)(refraction)\n");
		return (0);
	}
	while (ft_isspace(line[i]))
		++i;
	if (line[i] != '(' || (i = parse_texture(line, i, cyclide)) == -1)
	{
		ft_printf("Syntax error: cyclide syntax: cyclide(origin)(radius)(color)(reflection)(refraction)\n");
		return (0);
	}
	while (ft_isspace(line[i]))
		++i;
	if (line[i] != '(' || (i = parse_double(line, i, &cyclide->reflection)) == -1)
	{
		ft_printf("Syntax error: cyclide syntax: cyclide(origin)(radius)(color)(reflection)(refraction)\n");
		return (0);
	}
	while (ft_isspace(line[i]))
		++i;
	if (line[i] != '(' || (i = parse_double(line, i, &cyclide->refraction)) == -1)
	{
		ft_printf("Syntax error: cyclide syntax: cyclide(origin)(radius)(color)(reflection)(refraction)\n");
		return (0);
	}

	//printf("cyclide : %f %f %f && %f && %f %f %f\n", cyclide_param->origin.val[0], cyclide_param->origin.val[1], cyclide_param->origin.val[2], cyclide_param->radius, cyclide->color.val[0], cyclide->color.val[1], cyclide->color.val[2]);
	cyclide->obj_param = cyclide_param;
	cyclide->obj_type = OBJ_cyclide;
	cyclide->ray_intersect = &ray_intersect_cyclide;
	cyclide->get_normal_inter = &get_normal_intersect_cyclide;
	cyclide->get_text_coordinate = &get_text_coordinate_cyclide;
	if (data->objs)
	{
		cyclide->next = data->objs;
	}
	else
		cyclide->next = NULL;
	data->objs = cyclide;
	return (1);
}*/
