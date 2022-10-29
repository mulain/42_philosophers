/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   2_threadfunctions.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wmardin <wmardin@student.42wolfsburg.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/24 22:06:50 by wmardin           #+#    #+#             */
/*   Updated: 2022/10/29 12:44:55 by wmardin          ###   ########.fr       */
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
	if (p->common->times_to_eat == 0)
		return (NULL);
	p->last_eat = p->common->starttime;
	wait_start(p);
	while (p->times_eaten < p->common->times_to_eat)
	take_fork_right(p);
	take_fork_left(p);
	eat_and_sleep(p);
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
	printf("%li %i has taken a fork\n",
		get_time_ms() - p->common->starttime, p->id);
}

void	take_fork_left(t_philo *p)
{
	pthread_mutex_lock(p->fork_left);
	printf("%li %i has taken a fork\n",
		get_time_ms() - p->common->starttime, p->id);
}

void	eat_and_sleep(t_philo *p)
{
	time_t		now;

	now = broadcast("is eating", p);
	p->last_eat = now;
	p->times_eaten++;
	while (get_time_ms() < now + p->common->time_to_eat)
		usleep(10);
	pthread_mutex_unlock(p->fork_right);
	pthread_mutex_unlock(p->fork_left);
	now = broadcast("is sleeping", p);
	while (get_time_ms() < now + p->common->time_to_sleep)
		usleep(10);
	now = broadcast("is thinking", p);
	while (get_time_ms() < now + p->common->time_to_think)
		usleep(10);
}

time_t	broadcast(char *msg, t_philo *p)
{
	time_t		now;

	pthread_mutex_lock(&p->common->printlock);
	now = get_time_ms();
	printf("%li %i %s\n", now - p->common->starttime, p->id, msg);
	pthread_mutex_unlock(&p->common->printlock);
	return (now);
}
