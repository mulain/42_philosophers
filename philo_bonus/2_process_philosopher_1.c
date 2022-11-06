/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   2_process_philosopher_1.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wmardin <wmardin@student.42wolfsburg.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/04 08:07:36 by wmardin           #+#    #+#             */
/*   Updated: 2022/11/06 19:27:05 by wmardin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers_bonus.h"

/*
The child process receives its own stack in the while loop in the main, so the
e.id is unique to the child. Important to remember: the i value is e.id - 1.
The unique semaphores are named after i, i.e. "/last_eat0", "/last_eat1", etc.

printf("child: philo id:%i\n", e->id);
printf("child eatloc name:%s\n", e->le_locks_names[e->id - 1]);
*/
void	philosopher(t_envl *e)
{

	//make deathchecker thread
	wait_timetarget(e->starttime, e);
	if (e->id % 2 == 0)
		usleep(800);
	if (e->id == 1)
		usleep(200);
	while (1)
		eat_sleep_think(e);
}

void	eat_sleep_think(t_philo *p)
{
	time_t		now;

	take_forks(p);
	if (check_stopped(p))
	{
		release_forks(p);
		return ;
	}
	now = broadcast("is eating", p);
	pthread_mutex_lock(p->last_eat_lock);
	p->last_eat = now;
	p->times_eaten++;
	pthread_mutex_unlock(p->last_eat_lock);
	wait_timetarget(now + p->global->time_to_eat, p);
	now = broadcast("is sleeping", p);
	release_forks(p);
	if (wait_timetarget(now + p->global->time_to_sleep, p))
		return ;
	now = broadcast("is thinking", p);
	wait_timetarget(now + calc_thinktime(p), p);
}

/*
Processes don't have to check for the simulation being stopped because they
will be killed by main process. So they just wait for time_target.
*/
void	wait_timetarget(time_t timetarget, t_envl *e)
{
	while (get_time_ms() < timetarget)
		usleep(100);
}

bool	thread_wait_timetarget(time_t timetarget, t_philo *p)
{
	bool	stopped;

	stopped = check_stopped(p);
	while (get_time_ms() < timetarget && !stopped)
	{
		usleep(100);
		stopped = check_stopped(p);
	}
	return (stopped);
}

void	philosopher_solo(t_envl *e)
{
	printf("child: philo id:%i\n", e->id);
	printf("child eatloc name:%s\n", e->le_locks_names[e->id - 1]);
	return ;
}