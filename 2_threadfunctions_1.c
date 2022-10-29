/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   2_threadfunctions_1.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wmardin <wmardin@student.42wolfsburg.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/24 22:06:50 by wmardin           #+#    #+#             */
/*   Updated: 2022/10/29 15:07:35 by wmardin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	*philosopher(void *arg)
{
	t_philo		*p;

	p = (t_philo *) arg;
	if (!wait_timetarget(p->common->starttime, p))
		return (NULL);
	if (p->id % 2 == 0)
		usleep(200);
	if (p->common->times_to_eat != -1)
	{
		while (p->times_eaten < p->common->times_to_eat && !p->common->stop)
		{
			take_fork_right(p);
			take_fork_left(p);
			eat_sleep_think(p);
		}
	}
	else
	{
		while (!p->common->stop)
		{
			take_fork_right(p);
			take_fork_left(p);
			eat_sleep_think(p);
		}
	}
	return (NULL);
}

void	wait_start(t_philo *p)
{
	while (get_time_ms() < p->common->starttime)
		usleep(5);
	if (p->id % 2 == 0)
		usleep(200);
}

void	take_fork_right(t_philo *p)
{
	pthread_mutex_lock(p->fork_right);
	printf("%li %i has taken a fork\n",
		get_time_ms() - p->common->starttime, p->id);
}

void	take_fork_left(t_philo *p)
{
	pthread_mutex_lock(p->fork_left);
	printf("%li %i has taken a fork\n",
		get_time_ms() - p->common->starttime, p->id);
}

void	eat_sleep_think(t_philo *p)
{
	time_t		now;

	now = broadcast("is eating", p);
	p->last_eat = now;
	p->times_eaten++;
	if (!wait_activity(now + p->common->time_to_eat, p))
		return (NULL);
	pthread_mutex_unlock(p->fork_right);
	pthread_mutex_unlock(p->fork_left);
	now = broadcast("is sleeping", p);
	if (!wait_activity(now + p->common->time_to_sleep, p))
		return (NULL);
	now = broadcast("is thinking", p);
	if (!wait_activity(now + p->common->time_to_think, p))
		return (NULL);
}

bool	wait_timetarget(time_t timetarget, t_philo *p)
{
	while (get_time_ms() < timetarget && !p->common->stop)
		usleep(10);
	if (!p->common->stop)
		return (false);
	return (true);
}
