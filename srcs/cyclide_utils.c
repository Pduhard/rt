#include "rt.h"

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

void	assign_cyclide_function(t_obj *cyclide)
{
	cyclide->obj_type = OBJ_CYCLIDE;
	cyclide->check_inside = &check_inside_cyclide;
	cyclide->ray_intersect = &ray_intersect_cyclide;
	cyclide->get_normal_inter = &get_normal_intersect_cyclide;
	cyclide->get_origin = &get_origin_cyclide;
	cyclide->move = &move_cyclide;
	cyclide->rotate = NULL;
	cyclide->get_text_coordinate = &get_text_coordinate_cyclide;
}

void	generate_new_cyclide(t_data *data)
{
	t_obj		*cyclide;
	t_cyclide	*param;
	t_3vecf		dir;

	dir = mult_3vecf_33matf(mult_3vecf_33matf(window_to_view(0, 0,
		data->size.val[0], data->size.val[1]), data->rot_mat[1]),
			data->rot_mat[0]);
	normalize_3vecf(&dir);
	if (!(cyclide = ft_memalloc(sizeof(t_obj))))
		return ;
	if (!(param = ft_memalloc(sizeof(t_cyclide))))
		return ;
	param->origin = add_3vecf(data->camera->origin, product_c3vecf(dir, 2));
	param->param = (get_random_number((time(NULL) * 0xcacacaca) << 3) - 0.5)
		* 2;
	cyclide->obj_param = param;
	assign_cyclide_function(cyclide);
	cyclide->text = generate_random_texture(cyclide);
	set_bump_own(cyclide);
	add_object(cyclide, data);
	data->new_obj = 1;
}

int		check_inside_cyclide(t_3vecf point, t_obj *cyclide)
{
	return (0);
	(void)point;
	(void)cyclide;
}
