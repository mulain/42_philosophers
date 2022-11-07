/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   2_process_philosopher_1.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wmardin <wmardin@student.42wolfsburg.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/04 08:07:36 by wmardin           #+#    #+#             */
/*   Updated: 2022/11/07 10:50:16 by wmardin          ###   ########.fr       */
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
	wait_timetarget(e->starttime);
	if (e->id % 2 == 0)
		usleep(800);
	if (e->id == 1)
		usleep(200);
	while (1)
		eat_sleep_think(e);
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
	int			sval;

	take_forks(e);
	now = broadcast("is eating", e);
	sem_wait(e->last_eat_locks[e->id - 1]);
	e->last_eat = now;
	e->times_eaten++;
	if (e->times_eaten == e->times_to_eat)
	{
		printf("if clause allsated active\n");
		sem_getvalue(e->allsated, &sval);
		printf("allsated val before post:%i\n", sval);
		sem_post(e->allsated);
		printf("allsated val after post:%i\n", sval);
	}
	sem_post(e->last_eat_locks[e->id - 1]);
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
	time_to_think *= 0.8;
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

void	philosopher_solo(t_envl *e)
{
	printf("child: philo id:%i\n", e->id);
	printf("child eatloc name:%s\n", e->le_locks_names[e->id - 1]);
	return ;
}