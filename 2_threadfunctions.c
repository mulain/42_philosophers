/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   2_threadfunctions.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wmardin <wmardin@student.42wolfsburg.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/24 22:06:50 by wmardin           #+#    #+#             */
/*   Updated: 2022/10/25 17:01:55 by wmardin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

/*
printf("tv_sec:%li\n", e->t.tv_sec);
printf("tv_usec:%li\n", e->t.tv_usec);
printf("ms:%li\n", p->t.tv_usec / 1000);
*/
void	*philosopher(void *arg)
{
	t_philo		*p;

	p = (t_philo *) arg;
	pthread_mutex_lock(p->fork_right);
	taken_a_fork(p->t.tv_usec / 1000, p->id, &p->info->printlock);
	pthread_mutex_lock(p->fork_left);
	taken_a_fork(p->t.tv_usec / 1000, p->id, &p->info->printlock);
	is_eating(p->t.tv_usec / 1000, p->id, &p->info->printlock);
	return (NULL);
}

time_t	get_timestamp(t_philo *p)
{
	struct timeval		time;

	gettimeofday(&time, NULL);
	return (p->info->starttime - time.tv_sec * 1000 + time.tv_usec / 1000);
}

void	broadcast(t_philo *p, char *msg)
{
	pthread_mutex_lock(p->info->printlock);
	printf("%i %i %s\n", p->timestamp, p->id, msg);
	pthread_mutex_unlock(p->info->printlock);
}

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