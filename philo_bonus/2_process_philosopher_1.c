/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   2_process_philosopher_1.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wmardin <wmardin@student.42wolfsburg.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/04 08:07:36 by wmardin           #+#    #+#             */
/*   Updated: 2022/11/07 12:48:01 by wmardin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers_bonus.h"

/*
The child process receives its own stack in the while loop in the main, so the
e.id is unique to the child. Important to remember: the i value is e.id - 1.
The unique semaphores are named after i, i.e. "/last_eat0", "/last_eat1", etc.

There is no thread_join function because the philos will run indefinitely or
until killed.

printf("child: philo id:%i\n", e->id);
printf("child eatloc name:%s\n", e->le_locks_names[e->id - 1]);
*/
void	philosopher(t_envl *e)
{
	pthread_t		monitor_id;

	if (pthread_create(&monitor_id, NULL, monitor, e))
		exec_error_exit(ERR_THREAD_CREATE, e);
	wait_timetarget(e->starttime);
	if (e->id % 2 == 0)
		usleep(800);
	if (e->id == 1)
		usleep(200);
	while (1)
		eat_sleep_think(e);
}

/*
Runs a continuous loop that checks if the philosopher has died from
hunger.
Waits for the lasteatlock to not create a race condition. This is to
satisfy the formal project requirements. I don't think it would cause
any functional problems.
If philo died, locks the printlock, broadcasts the death, posts to stoplock.
This will result in the main process stopmonitor ending the simulation.
If not dead, gives back the last_eat_lock. And sleeps until it's time to
start over.
*/
void	*monitor(void *arg)
{
	t_envl		*e;

	e = (t_envl *)arg;
	while (1)
	{
		sem_wait(e->last_eat_locks[e->id - 1]);
		if (get_time_ms() - e->last_eat >= e->time_to_die)
		{
			sem_wait(e->printlock);
			printf("%li %i died\n", get_time_ms() - e->starttime, e->id);
			sem_post(e->stoplock);
		}
		sem_post(e->last_eat_locks[e->id - 1]);
		usleep(100);
	}
}

/*
Remember: semaphores are named according to i, which starts at 0.
But we only have e.id (starting at 1) which is i + 1.
At the moment <times_to_eat> is reached, posts to semaphore allsated once.
Only posts when times_eaten is equal to <times_to_eat>, so only ever posts once
per process. The main process sem_waits for n_philosophers, so it will "know"
when all have eaten enough.
*/
void	eat_sleep_think(t_envl *e)
{
	time_t		now;

	take_forks(e);
	now = broadcast("is eating", e);
	sem_wait(e->last_eat_locks[e->id - 1]);
	e->last_eat = now;
	sem_post(e->last_eat_locks[e->id - 1]);
	e->times_eaten++;
	if (e->times_eaten == e->times_to_eat)
		sem_post(e->allsated);
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

	sem_wait(e->last_eat_locks[e->id - 1]);
	time_to_think = e->time_to_die - get_time_ms() + e->last_eat;
	sem_post(e->last_eat_locks[e->id - 1]);
	time_to_think *= 0.5;
	return (time_to_think);
}

/*
Processes don't have to check for the simulation being stopped because they
will be killed by main process. So they just wait for time_target.

for pid in $(ps -ef | grep "/philo" | awk '{print $2}'); do kill -9 $pid; done
*/
void	wait_timetarget(time_t timetarget)
{
	while (get_time_ms() < timetarget)
		usleep(100);
}

/* void	philosopher_solo(t_envl *e)
{
	wait_timetarget(e->starttime);


} */