/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   5_semaphores.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wmardin <wmardin@student.42wolfsburg.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/07 08:52:34 by wmardin           #+#    #+#             */
/*   Updated: 2022/11/11 20:01:56 by wmardin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers_bonus.h"

/*
It is good practice to unlink semaphores before opening them to
make sure they are not used by a previous process. So start by unlinking,
then opening in main.
Flags: O_CREAT for create semaphore, O_EXCL makes sem_open return an error
if the semaphore to be created already exists. Highly unlikely since we
closed them before and named them specifically, but apparently still good
practice.
-	eaten_enough is a resource semaphore that starts at 0 and gets posted to
	when a philosopher has eaten enough times. At that point the main
	process, that has been waiting for eaten_enough to reach n_philosopher,
	terminates the simulation
-	print is a binary semaphore that controls access to the broadcast function.
	This is to not mix up timestamps - an issue that exists on my system (Win 11
	WSL 2 Ubuntu), but apparently not on school iMacs.
-	stop is a binary semaphore that controls access to the function that will
	terminate the simulation.
-	forks is a resource semaphore that gets depleted by the eating philosophers.
*/
void	open_semaphores_global(t_envl *e)
{
	e->eaten_enough = sem_open("/eaten_enough", O_CREAT | O_EXCL, 0644, 0);
	if (e->eaten_enough == SEM_FAILED)
		exec_error_exit("knudel\n", e);
	e->print = sem_open("/print", O_CREAT | O_EXCL, 0644, 1);
	if (e->print == SEM_FAILED)
		exec_error_exit(ERR_SEM_OPEN, e);
	e->stop_sim = sem_open("/stop", O_CREAT | O_EXCL, 0644, 0);
	if (e->stop_sim == SEM_FAILED)
		exec_error_exit(ERR_SEM_OPEN, e);
	e->forks = sem_open("/forks", O_CREAT | O_EXCL, 0644, e->n_philosophers);
	if (e->forks == SEM_FAILED)
		exec_error_exit(ERR_SEM_OPEN, e);
}

void	unlink_semaphores(t_envl *e)
{
	(void)e;
	sem_unlink("/eaten_enough");
	sem_unlink("/print");
	sem_unlink("/stop");
	sem_unlink("/forks");
}

void	close_semaphores(t_envl *e)
{
	(void)e;
	sem_close(e->eaten_enough);
	sem_close(e->print);
	sem_close(e->stop_sim);
	sem_close(e->forks);
}
