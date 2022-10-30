/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   2_thread_philosopher.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wmardin <wmardin@student.42wolfsburg.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/24 22:06:50 by wmardin           #+#    #+#             */
/*   Updated: 2022/10/30 22:43:54 by wmardin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

/*
Philosopher 1 starts with a delay for the case of an uneven number of philos.
This enables the last philosopher to grab the fork of philosopher 1.

Even numbered philosophers start with a delay - but they also have their forks
switched. This is an added and more dynamic delay, dependent on the time it takes
the system to handle mutexes. If the even numbered philosphers started with the
same fork order as the odd numbered ones, the hard coded delay might not be long
enough for the odd numbered philosophers to finish locking their mutexes and then
also take the mutexes of the even numbered philosphers.
An alternative solution would be to further increase the hard coded start delay,
but I prefer this solution because it is proportional to the local system's
speed.
*/
void	*philosopher(void *arg)
{
	t_philo				*p;
	pthread_mutex_t		*temp;
	bool				stopped;

	p = (t_philo *) arg;
	wait_timetarget(p->common->starttime, p);
	if (p->id == 1)
	{
		usleep(100);
	}
	if (p->id % 2 == 0)
	{
		usleep(calc_thinktime(p));
		temp = p->fork_left;
		p->fork_left = p->fork_right;
		p->fork_right = temp;
	}
	stopped = check_stopped(&p->common->stoplock, &p->common->stop);
	while (!stopped)
	{
		take_forks(p);
		eat_sleep_think(p);
		stopped = check_stopped(&p->common->stoplock, &p->common->stop);
	}
	printf("%i times eaten: %i\n", p->id, p->times_eaten);
	return (NULL);
}

void	*philosopher_solo(void *arg)
{
	t_philo			*p;

	p = (t_philo *) arg;
	wait_timetarget(p->common->starttime, p);
	pthread_mutex_lock(p->fork_right);
	broadcast("has taken a fork", p);
	while (!p->common->stop)
		usleep(100);
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
wait_timetarget returns the value of the bool "stopped". So if it is true,
the simulation has stopped and the routine must be canceled.
*/
void	eat_sleep_think(t_philo *p)
{
	time_t		now;
	bool		stopped;

	if (check_stopped(&p->common->stoplock, &p->common->stop))
	{
		release_forks(p);
		return ;
	}
	now = broadcast("is eating", p);
	pthread_mutex_lock(p->last_eat_lock);
	p->last_eat = now;
	p->times_eaten++;
	pthread_mutex_unlock(p->last_eat_lock);
	stopped = wait_timetarget(now + p->common->time_to_eat, p);
	release_forks(p);
	if (stopped)
		return ;
	now = broadcast("is sleeping", p);
	if (wait_timetarget(now + p->common->time_to_sleep, p))
		return ;
	now = broadcast("is thinking", p);
	wait_timetarget(now + calc_thinktime(p), p);
}

void	release_forks(t_philo *p)
{
	pthread_mutex_unlock(p->fork_right);
	pthread_mutex_unlock(p->fork_left);
}
