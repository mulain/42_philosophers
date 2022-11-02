/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   0_main.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wmardin <wmardin@student.42wolfsburg.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/21 17:59:27 by wmardin           #+#    #+#             */
/*   Updated: 2022/11/02 13:12:18 by wmardin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

/*
Mandatory part of the project doesn't allow function exit, so the various parts
of the main have a return to stop execution of the main.
*/
int	main(int argc, char **argv)
{
	t_envl		e;

	if (!setup(&e, argc, argv))
		return (EXIT_FAILURE);
	if (!launch_threads(&e))
		return (EXIT_FAILURE);
	if (!collect_threads(&e))
		return (EXIT_FAILURE);
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
bool	launch_threads(t_envl *e)
{
	int		i;

	if (pthread_create(&e->monitor, NULL, monitor, e))
		return (exec_error_exit(ERR_THREAD_CREATE, e));
	i = 0;
	while (i < e->n_philosophers)
	{
		if (pthread_create(e->threads + i, NULL, e->philofunction,
				&e->philo[i]))
			return (exec_error_exit(ERR_THREAD_CREATE, e));
		i++;
	}
	return (true);
}

bool	collect_threads(t_envl *e)
{
	int		i;

	i = 0;
	while (i < e->n_philosophers)
	{
		if (pthread_join(e->threads[i], NULL))
			return (exec_error_exit(ERR_THREAD_JOIN, e));
		i++;
	}
	if (pthread_join(e->monitor, NULL))
		return (exec_error_exit(ERR_THREAD_JOIN, e));
	return (true);
}

time_t	get_time_ms(void)
{
	struct timeval		time;

	gettimeofday(&time, NULL);
	return (time.tv_sec * 1000 + time.tv_usec / 1000);
}
