/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   3_thread_monitor.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wmardin <wmardin@student.42wolfsburg.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/29 15:37:17 by wmardin           #+#    #+#             */
/*   Updated: 2022/10/29 16:29:16 by wmardin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	*monitor(void *arg)
{
	t_envl	*e;
	int		i;

	e = (t_envl *)arg;
	while (!e->common.stop)
	{
		i = 0;
		while (i < e->n_philosophers)
		{
			if (e->philostructs[i].last_eat + e->common.time_to_die < get_time_ms())
			{
				e->common.stop = true;
				printf("%li %i has died\n", get_time_ms() - e->common.starttime,
					e->philostructs[i].id);
				break ;
			}
			i++;
		}
	}
	return (NULL);
}
