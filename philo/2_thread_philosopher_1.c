/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   2_thread_philosopher_1.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wmardin <wmardin@student.42wolfsburg.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/24 22:06:50 by wmardin           #+#    #+#             */
/*   Updated: 2022/12/25 20:46:06 by wmardin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

/*
Even numbered philosophers start with a usleep delay - but they also have
their forks switched.
If the even numbered philosphers started with the same fork order as
the odd numbered ones, the usleep delay might not be long enough for the odd
numbered philosophers to both finish locking their mutexes and
then also take the mutexes of the even numbered philosphers.
The net delay would be: usleep_delay - time_to_lock_first_mutex.
This way the even numbered philosophers will start by trying to get the same
mutex as the odd numbered philosophers - and be blocked there because they will
be too late due to the usleep delay.
An alternative solution could be to further increase the usleep delay,
but this way the usleep can be kept under 1 ms, which is the minimum input
for the time values.

Philosopher 1 starts with a smaller usleep delay. This means that the last
philosopher will take number 1's fork - if the last philosopher isn't subjected
to the large usleep delay of the even philosophers. So this will only happen if
the number of philosophers is uneven.
This results in there being 3 dining groups:
- Uneven philosophers (minus philosopher 1)
- Philosopher 1
- Even philosophers
*/
void	*philosopher(void *arg)
{
	t_philo				*p;

	p = (t_philo *) arg;
	if (p->id % 2 == 0)
		switch_forks(p);
	wait_timetarget(p->starttime, p);
	if (p->id % 2 == 0)
		usleep(5000);
	/* if (p->id == 1)
		usleep(200); */
	while (!check_stopped(p))
		eat_sleep_think(p);
	return (NULL);
}

/*
wait_timetarget contains a call to check_stopped and returns the value
of the bool that stops the simulation.
broadcast contains a call to get_time_ms and returns the current time in ms.
"is sleeping" has to be broadcast before releasing the forks in order to not have
the log look as if an unavailable fork was taken.
*/
void	eat_sleep_think(t_philo *p)
{
	time_t		now;

	take_forks(p);
	/* if (check_stopped(p))
	{
		release_forks(p);
		return ;
	} */
	pthread_mutex_lock(&p->last_eat_lock);
	now = broadcast("is eating", p);
	p->last_eat = now;
	p->times_eaten++;
	pthread_mutex_unlock(&p->last_eat_lock);
	wait_timetarget(now + p->time_to_eat, p);
	now = broadcast("is sleeping", p);
	release_forks(p);
	if (wait_timetarget(now + p->time_to_sleep, p))
		return ;
	now = broadcast("is thinking", p);
	wait_timetarget(now + calc_thinktime(p), p);
}

/*
The remaining time is <time_to_die> - <time elapsed since last meal>
(= current_time - last_eat). Philosophers will think for 90 % of that time to
not block other philosophers from getting forks. Empirically, this seems to
only be relevant in case of uneven number of philosophers.
If remaining_time drops below 1 ms, time_to_think is set to 0 (int division).
This is accepted in order to give a safety margin.

Potential negative values of <time_to_think> aren't a problem as
wait_time_target will immediately terminate (and also the simulation likely
has already stopped due to a death).

printf("remaining time:%i\n", remaining_time);
printf("thinktime:%i\n", time_to_think);
*/
int	calc_thinktime(t_philo *p)
{
	int		time_to_think;

	pthread_mutex_lock(&p->last_eat_lock);
	time_to_think = p->time_to_die - get_time_ms() + p->last_eat;
	pthread_mutex_unlock(&p->last_eat_lock);
	time_to_think *= 0.8;
	return (time_to_think);
}

/*
Waiting function used for all philosopher activities.
Returns a bool whose value is based on whether the simulation has stopped.
*/
bool	wait_timetarget(time_t timetarget, t_philo *p)
{
	while (get_time_ms() < timetarget && !check_stopped(p))
		usleep(100);
	return (check_stopped(p));
}

/*
This function is necessary because killing a thread is not allowed in this
project. Therefore, a solution for not getting stuck waiting for a 2nd fork
has to be hard coded. This routine gets chosen during setup if the number of
philosophers is 1.
The function could just return after broadcasting, but the poor lone philosopher
should at least live his life as long as he can.
*/
void	*philosopher_solo(void *arg)
{
	t_philo		*p;

	p = (t_philo *) arg;
	wait_timetarget(p->starttime, p);
	pthread_mutex_lock(p->fork_right);
	broadcast("has taken a fork", p);
	while (!check_stopped(p))
		usleep(100);
	return (NULL);
}
