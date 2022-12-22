/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   3_thread_monitor.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wmardin <wmardin@student.42wolfsburg.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/29 15:37:17 by wmardin           #+#    #+#             */
/*   Updated: 2022/12/22 18:23:35 by wmardin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

/*
Googled a neat solution for testing if only one element of a group doesn't
conform (here: isn't true):
Set return value to true, loop through group, only modify return value
if checked element is false. That way only returns true if all values are
true. Returns false if only one value is false.
*/
void	*monitor(void *arg)
{
	t_envl	*e;
	bool	all_sated;
	int		i;

	e = (t_envl *)arg;
	while (1)
	{
		all_sated = true;
		i = 0;
		while (i < e->n_philosophers)
		{
			if (check_death(e, i))
				return (NULL);
			if (!check_sated(e, i))
				all_sated = false;
			i++;
		}
		if (all_sated)
		{
			set_stop(e);
			return (NULL);
		}
		usleep(1000);
	}
}

bool	check_death(t_envl *e, int i)
{
	bool	dieded;

	dieded = false;
	pthread_mutex_lock(e->philo[i].last_eat_lock);
	if (get_time_ms() - e->philo[i].last_eat > e->global.time_to_die)
	{
		set_stop(e);
		printf("%li %i died\n",
			get_time_ms() - e->global.starttime, e->philo[i].id);
		dieded = true;
	}
	pthread_mutex_unlock(e->philo[i].last_eat_lock);
	return (dieded);
}

bool	check_sated(t_envl *e, int i)
{
	bool	sated;

	if (e->global.times_to_eat == -1)
		return (false);
	pthread_mutex_lock(e->philo[i].last_eat_lock);
	sated = e->philo[i].times_eaten >= e->global.times_to_eat;
	pthread_mutex_unlock(e->philo[i].last_eat_lock);
	return (sated);
}

void	set_stop(t_envl *e)
{
	pthread_mutex_lock(&e->global.stoplock);
	e->global.stop = true;
	pthread_mutex_unlock(&e->global.stoplock);
}
