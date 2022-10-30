/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   0_main.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wmardin <wmardin@student.42wolfsburg.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/21 17:59:27 by wmardin           #+#    #+#             */
/*   Updated: 2022/10/30 11:46:40 by wmardin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	main(int argc, char **argv)
{
	t_envl		e;

	setup(&e, argc, argv);
	launch_threads(&e);
	collect_threads(&e);
	shutdown(&e);
}

/*
Launches the monitor thread.
Launches the philosopher threads.
If only one philosopher is present, it only has one fork and will die at
the correct time (function of monitor). But the philosopher thread would
be stuck trying to get the 2nd fork. We are not allowed to kill threads,
so the lone philo thread must be hard coded to not try to get a 2nd fork
in order to not be stuck and be able terminate on its own.
*/
void	launch_threads(t_envl *e)
{
	int		i;
	void	*(*threadfunction)();

	if (pthread_create(&e->monitor, NULL, monitor, e))
		exec_error_exit("Error: pthread_create", e);
	if (e->n_philosophers == 1)
		threadfunction = philosopher_solo;
	else
		threadfunction = philosopher;
	i = 0;
	while (i < e->n_philosophers)
	{
		if (pthread_create(e->threads + i, NULL, threadfunction,
				&e->philostructs[i]))
			exec_error_exit("Error: pthread_create", e);
		i++;
	}
}

void	collect_threads(t_envl *e)
{
	int		i;

	i = 0;
	while (i < e->n_philosophers)
	{
		if (pthread_join(e->threads[i], NULL))
			exec_error_exit("Error: pthread_join", e);
		i++;
	}
	if (pthread_join(e->monitor, NULL))
		exec_error_exit("Error: pthread_join", e);
}
