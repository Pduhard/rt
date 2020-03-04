#include "rt.h"

void	add_object(t_obj *obj, t_data *data)
{
	if (obj->material_type == MAT_DIFFUSE)
	{
		if (data->objs)
			obj->next = data->objs;
		else
			obj->next = NULL;
		data->objs = obj;
	}
	else if (obj->material_type == MAT_NEGATIVE)
	{
		if (data->negative_objs)
			obj->next = data->negative_objs;
		else
			obj->next = NULL;
		data->negative_objs = obj;
	}
}

void	add_component(t_obj *obj, t_composed *composed)
{
	t_obj	**obj_tab;
	int		i;

	i = 0;
	while (composed->components && composed->components[i])
		++i;
	if (!(obj_tab = malloc(sizeof(t_obj *) * (i + 2))))
		return ;
	obj_tab[i + 1] = NULL;
	if (composed->components)
		while (i > 0)
		{
			obj_tab[i] = composed->components[i - 1];
			--i;
		}
	obj_tab[0] = obj;
	free(composed->components);
	composed->components = obj_tab;
}

void	push_object(t_obj *obj, int composed, t_data *data, t_composed *from)
{
	if (composed)
	{
		//free object
	}
	else
	{
		if (!from)
			add_object(obj, data);
		else
			add_component(obj, from);
	}
}
