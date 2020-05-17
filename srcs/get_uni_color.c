#include "rt.h"

t_4vecf	get_uni_color(t_3vecf inter_point, t_3vecf normal_inter, t_obj *obj)
{
	t_text_proc	*text;

	text = (t_text_proc *)obj->text.text_param;
	return (text->color[0]);
	(void)inter_point;
	(void)normal_inter;
}
