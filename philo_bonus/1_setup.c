/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   1_setup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wmardin <wmardin@student.42wolfsburg.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/24 22:05:12 by wmardin           #+#    #+#             */
/*   Updated: 2022/12/27 21:45:32 by wmardin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers_bonus.h"

void	setup(t_envl *e, int argc, char **argv)
{
	check_input(argc, argv);
	init_envelopestruct(e, argc, argv);
	open_semaphores_global(e);
	open_semaphores_philo(e);
}

void	check_input(int argc, char **argv)
{
	int		i;

	if (argc != 5 && argc != 6)
		input_error_exit(ERR_ARG_COUNT);
	if (!is_one_to_maxphilo(argv[1]))
		input_error_philnumber_exit(ERR_PHILNUMBER);
	i = 2;
	while (i < argc)
	{
		if (!is_digits(argv[i]) || !is_intsize(argv[i]))
			input_error_exit(ERR_TIMES);
		i++;
	}
}

/*
We are not allowed to use sem_init, only sem_open.
That means it is not possible to specify a process specific semaphore. So it
is necessary to make a distinct semaphore for each process to act as a lock for
the writing / reading of the eating-variables.
Another note: I don't believe it is functionally relevant to protect this data
from race conditions. However the subject stated that no races are allowed,
so here we are.
*/
void	init_envelopestruct(t_envl *e, int argc, char **argv)
{
	e->n_philosophers = ft_atoi(argv[1]);
	e->time_to_die = ft_atoi(argv[2]);
	e->time_to_eat = ft_atoi(argv[3]);
	e->time_to_sleep = ft_atoi(argv[4]);
	if (argc == 6)
		e->times_to_eat = ft_atoi(argv[5]);
	else
		e->times_to_eat = -1;
	e->starttime = calc_starttime(e);
	e->last_eat = e->starttime;
	e->times_eaten = 0;
	e->pids = malloc(e->n_philosophers * sizeof(pid_t));
	if (!e->pids)
		exec_error_exit(ERR_MALLOC, e);
	e->eat_locks = malloc((e->n_philosophers) * sizeof(sem_t *));
	if (!e->eat_locks)
		exec_error_exit(ERR_MALLOC, e);
}

time_t	calc_starttime(t_envl *e)
{
	int		offset;

	offset = e->n_philosophers * 50;
	if (offset > 2000)
		offset = 2000;
	if (offset < 500)
		offset = 500;
	return (get_time_ms() + offset);
}
