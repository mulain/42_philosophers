/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   2_thread_philosopher.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wmardin <wmardin@student.42wolfsburg.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/24 22:06:50 by wmardin           #+#    #+#             */
/*   Updated: 2022/10/29 16:29:27 by wmardin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	*philosopher(void *arg)
{
	t_philo		*p;

	p = (t_philo *) arg;
	if (wait_timetarget(p->common->starttime, p))
		return (NULL);
	if (p->id % 2 == 0)
		usleep(200);
	while (!p->common->stop)
	{
		take_forks(p);
		eat_sleep_think(p);
	}
	return (NULL);
}

void	take_forks(t_philo *p)
{
	pthread_mutex_lock(p->fork_right);
	broadcast("has taken a fork", p);
	pthread_mutex_lock(p->fork_left);
	broadcast("has taken a fork", p);
}

/*
Doesn't actually have to return a variable, just has to return at the
checkpoints. Kept it for now, might delete to void later.
wait_timetarget returns the value of the bool "stopped". So if it is true,
the simulation has stopped and the routine must be canceled.
*/
void	eat_sleep_think(t_philo *p)
{
	time_t		now;
	bool		stopped;

	if (p->common->stop)
	{
		release_forks(p);
		return ;
	}
	now = broadcast("is eating", p);
	p->last_eat = now;
	p->times_eaten++;
	stopped = wait_timetarget(now + p->common->time_to_eat, p);
	release_forks(p);
	if (stopped)
		return ;
	now = broadcast("is sleeping", p);
	if (wait_timetarget(now + p->common->time_to_sleep, p))
		return ;
	now = broadcast("is thinking", p);
	if (wait_timetarget(now + p->common->time_to_think, p))
		return ;
}

void	release_forks(t_philo *p)
{
	pthread_mutex_unlock(p->fork_right);
	pthread_mutex_unlock(p->fork_left);
}
