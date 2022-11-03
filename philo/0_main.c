/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   0_main.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wmardin <wmardin@student.42wolfsburg.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/21 17:59:27 by wmardin           #+#    #+#             */
/*   Updated: 2022/11/03 08:53:40 by wmardin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

/*
Mandatory part of the project doesn't allow function exit, so the various parts
of the main have a return to stop execution of the main.

General points:
- 	For philosphers not to die, <time_to_die> has to be > 2 x <time_to_eat>
	because the philosophers have to be split in at least 2 groups that can
	only eat sequentially.
-	In case of uneven number: There are 3 groups, so <time_to_die> has to be
	>= 3 x <time_to_eat>.
-	<time_to_sleep> of a philospher group can be used by the next group to eat,
	so it isn't wasted time.
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

void	shutdown(t_envl *e)
{
	int		i;

	if (e->mutex_init)
	{
		pthread_mutex_destroy(&e->global.printlock);
		pthread_mutex_destroy(&e->global.stoplock);
		i = 0;
		while (i < e->n_philosophers)
		{
			pthread_mutex_destroy(&e->forks[i]);
			pthread_mutex_destroy(&e->last_eat_locks[i]);
			i++;
		}
	}
	if (e->forks)
		free(e->forks);
	if (e->last_eat_locks)
		free(e->last_eat_locks);
	if (e->threads)
		free(e->threads);
	if (e->philo)
		free(e->philo);
}

time_t	get_time_ms(void)
{
	struct timeval		time;

	gettimeofday(&time, NULL);
	return (time.tv_sec * 1000 + time.tv_usec / 1000);
}
