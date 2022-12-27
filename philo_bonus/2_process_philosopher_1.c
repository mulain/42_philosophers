/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   2_process_philosopher_1.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wmardin <wmardin@student.42wolfsburg.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/04 08:07:36 by wmardin           #+#    #+#             */
/*   Updated: 2022/12/27 13:18:32 by wmardin          ###   ########.fr       */
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
	pthread_t		monitor_id;

	open_semaphore_philo(e);
	if (pthread_create(&monitor_id, NULL, monitor, e))
		exec_error_exit(ERR_THREAD_CREATE, e);
	wait_timetarget(e->starttime);
	if (e->id % 2 == 0)
		usleep(800);
	/* if (e->id == 1)
		usleep(200); */
	while (1)
		eat_sleep_think(e);
}

/*
Uses str_join to give a unique name since we cannot use
sem_init - which would allow to declare non shared semaphores.
zero_or_pos_itoa is a streamlined version of itoa to convert e.id to alpha.
Immediately unlinks because no other processes need the semaphore and it
stays open for those processes that already have it.
*/
void	open_semaphore_philo(t_envl *e)
{
	char	*semname;
	char	*id;

	id = zero_or_pos_itoa(e->id);
	semname = ft_strjoin("/eat", id);
	free(id);
	e->eatdata_lock = sem_open(semname, O_CREAT | O_EXCL, 0644, 1);
	sem_unlink(semname);
	free(semname);
	if (e->eatdata_lock == SEM_FAILED)
		exec_error_exit(ERR_SEM_OPEN, e);
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
		sem_wait(e->eatdata_lock);
		if (get_time_ms() - e->last_eat > e->time_to_die)
		{
			sem_wait(e->print);
			printf("%li %i died\n", get_time_ms() - e->starttime, e->id);
			sem_post(e->stop_sim);
		}
		sem_post(e->eatdata_lock);
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
	sem_wait(e->eatdata_lock);
	e->last_eat = now;
	sem_post(e->eatdata_lock);
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

int	calc_thinktime(t_envl *e)
{
	int		time_to_think;

	sem_wait(e->eatdata_lock);
	time_to_think = e->time_to_die - get_time_ms() + e->last_eat;
	sem_post(e->eatdata_lock);
	time_to_think *= 0.8;
	return (time_to_think);
}

