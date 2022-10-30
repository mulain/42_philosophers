/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   6_utils.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wmardin <wmardin@student.42wolfsburg.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/21 18:35:45 by wmardin           #+#    #+#             */
/*   Updated: 2022/10/30 21:38:10 by wmardin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

time_t	get_time_ms(void)
{
	struct timeval		time;

	gettimeofday(&time, NULL);
	return (time.tv_sec * 1000 + time.tv_usec / 1000);
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
	if (!check_stopped(&p->common->stoplock, &p->common->stop))
		printf("%li %i %s\n", now - p->common->starttime, p->id, msg);
	pthread_mutex_unlock(&p->common->printlock);
	return (now);
}

bool	wait_timetarget(time_t timetarget, t_philo *p)
{
	bool	stopped;

	stopped = false;
	while (get_time_ms() < timetarget && !stopped)
	{
		usleep(100);
		stopped = check_stopped(&p->common->stoplock, &p->common->stop);
	}
	return (stopped);
}

int	calc_thinktime(t_envl *e)
{
	int		freetime;
	int		thinktime;

	freetime = e->common.time_to_die - e->common.time_to_eat
		- e->common.time_to_sleep;
	thinktime = freetime * 0.5;
	if (thinktime < 0)
		thinktime = 0;
	printf("time_to_die:%i\n", e->common.time_to_die);
	printf("time_to_eat:%i\n", e->common.time_to_eat);
	printf("time_to_sleep:%i\n", e->common.time_to_sleep);
	printf("free time:%i\n", freetime);
	printf("thinktime:%i\n", thinktime);
	return (thinktime);
}

bool	check_stopped(pthread_mutex_t *stoplock, bool *stop)
{
	bool	stopped;

	pthread_mutex_lock(stoplock);
	stopped = *stop;
	pthread_mutex_unlock(stoplock);
	return (stopped);
}
