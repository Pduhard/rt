#include "rt.h"

static void	push_front_motion(t_motion **root, t_motion *new)
{
	t_motion	*motion;

	clamp_val((double *)&new->spf, 0, 32);
	if (!*root)
		*root = new;
	else
	{
		motion = *root;
		while (motion->next)
			motion = motion->next;
		motion->next = new;
	}
}

static int parse_motion_element(char **line, t_motion *motion, int *ret)
{
  char		stripe;

	stripe = 0;
	*ret = 1;
  while (stripe != '>' && *ret != 0)
	{
		stripe = goto_next_element(line);
		if (!(ft_strncmp_case(*line, "dir", 3)))
			*ret = parse_origin(line, &motion->dir, 3);
		else if (!(ft_strncmp_case(*line, "speed", 3)))
			*ret = parse_double(line, 5, &motion->speed_fact);
		else if (!(ft_strncmp_case(*line, "spf", 3)))
			*ret = parse_int(line, 3, &motion->spf);
		else if (stripe == '<')
		  return (0);
	}
  return (*ret);
}

static int check_motion(t_motion *motion)
{
  if (is_null(get_length_3vecf(motion->dir)))
    ft_fdprintf(2, "Parse error motion: dir vector cannot be null\n");
  else if (is_null(motion->speed_fact))
    ft_fdprintf(2, "Parse error motion: speed factor cannot be null\n");
  else if (motion->spf <= 0)
    ft_fdprintf(2, "Parse error motion: sample per frame must be greater than 0\n");
  else
    return (1);
  return (0);
}

int			parse_motion(char **line, t_obj *obj)
{
	char		stripe;
	int			ret;
	t_motion	*motion;

	stripe = 0;
	ret = 1;
	if (!(motion = ft_memalloc(sizeof(t_motion))))
		return (0);
  if (!parse_motion_element(line, motion, &ret) ||
      !check_motion(motion))
  {
    free(motion);
    return (syn_error(SERROR, MOTION, "<spf (Value)>\n", NULL));
  }
	// while (stripe != '>' && ret != 0)
	// {
	// 	stripe = goto_next_element(line);
	// 	if (!(ft_strncmp_case(*line, "dir", 3)))
	// 		ret = parse_origin(line, &motion->dir, 3);
	// 	else if (!(ft_strncmp_case(*line, "speed", 3)))
	// 		ret = parse_double(line, 5, &motion->speed_fact);
	// 	else if (!(ft_strncmp_case(*line, "spf", 3)))
	// 		ret = parse_int(line, 3, &motion->spf);
	// 	else if (stripe == '<')
	// 	{
	// 		free(motion);
	// 		return (syn_error(SERROR, MOTION, "<spf (Value)>\n", NULL));
	// 	}
	// }
	push_front_motion(&obj->motions, motion);
	return (ret);
}
