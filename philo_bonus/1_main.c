/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   1_main.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wmardin <wmardin@student.42wolfsburg.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/03 09:03:05 by wmardin           #+#    #+#             */
/*   Updated: 2022/11/05 12:28:33 by wmardin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers_bonus.h"

int	main(int argc, char **argv)
{
	t_envl		e;
	int			i;

	setup(&e, argc, argv);
	i = 0;
	while (i < e.n_philosophers)
	{
		e.philo.id = i + 1;
		e.pids[i] = fork();
		if (e.pids[i] == -1)
			exec_error_exit("Error: fork\n", &e);
		if (e.pids[i] == 0)
		{
			e.philofunction(&e);
			exit(EXIT_SUCCESS);
		}
		i++;
	}

	shutdown(&e);
}

void	shutdown(t_envl *e)
{
	int		i;

	if (e->sem_init)
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
