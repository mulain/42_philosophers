/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   1_setup_2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wmardin <wmardin@student.42wolfsburg.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/02 11:11:57 by wmardin           #+#    #+#             */
/*   Updated: 2022/12/26 20:07:13 by wmardin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

bool	init_envelopestruct(t_envl *e, int argc, char **argv)
{
	e->n_philosophers = ft_atoi(argv[1]);
	e->philo = malloc(e->n_philosophers * sizeof(t_philo));
	if (!e->philo)
		return (msg_exec_error(ERR_MALLOC, e), true);
	e->mutex_init = false;
	e->time_to_die = ft_atoi(argv[2]);
	e->time_to_eat = ft_atoi(argv[3]);
	e->time_to_sleep = ft_atoi(argv[4]);
	if (argc == 6)
		e->times_to_eat = ft_atoi(argv[5]);
	else
		e->times_to_eat = -1;
	e->starttime = calc_starttime(e);
	e->stop = false;
	if (pthread_mutex_init(&e->printlock, NULL))
		return (msg_exec_error(ERR_MUTEX_INIT, e), true);
	if (pthread_mutex_init(&e->stoplock, NULL))
		return (false);
	if (e->n_philosophers == 1)
		e->philofunction = philosopher_solo;
	else
		e->philofunction = philosopher;
	return (false);
}

bool	init_philostructs(t_envl *e)
{
	int		i;

	i = -1;
	while (++i < e->n_philosophers)
	{
		e->philo[i].id = i + 1;
		e->philo[i].times_eaten = 0;
		e->philo[i].starttime = e->starttime;
		e->philo[i].last_eat = e->starttime;
		e->philo[i].time_to_die = e->time_to_die;
		e->philo[i].time_to_eat = e->time_to_eat;
		e->philo[i].time_to_sleep = e->time_to_sleep;
		e->philo[i].times_to_eat = e->times_to_eat;
		pthread_mutex_init(&e->philo[i].fork, NULL);
		e->philo[i].fork_left = &e->philo[i].fork;
		e->philo[i].fork_right = &e->philo[(i + 1) % e->n_philosophers].fork;
		pthread_mutex_init(&e->philo[i].last_eat_lock, NULL);
		e->philo[i].printlock = &e->printlock;
		e->philo[i].stoplock = &e->stoplock;
		e->philo[i].stop = &e->stop;
	}
	return (false);
}

time_t	calc_starttime(t_envl *e)
{
	int		offset;

	offset = e->n_philosophers * 50;
	if (offset > 2000)
		offset = 2000;
	else if (offset < 100)
		offset = 100;
	return (get_time_ms() + offset);
}
