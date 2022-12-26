/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   0_main.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wmardin <wmardin@student.42wolfsburg.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/21 17:59:27 by wmardin           #+#    #+#             */
/*   Updated: 2022/12/25 20:00:34 by wmardin          ###   ########.fr       */
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

	if (setup(&e, argc, argv))
		return (1);
	if (launch_threads(&e))
		return (1);
	if (collect_threads(&e))
		return (1);
	shutdown(&e);
	return (0);
}

bool	launch_threads(t_envl *e)
{
	int		i;

	if (pthread_create(&e->monitor, NULL, monitor, e))
		return (msg_exec_error(ERR_THREAD_CREATE, e), true);
	i = 0;
	while (i < e->n_philosophers)
	{
		if (pthread_create(&e->philo[i].thread, NULL, e->philofunction,
				&e->philo[i]))
			return (msg_exec_error(ERR_THREAD_CREATE, e), true);
		i++;
	}
	return (false);
}

bool	collect_threads(t_envl *e)
{
	int		i;

	i = 0;
	while (i < e->n_philosophers)
	{
		if (pthread_join(e->philo[i].thread, NULL))
			return (msg_exec_error(ERR_THREAD_JOIN, e), true);
		i++;
	}
	if (pthread_join(e->monitor, NULL))
		return (msg_exec_error(ERR_THREAD_JOIN, e), true);
	return (false);
}

void	shutdown(t_envl *e)
{
	int		i;

	if (e->mutex_init)
	{
		pthread_mutex_destroy(&e->printlock);
		pthread_mutex_destroy(&e->stoplock);
		i = 0;
		while (i < e->n_philosophers)
		{
			pthread_mutex_destroy(&e->philo[i].fork);
			pthread_mutex_destroy(&e->philo[i].last_eat_lock);
			i++;
		}
	}
	if (e->philo)
		free(e->philo);
}

time_t	get_time_ms(void)
{
	struct timeval		time;

	gettimeofday(&time, NULL);
	return (time.tv_sec * 1000 + time.tv_usec / 1000);
}
