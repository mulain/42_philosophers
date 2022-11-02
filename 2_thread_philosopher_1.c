/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   2_thread_philosopher_1.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wmardin <wmardin@student.42wolfsburg.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/24 22:06:50 by wmardin           #+#    #+#             */
/*   Updated: 2022/11/02 13:09:59 by wmardin          ###   ########.fr       */
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
	bool				stopped;

	p = (t_philo *) arg;
	if (p->id % 2 == 0)
		switch_forks(p);
	wait_timetarget(p->common->starttime, p);
	if (p->id % 2 == 0)
		usleep(800);
	if (p->id == 1)
		usleep(200);
	stopped = check_stopped(p);
	while (!stopped)
	{
		eat_sleep_think(p);
		stopped = check_stopped(p);
	}
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
	wait_timetarget(now + p->common->time_to_eat, p);
	now = broadcast("is sleeping", p);
	release_forks(p);
	if (wait_timetarget(now + p->common->time_to_sleep, p))
		return ;
	now = broadcast("is thinking", p);
	wait_timetarget(now + calc_thinktime(p), p);
}

/*
For philosphers not to die, <time_to_die> has to be > 2 x <time_to_eat>
because the philosophers have to be split in at least 2 groups that can
only eat sequentially. <time_to_sleep> of philospher group 1 can be used
by group 2 to eat, so it isn't wasted time.
Philosophers die if they don't start eating time_to_die after starting their
last meal. The remaining time therefore is <time_to_die> - <time elapsed since
last meal> (= current_time - last_eat).
Uneven philosophers are supposed to eat first - but the even ones have to eat
after them without dying. wronk. they can already eat while the uneven ones are
sleeping........

So their remaining time is decreased by <time_to_eat>
for the time they need to eat before they can give the forks to the other
philosphers.
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
	int		remaining_time;
	int		time_to_think;

	pthread_mutex_lock(p->last_eat_lock);
	remaining_time = p->common->time_to_die - get_time_ms() + p->last_eat;
	pthread_mutex_unlock(p->last_eat_lock);
	time_to_think = remaining_time * 0.5;
	/* if (time_to_think > 200)
		time_to_think = 200; */
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
has to be hard coded. This routine gets chosen during setup if the number of
philosophers is 1.
The function could just return after broadcasting, but the poor lone philosopher
should at least live his life as long as he can.
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
