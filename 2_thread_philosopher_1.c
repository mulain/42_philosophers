/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   2_thread_philosopher_1.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wmardin <wmardin@student.42wolfsburg.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/24 22:06:50 by wmardin           #+#    #+#             */
/*   Updated: 2022/10/31 12:58:48 by wmardin          ###   ########.fr       */
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
	bool				stopped;

	p = (t_philo *) arg;
	if (p->id % 2 == 0)
		switch_forks(p);
	wait_timetarget(p->common->starttime, p);
	wait_timetarget(get_time_ms() + calc_thinktime(p), p);
	/* if (p->id == 1)
	{
		usleep(100);
	} */
	stopped = check_stopped(p);
	while (!stopped)
	{
		eat_sleep_think(p);
		stopped = check_stopped(p);
	}
	printf("%i times eaten: %i\n", p->id, p->times_eaten);
	return (NULL);
}

/*
wait_timetarget conatins a call to check_stopped and returns the value
of the bool that stops the simulation.
broadcast contains a call to get_time_ms and returns the current time in ms.
*/
void	eat_sleep_think(t_philo *p)
{
	time_t		now;
	bool		stopped;

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
	//p->death_time = now + p->common->time_to_die;
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

/*
Philosophers die if they don't start eating time_to_die after starting their
last meal. The remaining time therefore is <time_to_die> - <time elapsed since
last meal>.
Uneven philosophers are supposed to eat first - but the even ones have to eat
after them without dying. So their remaining time is decreased by <time_to_eat>
for the time they need to eat before they can give the forks to the other
philosphers.
Final value is decreased by 1 ms to give a safety margin - at the cost of not
being able to meet close deadlines (i.e. <time_to_die> - <time_to_sleep> -
<time_to_eat> < 2 ms.
*/
int	calc_thinktime(t_philo *p)
{
	int		remaining_time;
	int		time_to_think;

	pthread_mutex_lock(p->last_eat_lock);
	remaining_time = p->common->time_to_die - get_time_ms() + p->last_eat;
	pthread_mutex_unlock(p->last_eat_lock);
	if (p->id % 2 == 1)
		remaining_time -= p->common->time_to_eat;
	time_to_think = remaining_time - 1;
	/* if (time_to_think < 0)
		time_to_think = 0; */
	/* if (time_to_think > 200)
		time_to_think = 200; */
	printf("remaining time:%i\n", remaining_time);
	printf("thinktime:%i\n", time_to_think);
	return (time_to_think);
}

bool	wait_timetarget(time_t timetarget, t_philo *p)
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

/*
This function is necessary because killing a thread is not allowed in this
project. Therefore, a solution for not getting stuck waiting for a 2nd fork
has to be hard coded. The function could just return after broadcasting, but the
poor lone philosopher should at least live his life as long as he can.
*/
void	*philosopher_solo(void *arg)
{
	t_philo		*p;
	bool		stopped;

	p = (t_philo *) arg;
	wait_timetarget(p->common->starttime, p);
	pthread_mutex_lock(p->fork_right);
	broadcast("has taken a fork", p);
	stopped = check_stopped(p);
	while (!stopped)
	{
		usleep(100);
		stopped = check_stopped(p);
	}
	return (NULL);
}
