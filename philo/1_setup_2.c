/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   1_setup_2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wmardin <wmardin@student.42wolfsburg.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/02 11:11:57 by wmardin           #+#    #+#             */
/*   Updated: 2022/11/06 09:30:12 by wmardin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

bool	init_envelopestruct(t_envl *e)
{
	e->threads = NULL;
	e->forks = NULL;
	e->mutex_init = false;
	e->philo = NULL;
	e->threads = malloc(e->n_philosophers * sizeof(pthread_t));
	if (!e->threads)
		return (exec_error_exit(ERR_MALLOC, e));
	e->global.starttime = calc_starttime(e);
	e->global.stop = false;
	if (e->n_philosophers == 1)
		e->philofunction = philosopher_solo;
	else
		e->philofunction = philosopher;
	return (true);
}

bool	init_mutexes(t_envl *e)
{
	int		i;
	int		error;

	error = 0;
	e->forks = malloc(e->n_philosophers * sizeof(pthread_mutex_t));
	e->last_eat_locks = malloc(e->n_philosophers * sizeof(pthread_mutex_t));
	if (!e->forks || !e->last_eat_locks)
		return (exec_error_exit(ERR_MALLOC, e));
	i = 0;
	while (i < e->n_philosophers)
	{
		error += pthread_mutex_init(&e->forks[i], NULL);
		error += pthread_mutex_init(&e->last_eat_locks[i], NULL);
		i++;
	}
	error += pthread_mutex_init(&e->global.printlock, NULL);
	error += pthread_mutex_init(&e->global.stoplock, NULL);
	if (error)
		return (exec_error_exit(ERR_MUTEX_INIT, e));
	e->mutex_init = true;
	return (true);
}

bool	init_philostructs(t_envl *e)
{
	int		i;

	i = 0;
	e->philo = malloc(e->n_philosophers * sizeof(t_philo));
	if (!e->philo)
		return (exec_error_exit(ERR_MALLOC, e));
	while (i < e->n_philosophers)
	{
		e->philo[i].id = i + 1;
		e->philo[i].global = &e->global;
		e->philo[i].fork_right = &e->forks[i];
		if (i == 0)
			e->philo[i].fork_left = &e->forks[e->n_philosophers - 1];
		else
			e->philo[i].fork_left = &e->forks[i - 1];
		e->philo[i].last_eat = e->global.starttime;
		e->philo[i].last_eat_lock = &e->last_eat_locks[i];
		e->philo[i].times_eaten = 0;
		i++;
	}
	return (true);
}

int	calc_starttime(t_envl *e)
{
	int		offset;

	offset = e->n_philosophers * 20;
	if (offset > 1000)
		offset = 1000;
	if (offset < 100)
		offset = 100;
	return (get_time_ms() + offset);
}
