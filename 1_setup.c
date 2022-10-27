/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   1_setup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wmardin <wmardin@student.42wolfsburg.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/24 22:05:12 by wmardin           #+#    #+#             */
/*   Updated: 2022/10/25 16:35:13 by wmardin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	setup(t_envl *e, int argc, char **argv)
{
	check_input(argc, argv);
	parse_input(e, argc, argv);
	init_mutexes(e);
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
	if (!ispositiveint(argv[1]))
		error_exit("Only positive integers are valid for n_philosophers.");
	i = 2;
	while (i < argc)
	{
		if (!ispositiveintorzero(argv[i]))
			error_exit("Only integers >= 0 are valid for time / max_eat.");
		i++;
	}
}

void	parse_input(t_envl *e, int argc, char **argv)
{
	e->n_philosophers = ft_atoi(argv[1]);
	e->info->time_to_die = ft_atoi(argv[2]);
	e->info->time_to_eat = ft_atoi(argv[3]);
	e->info->time_to_sleep = ft_atoi(argv[4]);
	if (argc == 6)
		e->info->max_eat = ft_atoi(argv[5]);
	else
		e->info->max_eat = -1;
}

void	set_philostruct(t_envl *e, int i)
{
	i = 0;
	e->philostructs = malloc(e->n_philosophers * sizeof(t_philo));
	while (i < e->n_philosophers)
	{
		e->philostructs[i].id = i + 1;
		e->philostructs->info = e->info;
		e->philostructs[i].fork_right = e->forks + i;
		if (!i)
			e->philostructs[i].fork_left = e->forks + e->n_philosophers;
		else
			e->philostructs[i].fork_left = e->forks + i - 1;
		i++;
	}
}
