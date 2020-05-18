/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_object_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aplat <aplat@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/18 05:02:43 by aplat             #+#    #+#             */
/*   Updated: 2020/05/18 05:02:43 by aplat            ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

void	clamp_and_set_dflt(t_obj *obj)
{
	clamp_val(&obj->reflection, 0, 1);
	clamp_val(&obj->shininess, 0, 1);
	clamp_val(&obj->refraction, 0, 3);
	clamp_val(&obj->refraction, 0, 2.42);
	if (obj->text.scale.val[0] == 0 && obj->text.scale.val[1] == 0)
		obj->text.scale = (t_2vecf){{0, 0}};
	if (obj->shininess > 0)
		obj->shininess = exp(11 - 10 * obj->shininess);
}

int		error_parse_object(t_obj *obj)
{
	free_object(obj);
	return (0);
}
