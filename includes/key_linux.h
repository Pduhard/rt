/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   key_linux.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aplat <aplat@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/19 19:37:47 by aplat             #+#    #+#             */
/*   Updated: 2020/05/19 19:37:57 by aplat            ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef KEY_LINUX_H

# define KEY_LINUX_H

/*
** HOOKS MACRO
*/

# if defined(__linux)
#  define A_KEY 113
#  define S_KEY 115
#  define D_KEY 100
#  define W_KEY 122
#  define F_KEY 102
#  define G_KEY 103
#  define P_KEY 112
#  define ARR_LEFT_KEY 65361
#  define ARR_RIGHT_KEY 65363
#  define ARR_DOWN_KEY 65364
#  define ARR_UP_KEY 65362
#  define SPACE_KEY 32
#  define SHIFT_KEY 65505
#  define TAB_KEY 65289
#  define ALPHA_ONE_KEY 38
#  define ALPHA_TWO_KEY 233
#  define ALPHA_THREE_KEY 34
#  define ALPHA_FOUR_KEY 39
#  define ALPHA_FIVE_KEY 40
#  define ALPHA_SIX_KEY 45
#  define ALPHA_SEVEN_KEY 232
#  define ALPHA_EIGHT_KEY 95
#  define ALPHA_NINE_KEY 231
#  define ALPHA_ZERO_KEY 224
#  define ALPHA_MINUS_KEY 41
#  define ALPHA_PLUS_KEY 61
#  define DELETE_KEY 65288
#  define SUPPR_KEY 65535
#  define IMPR_KEY 105
#  define ESC_KEY 65307
# else
#  define A_KEY 0
#  define S_KEY 1
#  define D_KEY 2
#  define W_KEY 13
#  define F_KEY 3
#  define G_KEY 5
#  define P_KEY 35
#  define ARR_LEFT_KEY 123
#  define ARR_RIGHT_KEY 124
#  define ARR_DOWN_KEY 125
#  define ARR_UP_KEY 126
#  define SPACE_KEY 49
#  define SHIFT_KEY 257
#  define TAB_KEY 48
#  define ALPHA_ONE_KEY 18
#  define ALPHA_TWO_KEY 19
#  define ALPHA_THREE_KEY 20
#  define ALPHA_FOUR_KEY 21
#  define ALPHA_FIVE_KEY 23
#  define ALPHA_SIX_KEY 22
#  define ALPHA_SEVEN_KEY 26
#  define ALPHA_EIGHT_KEY 28
#  define ALPHA_NINE_KEY 25
#  define ALPHA_ZERO_KEY 29
#  define ALPHA_MINUS_KEY 27
#  define ALPHA_PLUS_KEY 24
#  define DELETE_KEY 51
#  define SUPPR_KEY 117
#  define IMPR_KEY 105
#  define ESC_KEY 53
# endif

#endif
