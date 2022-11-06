/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   1_setup_2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wmardin <wmardin@student.42wolfsburg.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/02 11:11:57 by wmardin           #+#    #+#             */
/*   Updated: 2022/11/06 13:56:10 by wmardin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers_bonus.h"

/*
We are not allowed to use sem_init, only sem_open.
That means it is not possible to specify a process specific semaphore. So it
is necessary to make a distinct semaphore for each process to act as a lock for
the writing / reading of the eating-variables.
Another note: I don't believe it is functionally relevant to protect this data
from race conditions. However the subject stated that no races are allowed,
so here we are.
*/
void	init_envelopestruct(t_envl *e)
{
	int		i;

	e->times_eaten = 0;
	e->starttime = calc_starttime(e);
	e->pids = malloc(e->n_philosophers * sizeof(pid_t));
	if (!e->pids)
		exec_error_exit(ERR_MALLOC, e);
	e->le_locks_names = malloc(e->n_philosophers * sizeof(char *));
	if (!e->le_locks_names)
		exec_error_exit(ERR_MALLOC, e);
	i = 0;
	while (i < e->n_philosophers)
	{
		e->le_locks_names[i] = ft_strjoin("/last_eat", zero_or_pos_itoa(i));
		i++;
	}
	e->stop = false;
	e->sem_init = false;
	if (e->n_philosophers == 1)
		e->philofunction = philosopher_solo;
	else
		e->philofunction = philosopher;
}

/*
Apparently, it is good practice to unlink semaphores before opening them to
make sure they are not used by a previous process. So start by unlinking,
then opening.
-	The semaphores named in the 2d array e-lasteatnames are the locks for
	reading / writing to the eat associated variables. See comment on
	init_envelopestruct for why this approach was chosen.
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
void	open_semaphores(t_envl *e)
{
	int		i;

	unlink_semaphores(e);
	e->allsated = sem_open("/allsated", O_CREAT, 0644, 0);
	e->printlock = sem_open("/print", O_CREAT, 0644, 1);
	e->stoplock = sem_open("/stop", O_CREAT, 0644, 0);
	e->printlock = sem_open("/forks", O_CREAT, 0644, e->n_philosophers);
	if (e->allsated == SEM_FAILED || e->printlock == SEM_FAILED
		|| e->stoplock == SEM_FAILED || e->forks == SEM_FAILED)
		exec_error_exit(ERR_SEM_OPEN, e);
	i = 0;
	while (i < e->n_philosophers)
	{
		sem_open(e->le_locks_names[i], O_CREAT, 0644, 1);
		if (e->le_locks_names[i] == SEM_FAILED)
			exec_error_exit(ERR_SEM_OPEN, e);
		i++;
	}
}

void	unlink_semaphores(t_envl *e)
{
	int		i;

	sem_unlink("/allsated");
	sem_unlink("/print");
	sem_unlink("/stop");
	sem_unlink("/forks");
	i = 0;
	while (i < e->n_philosophers)
	{
		sem_unlink(e->le_locks_names[i]);
		i++;
	}
}

bool	init_philostructs(t_envl *e)
{
	int		i;

	i = 0;
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
