/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_object_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aplat <aplat@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/18 05:02:43 by aplat             #+#    #+#             */
/*   Updated: 2020/05/23 21:39:36 by aplat            ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

void	clamp_and_set_dflt(t_obj *obj)
{
	clamp_val(&obj->reflection, 0, 1);
	clamp_val(&obj->refraction, 0, 3);
	clamp_val(&obj->refraction, 0, 2.42);
	if (obj->text.scale.val[0] == 0 && obj->text.scale.val[1] == 0)
		obj->text.scale = (t_2vecf){{1, 1}};
	if (obj->shininess == -1)
		obj->shininess = 0;
	else if (obj->shininess == 0)
		obj->shininess = DEFAULT_SHININESS;
}

int		error_parse_object(t_obj *obj)
{
	free_object(obj);
	return (0);
}

void	set_shininess(double *shininess)
{
	if (*shininess < 0)
		*shininess *= -1;
	clamp_val(shininess, 0, 1);
	if (*shininess > 0)
		*shininess = exp(11 - 10 * *shininess);
	else
		*shininess = -1;
}
