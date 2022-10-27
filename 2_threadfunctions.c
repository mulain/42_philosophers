/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   2_threadfunctions.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wmardin <wmardin@student.42wolfsburg.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/24 22:06:50 by wmardin           #+#    #+#             */
/*   Updated: 2022/10/27 17:19:45 by wmardin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

/*
printf("tv_sec:%li\n", e->t.tv_sec);
printf("tv_usec:%li\n", e->t.tv_usec);
printf("ms:%li\n", p->t.tv_usec / 1000);

implement starting time
*/
void	*philosopher(void *arg)
{
	t_philo		*p;

	p = (t_philo *) arg;

	take_fork_right(p);
	take_fork_left(p);
	return (NULL);
}

void	take_fork_right(t_philo *p)
{
	pthread_mutex_lock(p->fork_right);
	printf("%li %i has taken a fork\n", get_timestamp(p->info->starttime), p->id);
	//pthread_mutex_unlock(p->fork_right);
}

void	take_fork_left(t_philo *p)
{
	pthread_mutex_lock(p->fork_left);
	//broadcast(get_timestamp(p->info->starttime), "has taken a fork\n");
	printf("%li %i has taken a fork\n", get_timestamp(p->info->starttime), p->id);
}

time_t	get_timestamp(time_t starttime)
{
	struct timeval		time;
	time_t				currenttime_ms;

	gettimeofday(&time, NULL);
	currenttime_ms = time.tv_sec * 1000 + time.tv_usec / 1000;
	return (currenttime_ms - starttime);
}

/*
void	broadcast(time_t timestamp, char *msg)
{
	printf("%i %i %s\n", timestamp, p->id, msg);
}
*/

void	taken_a_fork(int timestamp, int name, pthread_mutex_t *print)
{
	pthread_mutex_lock(print);
	printf("%i %i has taken a fork\n", timestamp, name);
	pthread_mutex_unlock(print);
}

void	is_eating(int timestamp, int name, pthread_mutex_t *print)
{
	pthread_mutex_lock(print);
	printf("%i %i is eating\n", timestamp, name);
	pthread_mutex_unlock(print);
}