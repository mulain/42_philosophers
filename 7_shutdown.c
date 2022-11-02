/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   7_shutdown.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wmardin <wmardin@student.42wolfsburg.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/24 11:40:17 by wmardin           #+#    #+#             */
/*   Updated: 2022/11/02 13:12:38 by wmardin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	shutdown(t_envl *e)
{
	int		i;

	if (e->mutex_init)
	{
		pthread_mutex_destroy(&e->common.printlock);
		pthread_mutex_destroy(&e->common.stoplock);
		i = 0;
		while (i < e->n_philosophers)
		{
			pthread_mutex_destroy(&e->forks[i]);
			pthread_mutex_destroy(&e->last_eat_locks[i]);
			i++;
		}
	}
	if (e->forks)
		free(e->forks);
	if (e->last_eat_locks)
		free(e->last_eat_locks);
	if (e->threads)
		free(e->threads);
	if (e->philo)
		free(e->philo);
}
