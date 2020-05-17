
#include "rt.h"

int		check_mov_key_press(int keycode, t_data *data)
{
	if (keycode == A_KEY)
		data->hooks |= A_HOOK;
	else if (keycode == S_KEY)
		data->hooks |= S_HOOK;
	else if (keycode == D_KEY)
		data->hooks |= D_HOOK;
	else if (keycode == W_KEY)
		data->hooks |= W_HOOK;
	else if (keycode == SPACE_KEY)
		data->hooks |= SPACE_HOOK;
	else if (keycode == SHIFT_KEY)
		data->hooks |= SHIFT_HOOK;
	else
		return (0);
	return (1);
}

int		check_rot_key_press(int keycode, t_data *data)
{
	if (keycode == ARR_LEFT_KEY)
		data->hooks |= ARR_LEFT_HOOK;
	else if (keycode == ARR_RIGHT_KEY)
		data->hooks |= ARR_RIGHT_HOOK;
	else if (keycode == ARR_DOWN_KEY)
		data->hooks |= ARR_DOWN_HOOK;
	else if (keycode == ARR_UP_KEY)
		data->hooks |= ARR_UP_HOOK;
	else
		return (0);
	return (1);
}
