/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   1_setup_1.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wmardin <wmardin@student.42wolfsburg.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/24 22:05:12 by wmardin           #+#    #+#             */
/*   Updated: 2022/10/30 18:50:27 by wmardin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

/*
Checks input.
Initializes variables relevant to shutdown functions. These guard the shutdown
processes (free, mutex destroy) in case of termination before their targets
have been populated.
*/
void	setup(t_envl *e, int argc, char **argv)
{
	int		offset;

	check_input(argc, argv);
	parse_input(e, argc, argv);
	e->threads = NULL;
	e->forks = NULL;
	e->mutex_init = false;
	e->philostructs = NULL;
	e->threads = malloc(e->n_philosophers * sizeof(pthread_t));
	if (!e->threads)
		exec_error_exit("Error: malloc", e);
	e->common.time_to_think = calc_thinktime(e);
	offset = e->n_philosophers * 20;
	if (offset > 1000)
		offset = 1000;
	if (offset < 100)
		offset = 100;
	e->common.starttime = get_time_ms() + offset;
	e->common.stop = false;
	init_mutexes(e);
	set_philostructs(e);
}

/*
input:			philo	n_phil	die		eat		sleep	max_eat
argv_index:		argv_0	argv_1	argv_2	argv_3	argv_4	argv_5
argc_value:		argc_1	argc_2	argc_3	argc_4	argc_5	argc_6
*/
void	check_input(int argc, char **argv)
{
	int		i;

	if (argc != 5 && argc != 6)
		input_error_exit(MSG_ARG_COUNT);
	if (!is_one_to_maxphilo(argv[1]))
		input_error_philo_exit(MSG_NUMBER_PHIL);
	i = 2;
	while (i < argc)
	{
		if (!is_digits(argv[i]) || !is_intsize(argv[i]))
			input_error_exit(MSG_TIMES);
		i++;
	}
}

void	parse_input(t_envl *e, int argc, char **argv)
{
	e->n_philosophers = ft_atoi(argv[1]);
	e->common.time_to_die = ft_atoi(argv[2]);
	e->common.time_to_eat = ft_atoi(argv[3]);
	e->common.time_to_sleep = ft_atoi(argv[4]);
	if (argc == 6)
		e->common.times_to_eat = ft_atoi(argv[5]);
	else
		e->common.times_to_eat = -1;
}

void	init_mutexes(t_envl *e)
{
	int		i;
	int		mutex_init_return;

	mutex_init_return = 0;
	e->forks = malloc(e->n_philosophers * sizeof(pthread_mutex_t));
	e->last_eat_locks = malloc(e->n_philosophers * sizeof(pthread_mutex_t));
	if (!e->forks || !e->last_eat_locks)
		exec_error_exit("Error: malloc", e);
	i = 0;
	while (i < e->n_philosophers)
	{
		mutex_init_return += pthread_mutex_init(&e->forks[i], NULL);
		mutex_init_return += pthread_mutex_init(&e->last_eat_locks[i], NULL);
		i++;
	}
	mutex_init_return += pthread_mutex_init(&e->common.printlock, NULL);
	mutex_init_return += pthread_mutex_init(&e->common.stoplock, NULL);
	if (mutex_init_return)
		exec_error_exit("Error: pthread_mutex_init", e);
	e->mutex_init = true;
}

void	set_philostructs(t_envl *e)
{
	int		i;

	i = 0;
	e->philostructs = malloc(e->n_philosophers * sizeof(t_philo));
	if (!e->philostructs)
		exec_error_exit("Error: malloc", e);
	while (i < e->n_philosophers)
	{
		e->philostructs[i].id = i + 1;
		e->philostructs[i].common = &e->common;
		e->philostructs[i].fork_right = &e->forks[i];
		if (i == 0)
			e->philostructs[i].fork_left = &e->forks[e->n_philosophers - 1];
		else
			e->philostructs[i].fork_left = &e->forks[i - 1];
		e->philostructs[i].last_eat = e->common.starttime;
		e->philostructs[i].last_eat_lock = &e->last_eat_locks[i];
		e->philostructs[i].times_eaten = 0;
		i++;
	}
}
