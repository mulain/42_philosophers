/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   2_process_philosopher_1.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wmardin <wmardin@student.42wolfsburg.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/04 08:07:36 by wmardin           #+#    #+#             */
/*   Updated: 2022/12/28 23:41:12 by wmardin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers_bonus.h"

/*
The child process receives its own stack in the while loop in the main, so the
e.id is unique to each philo. It is used to name the semaphore that (for each
philo) controls the interaction of the eat_sleep_think loop with the monitor
thread. There is no thread_join function because the philos will run
indefinitely or until killed.
*/
void	philosopher(t_envl *e)
{
	pthread_t	monitor_id;

	if (pthread_create(&monitor_id, NULL, monitor, e))
		exec_error_exit(ERR_THREAD_CREATE, e);
	wait_timetarget(e->starttime);
	if (e->id % 2 == 0)
		wait_timetarget(e->starttime + calc_thinktime(e));
	while (1)
		eat_sleep_think(e);
}

/*
Runs a continuous loop that checks if the philosopher has died from
hunger.
Waits for the lasteatlock to not create a race condition. This is to
satisfy the formal project requirements. I don't think it would cause
any functional problems.
If not dead, gives back the last_eat_lock and sleeps until it's time to
start over.
If philo died, locks the print, prints the death, posts to stop_sim.
This will result in the main process stopmonitor ending the simulation.
Doesn't use the broadcast function because it should not post the print.
*/
void	*monitor(void *arg)
{
	t_envl		*e;

	e = (t_envl *)arg;
	while (1)
	{
		sem_wait(e->eat_locks[e->id - 1]);
		if (get_time_ms() - e->last_eat > e->time_to_die)
		{
			sem_wait(e->print);
			printf("%li %i died\n", get_time_ms() - e->starttime, e->id);
			sem_post(e->stop_sim);
		}
		sem_post(e->eat_locks[e->id - 1]);
		usleep(1000);
	}
}

/*
Semaphores are named according to i, which starts at 0. We only have e.id
(starting at 1) which is i + 1. So reference to sem is e.id - 1.
At the moment <times_to_eat> is reached, posts to semaphore eaten_enough once.
Only posts when times_eaten is equal to <times_to_eat>, so only ever posts once
per process. The main process sem_waits for n_philosophers, so it will only be
able to continue when all have eaten enough.
*/
void	eat_sleep_think(t_envl *e)
{
	time_t		now;

	take_forks(e);
	now = broadcast("is eating", e);
	sem_wait(e->eat_locks[e->id - 1]);
	e->last_eat = now;
	sem_post(e->eat_locks[e->id - 1]);
	e->times_eaten++;
	if (e->times_eaten == e->times_to_eat)
		sem_post(e->eaten_enough);
	wait_timetarget(now + e->time_to_eat);
	now = broadcast("is sleeping", e);
	release_forks(e);
	wait_timetarget(now + e->time_to_sleep);
	now = broadcast("is thinking", e);
	wait_timetarget(now + calc_thinktime(e));
}

/*
Time to think is empirically selected to be able to handle 200 philos on my
machine and the school machines.
An algorithmic solution would be better, but I want to submit now.
*/
int	calc_thinktime(t_envl *e)
{
	int		time_to_think;

	sem_wait(e->eat_locks[e->id - 1]);
	time_to_think = e->time_to_die - get_time_ms() + e->last_eat;
	sem_post(e->eat_locks[e->id - 1]);
	time_to_think *= 0.4;
	return (time_to_think);
}
