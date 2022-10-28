/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   2_threadfunctions.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wmardin <wmardin@student.42wolfsburg.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/24 22:06:50 by wmardin           #+#    #+#             */
/*   Updated: 2022/10/28 20:47:54 by wmardin          ###   ########.fr       */
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
	if (p->common->max_eat == 0)
		return (NULL);
	p->last_eat = p->common->starttime;
	wait_start(p);
	take_fork_right(p);
	take_fork_left(p);
	eat(p);
	return (NULL);
}

void	wait_start(t_philo *p)
{
	while (get_time_ms() < p->common->starttime)
		usleep(5);
	if (p->id % 2 == 0)
		usleep(200);
}

void	take_fork_right(t_philo *p)
{
	pthread_mutex_lock(p->fork_right);
	printf("%ld %d has taken a fork\n",
		get_time_ms() - p->common->starttime, p->id);
}

void	take_fork_left(t_philo *p)
{
	pthread_mutex_lock(p->fork_left);
	printf("%li %i has taken a fork\n",
		get_time_ms() - p->common->starttime, p->id);
}

void	eat(t_philo *p)
{
	time_t		currenttime;
	time_t		activity_end;

	currenttime = get_time_ms();
	printf("%li %i is eating\n", currenttime - p->common->starttime, p->id);
	activity_end = currenttime + (time_t)(p->common->time_to_eat);
	while (get_time_ms() < activity_end)
		usleep(10);
	pthread_mutex_unlock(p->fork_right);
	pthread_mutex_unlock(p->fork_left);
}