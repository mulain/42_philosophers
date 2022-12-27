/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   0_main.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wmardin <wmardin@student.42wolfsburg.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/03 09:03:05 by wmardin           #+#    #+#             */
/*   Updated: 2022/12/27 21:38:24 by wmardin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers_bonus.h"

/*
To do:
- Speed issues bonus
- Kill child: cleanup?
- Data races: how to test for processes?

*/
int	main(int argc, char **argv)
{
	t_envl		e;

	setup(&e, argc, argv);
	launch_subentities(&e);
	land_subentities(&e);
	shutdown(&e);
}

/*
The children will never return from executing the philofunction so
there is no guard against them continuing the execution of the main
function.
*/
void	launch_subentities(t_envl *e)
{
	int		i;

	i = 0;
	while (i < e->n_philosophers)
	{
		e->id = i + 1;
		e->pids[i] = fork();
		if (e->pids[i] == -1)
			exec_error_exit("Error: fork\n", e);
		if (e->pids[i] == 0)
			philosopher(e);
		i++;
	}
	if (pthread_create(&e->eatmonitor, NULL, eatmonitor, e))
		exec_error_exit(ERR_THREAD_CREATE, e);
	if (pthread_create(&e->stopmonitor, NULL, stopmonitor, e))
		exec_error_exit(ERR_THREAD_CREATE, e);
}

void	land_subentities(t_envl *e)
{
	int		i;

	i = 0;
	while (i < e->n_philosophers)
	{
		if (waitpid(e->pids[i], NULL, 0) == -1)
			exec_error_exit(ERR_WAITPID, e);
		i++;
	}
	if (pthread_join(e->eatmonitor, NULL))
		exec_error_exit(ERR_THREAD_JOIN, e);
	if (pthread_join(e->stopmonitor, NULL))
		exec_error_exit(ERR_THREAD_JOIN, e);
}

/*
Keeps waiting on (reducing) the eaten_enough semaphore until all philosophers
have posted to it once.
Sets the stop_sim to signal the end of the simulation.
*/
void	*eatmonitor(void *arg)
{
	t_envl		*e;
	int			i;

	e = (t_envl *)arg;
	wait_timetarget(e->starttime);
	if (e->times_to_eat == 0)
		sem_post(e->stop_sim);
	i = 0;
	while (i < e->n_philosophers)
	{
		sem_wait(e->eaten_enough);
		i++;
	}
	sem_post(e->stop_sim);
	return (NULL);
}

/*
Waits for the binary semaphore stop_sim to be available (i.e. get posted once).
That means a stop condition has been reached and the simulation should end.
Kills the child processes and posts to the eaten_enough semaphore, because:
If the stop signal was sent from a philo process (death of hunger), the other
monitor thread in the main process, the eatenenough_checker, will still be stuck
at its semaphore.
Sending a signal to kill it is not allowed (pthread_cancel). So the stopmonitor
posts n_philosophers to the eatmonitors semaphore which will allow it to
continue.
*/
void	*stopmonitor(void *arg)
{
	t_envl		*e;
	int			i;

	e = (t_envl *)arg;
	i = 0;
	sem_wait(e->stop_sim);
	kill_children(e);
	while (i < e->n_philosophers)
	{
		sem_post(e->eaten_enough);
		i++;
	}
	return (NULL);
}
