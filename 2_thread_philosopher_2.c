/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   2_thread_philosopher_2.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wmardin <wmardin@student.42wolfsburg.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/24 22:06:50 by wmardin           #+#    #+#             */
/*   Updated: 2022/10/31 12:59:28 by wmardin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	switch_forks(t_philo *p)
{
	pthread_mutex_t		*temp;

	temp = p->fork_left;
	p->fork_left = p->fork_right;
	p->fork_right = temp;
}

void	take_forks(t_philo *p)
{
	pthread_mutex_lock(p->fork_right);
	broadcast("has taken a fork", p);
	pthread_mutex_lock(p->fork_left);
	broadcast("has taken a fork", p);
}

void	release_forks(t_philo *p)
{
	pthread_mutex_unlock(p->fork_right);
	pthread_mutex_unlock(p->fork_left);
}

bool	check_stopped(t_philo *p)
{
	bool	stopped;

	pthread_mutex_lock(&p->common->stoplock);
	stopped = p->common->stop;
	pthread_mutex_unlock(&p->common->stoplock);
	return (stopped);
}

/*
printf itself doesn't require a mutex guard in order for its printing to not be
interrupted by another print. The mutex is to prevent a call with a higher
timestamp from being printed before a call with a lower timestamp.
Only prints if the simulation hasn't been stopped. Allowed functions don't permit
killing of threads, so they have to be prevented from printing before they can
themselves check whether the sim has stopped and terminate of their own accord.
*/
time_t	broadcast(char *msg, t_philo *p)
{
	time_t		now;

	pthread_mutex_lock(&p->common->printlock);
	now = get_time_ms();
	if (!check_stopped(p))
		printf("%li %i %s\n", now - p->common->starttime, p->id, msg);
	pthread_mutex_unlock(&p->common->printlock);
	return (now);
}
