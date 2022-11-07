/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   5_semaphores.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wmardin <wmardin@student.42wolfsburg.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/07 08:52:34 by wmardin           #+#    #+#             */
/*   Updated: 2022/11/07 13:27:35 by wmardin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers_bonus.h"

/*
It is good practice to unlink semaphores before opening them to
make sure they are not used by a previous process. So start by unlinking,
then opening.
Flags: O_CREAT for create semaphore, O_EXCL makes sem_open return an error
if the semaphore to be created already exists. Highly unlikely since we
closed them before and named them specifically, but apparently still good
practice.
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
	int		semval;

	e->allsated = sem_open("/allsated", O_CREAT | O_EXCL, 0644, 0);
	e->printlock = sem_open("/print", O_CREAT | O_EXCL, 0644, 1);
	e->stoplock = sem_open("/stop", O_CREAT | O_EXCL, 0644, 0);
	e->forks = sem_open("/forks", O_CREAT | O_EXCL, 0644, e->n_philosophers);
	if (e->allsated == SEM_FAILED || e->printlock == SEM_FAILED
		|| e->stoplock == SEM_FAILED || e->forks == SEM_FAILED)
		exec_error_exit(ERR_SEM_OPEN, e);
	i = 0;
	while (i < e->n_philosophers)
	{
		e->last_eat_locks[i] = sem_open(e->le_locks_names[i],
				O_CREAT | O_EXCL, 0644, 1);
		if (e->last_eat_locks[i] == SEM_FAILED)
			exec_error_exit(ERR_SEM_OPEN, e);
		i++;
	}
	sem_getvalue(e->forks, &semval);
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

void	close_semaphores(t_envl *e)
{
	int		i;

	sem_close(e->allsated);
	sem_close(e->printlock);
	sem_close(e->stoplock);
	sem_close(e->forks);
	i = 0;
	while (i < e->n_philosophers)
	{
		sem_close(e->last_eat_locks[i]);
		i++;
	}
}
