/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_light_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aplat <aplat@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/18 05:38:09 by aplat             #+#    #+#             */
/*   Updated: 2020/05/18 05:38:22 by aplat            ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

void	cel_shade(double *val)
{
	if (*val < CEL_BOUND_1)
		*val = 0.;
	else if (*val < CEL_BOUND_2)
		*val = CEL_BOUND_1;
	else if (*val < CEL_BOUND_3)
		*val = CEL_BOUND_2;
	else if (*val < CEL_BOUND_4)
		*val = CEL_BOUND_3;
	else if (*val < CEL_BOUND_6)
		*val = CEL_BOUND_4;
	else
		*val = CEL_BOUND_6;
}

void	clamp_transparency(t_3vecf *transp_fact)
{
	if (transp_fact->val[0] < 0)
		transp_fact->val[0] = 0;
	if (transp_fact->val[1] < 0)
		transp_fact->val[1] = 0;
	if (transp_fact->val[2] < 0)
		transp_fact->val[2] = 0;
}
