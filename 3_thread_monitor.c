/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   3_thread_monitor.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wmardin <wmardin@student.42wolfsburg.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/29 15:37:17 by wmardin           #+#    #+#             */
/*   Updated: 2022/10/30 19:05:27 by wmardin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

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

/*
pthread_mutex_lock(e->philostructs[i].last_eat_lock);
			if (e->philostructs[i].last_eat + e->common.time_to_die < get_time_ms())
			{
				pthread_mutex_lock(&e->stop_lock);
				e->common.stop = true;
				pthread_mutex_unlock(&e->stop_lock);
				printf("%li %i has died\n", get_time_ms() - e->common.starttime,
					e->philostructs[i].id);
				break ;
			}
*/
bool	check_death(t_envl *e, int i)
{
	bool	dieded;

	dieded = false;
	pthread_mutex_lock(e->philostructs[i].last_eat_lock);
	if (e->philostructs[i].last_eat + e->common.time_to_die < get_time_ms())
	{
		set_stop(e);
		printf("%li %i has died\n",
			get_time_ms() - e->common.starttime, e->philostructs[i].id);
		dieded = true;
	}
	pthread_mutex_unlock(e->philostructs[i].last_eat_lock);
	return (dieded);
}

/*
if (e->common.times_to_eat != -1)
			{
				if (e->philostructs[i].times_eaten < e->common.times_to_eat)
					eatenenough = false;
			}
			else
				eatenenough = false;
			pthread_mutex_unlock(e->philostructs[i].last_eat_lock);
*/
bool	check_sated(t_envl *e, int i)
{
	bool	sated;

	if (e->common.times_to_eat == -1)
		return (false);
	pthread_mutex_lock(e->philostructs[i].last_eat_lock);
	sated = e->philostructs[i].times_eaten >= e->common.times_to_eat;
	pthread_mutex_unlock(e->philostructs[i].last_eat_lock);
	return (sated);
}

void	set_stop(t_envl *e)
{
	pthread_mutex_lock(&e->common.stoplock);
	e->common.stop = true;
	pthread_mutex_unlock(&e->common.stoplock);
}
