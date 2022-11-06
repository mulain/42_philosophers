/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   1_setup_2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wmardin <wmardin@student.42wolfsburg.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/02 11:11:57 by wmardin           #+#    #+#             */
/*   Updated: 2022/11/06 10:03:42 by wmardin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers_bonus.h"

void	init_envelopestruct(t_envl *e)
{
	int		offset;

	e->init = false;
	e->pids = malloc(e->n_philosophers * sizeof(pid_t));
	if (!e->pids)
		exec_error_exit(ERR_MALLOC, e);
	offset = e->n_philosophers * 20;
	if (offset > 1000)
		offset = 1000;
	if (offset < 100)
		offset = 100;
	e->global.starttime = get_time_ms() + offset;
	//e->global.stop = false;
	if (e->n_philosophers == 1)
		e->philofunction = philosopher_solo;
	else
		e->philofunction = philosopher;
}

/*
jacob sorber what is a semaphore
9.30
Apparently, it is good practice to unlink semaphores before opening them to
make sure they are not used by a previous process. So start by unlinking,
then opening.
-	allsated is a counter semaphore that starts at 0 and gets posted to
	when a philosopher has eaten enough times. At that point the main
	process, that has been waiting for allsated to reach n_philosopher,
	terminates the simulation
-	print is a binary semaphore that controls access to the broadcast function.
	This is to not mix up timestamps - an issue that exists on my system (Win 11
	WSL 2 Ubuntu), but apparently not on school iMacs.
-	stop is a binary semaphore that controls access to the function that will
	terminate the simulation.
-	forks is a resource semaphore that gets depleted by the eating philosophers.
*/
void	open_sharedsemaphores(t_envl *e)
{
	sem_t	*allsated;
	sem_t	*print;
	sem_t	*stop;
	sem_t	*forks;
	//sem_t	*lasteat;

	sem_unlink("/allsated");
	sem_unlink("/print");
	sem_unlink("/stop");
	sem_unlink("/forks");
	sem_unlink("/lasteat");
	allsated = sem_open("/allsated", O_CREAT, 0660, 0);
	print = sem_open("/print", O_CREAT, 0660, 1);
	stop = sem_open("/stop", O_CREAT, 0660, 0);
	print = sem_open("/forks", O_CREAT, 0660, e->n_philosophers);
	if (allsated == SEM_FAILED || print == SEM_FAILED || stop == SEM_FAILED
		|| forks == SEM_FAILED)
		exec_error_exit(ERR_SEM_OPEN);
	//lasteat = semopen("/lasteat", 0660, 1);
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
	error += pthread_mutex_init(&e->common.printlock, NULL);
	error += pthread_mutex_init(&e->common.stoplock, NULL);
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
		e->philo[i].common = &e->common;
		e->philo[i].fork_right = &e->forks[i];
		if (i == 0)
			e->philo[i].fork_left = &e->forks[e->n_philosophers - 1];
		else
			e->philo[i].fork_left = &e->forks[i - 1];
		e->philo[i].last_eat = e->common.starttime;
		e->philo[i].death_time = e->common.starttime + e->common.time_to_die;
		e->philo[i].last_eat_lock = &e->last_eat_locks[i];
		e->philo[i].times_eaten = 0;
		i++;
	}
	return (true);
}
