/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   1_setup_3.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wmardin <wmardin@student.42wolfsburg.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/29 12:49:04 by wmardin           #+#    #+#             */
/*   Updated: 2022/10/29 22:55:17 by wmardin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	calc_thinktime(t_envl *e)
{
	int		freetime;

	freetime = e->common.time_to_die - e->common.time_to_eat
		- e->common.time_to_sleep;
	e->common.time_to_think = freetime * 0.5;
	if (e->common.time_to_think < 0)
		e->common.time_to_think = 0;
	printf("time_to_die:%i\n", e->common.time_to_die);
	printf("time_to_eat:%i\n", e->common.time_to_eat);
	printf("time_to_sleep:%i\n", e->common.time_to_sleep);
	printf("free time:%i\n", freetime);
	printf("thinktime:%i\n", e->common.time_to_think);
}
