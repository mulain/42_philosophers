/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   2_threadfunctions.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wmardin <wmardin@student.42wolfsburg.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/24 22:06:50 by wmardin           #+#    #+#             */
/*   Updated: 2022/10/24 22:52:55 by wmardin          ###   ########.fr       */
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
	gettimeofday(&p->t, NULL);
	pthread_mutex_lock(p->fork_right);
	taken_a_fork(p->t.tv_usec / 1000, p->name, &p->print);
	pthread_mutex_lock(p->fork_left);
	taken_a_fork(p->t.tv_usec / 1000, p->name, &p->print);
	is_eating(p->t.tv_usec / 1000, p->name, &p->print);
	return (NULL);
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