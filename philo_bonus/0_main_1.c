/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   0_main_1.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wmardin <wmardin@student.42wolfsburg.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/03 09:03:05 by wmardin           #+#    #+#             */
/*   Updated: 2022/11/07 13:26:32 by wmardin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers_bonus.h"

int	main(int argc, char **argv)
{
	t_envl		e;

	setup(&e, argc, argv);
	launch_philoforks(&e);
	launch_monitorthreads(&e);
	wait_children(&e);
	join_threads(&e);
	shutdown(&e);
}

/*
The children will never return from executing the philofunction so
there is no guard against them continuing the execution of the main
function.
*/
void	launch_philoforks(t_envl *e)
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
}

void	launch_monitorthreads(t_envl *e)
{
	if (pthread_create(&e->eatmonitor, NULL, eatmonitor, e))
		exec_error_exit(ERR_THREAD_CREATE, e);
	if (pthread_create(&e->stopmonitor, NULL, stopmonitor, e))
		exec_error_exit(ERR_THREAD_CREATE, e);
}

void	wait_children(t_envl *e)
{
	int		i;

	i = 0;
	while (i < e->n_philosophers)
	{
		if (waitpid(e->pids[i], NULL, 0) == -1)
			exec_error_exit(ERR_WAITPID, e);
		i++;
	}
}

void	join_threads(t_envl *e)
{
	if (pthread_join(e->eatmonitor, NULL))
		exec_error_exit(ERR_THREAD_JOIN, e);
	if (pthread_join(e->stopmonitor, NULL))
		exec_error_exit(ERR_THREAD_JOIN, e);
}
