/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   1_setup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wmardin <wmardin@student.42wolfsburg.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/24 22:05:12 by wmardin           #+#    #+#             */
/*   Updated: 2022/10/28 20:02:35 by wmardin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	setup(t_envl *e, int argc, char **argv)
{
	check_input(argc, argv);
	parse_input(e, argc, argv);
	init_mutexes(e);
	set_philostructs(e);
	e->threads = malloc(e->n_philosophers * sizeof(pthread_t));
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
		error_exit("Wrong number of arguments.");
	if (!is_positivenum(argv[1]) || !is_intsize(argv[1]))
		error_exit("Only ints >0 are valid for n_philosophers.");
	i = 2;
	while (i < argc)
	{
		if (!is_digits(argv[i]) || !is_intsize(argv[1]))
			error_exit("Only ints >= 0 are valid for times / max_eat.");
		i++;
	}
}

void	parse_input(t_envl *e, int argc, char **argv)
{
	e->common->time_to_die = ft_atoi(argv[2]);
	e->common->time_to_eat = ft_atoi(argv[3]);
	e->common->time_to_sleep = ft_atoi(argv[4]);
	if (argc == 6)
		e->common->max_eat = ft_atoi(argv[5]);
	else
		e->common->max_eat = -1;
	e->n_philosophers = ft_atoi(argv[1]);
}

void	init_mutexes(t_envl *e)
{
	int		i;

	e->forks = malloc(e->n_philosophers * sizeof(pthread_mutex_t));
	i = 0;
	while (i < e->n_philosophers)
	{
		pthread_mutex_init(&e->forks[i], NULL);
		i++;
	}
}

/*
fork: 	left		right	left		right	left			right
fork_i:	n-1			0		0			1		i-1(=n-2)		i(=n-1)
i:				0					1					n-1
id:				1					2					n
*/
void	set_philostructs(t_envl *e)
{
	int		i;

	i = 0;
	e->philostructs = malloc(e->n_philosophers * sizeof(t_philo));
	while (i < e->n_philosophers)
	{
		e->philostructs[i].id = i + 1;
		e->philostructs[i].common = e->common;
		e->philostructs[i].fork_right = &e->forks[i];
		if (i == 0)
			e->philostructs[i].fork_left = &e->forks[e->n_philosophers - 1];
		else
			e->philostructs[i].fork_left = &e->forks[i - 1];
		i++;
	}
}
