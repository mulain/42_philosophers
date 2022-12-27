/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   5_semaphores.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wmardin <wmardin@student.42wolfsburg.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/07 08:52:34 by wmardin           #+#    #+#             */
/*   Updated: 2022/12/27 21:56:38 by wmardin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers_bonus.h"

/*
It is good practice to unlink semaphores before opening them to
make sure they are not used by a previous process (however unlikely).
It is also good practice to immediately unlink them again if no separate
processes need them as they stay open for those processes, that already
have them open. This also applies to children that inherit the open sems
from the parent.
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
	unlink_semaphores_global(e);
	e->eaten_enough = sem_open("/eaten_enough", O_CREAT | O_EXCL, 0644, 0);
	if (e->eaten_enough == SEM_FAILED)
		exec_error_exit(ERR_SEM_OPEN, e);
	e->print = sem_open("/print", O_CREAT | O_EXCL, 0644, 1);
	if (e->print == SEM_FAILED)
		exec_error_exit(ERR_SEM_OPEN, e);
	e->stop_sim = sem_open("/stop", O_CREAT | O_EXCL, 0644, 0);
	if (e->stop_sim == SEM_FAILED)
		exec_error_exit(ERR_SEM_OPEN, e);
	e->forks = sem_open("/forks", O_CREAT | O_EXCL, 0644, e->n_philosophers);
	if (e->forks == SEM_FAILED)
		exec_error_exit(ERR_SEM_OPEN, e);
	unlink_semaphores_global(e);
}

void	unlink_semaphores_global(t_envl *e)
{
	(void)e;
	sem_unlink("/eaten_enough");
	sem_unlink("/print");
	sem_unlink("/stop");
	sem_unlink("/forks");
}

void	close_semaphores_all(t_envl *e)
{
	int		i;

	sem_close(e->eaten_enough);
	sem_close(e->print);
	sem_close(e->stop_sim);
	sem_close(e->forks);
	i = 0;
	while (i < e->n_philosophers)
	{
		sem_close(e->eat_locks[i]);
		i++;
	}
}

/*
Semaphores must be named with a distrinct string: The common root
"/eat" is taken and the iteration of i + 1 (=philo id) is transformed
to alpha and appended using stringjoin. The sems look like this:
"/eat1" "/eat2" "/eat3", etc.
I thought this could be done after forking by having each philo process
open its own semaphore to the struct var e.lasteatlock. But those sems
actually all had the same address across the processes. I do not understand
why that happened -  I thought each process would have its own heap and
stack therefore would not open the sems at the same address.
I changed to this to try to speed things up (I thought all processes might be
using the same sem!). Not 100 % sure if this is actually the case but now
things are way faster (philo 200 950 200 200 was neede before, now
philo 200 430 200 200 is ok). Also changed the factor for time to think
but I tested it also with old semaphore version. So it is currently my best
explanation that the processes were sharing the same eatlock semaphore and that
was slowing things down.
*/
void	open_semaphores_philo(t_envl *e)
{
	int		i;
	char	*semname;
	char	*id_char;

	i = 0;
	while (i < e->n_philosophers)
	{
		id_char = zero_or_pos_itoa(i + 1);
		semname = ft_strjoin("/eat", id_char);
		free(id_char);
		sem_unlink(semname);
		e->eat_locks[i] = sem_open(semname, O_CREAT | O_EXCL, 0644, 1);
		sem_unlink(semname);
		free(semname);
		if (e->eat_locks[i] == SEM_FAILED)
			exec_error_exit(ERR_SEM_OPEN, e);
		i++;
	}
}
