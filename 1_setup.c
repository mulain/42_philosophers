/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   1_setup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wmardin <wmardin@student.42wolfsburg.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/24 22:05:12 by wmardin           #+#    #+#             */
/*   Updated: 2022/10/24 22:44:54 by wmardin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	setup(t_envl *e, int argc, char **argv)
{
	int			i;

	check_input(argc, argv);
	parse_input(e, argc, argv);
	pthread_mutex_init(&e->print, NULL);
	e->forks = malloc(e->n_philo * sizeof(pthread_mutex_t));
	e->philostructs = malloc(e->n_philo * sizeof(t_philo));
	i = 0;
	while (i < e->n_philo)
	{
		pthread_mutex_init(e->forks + i, NULL);
		i++;
	}
	i = 0;
	while (i < e->n_philo)
	{
		set_philostruct(e, i);
		i++;
	}
	e->threads = malloc(e->n_philo * sizeof(pthread_t));
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
	e->n_philo = ft_atoi(argv[1]);
	e->time_die = ft_atoi(argv[2]);
	e->time_eat = ft_atoi(argv[3]);
	e->time_sleep = ft_atoi(argv[4]);
	if (argc == 6)
		e->max_eat = ft_atoi(argv[5]);
	else
		e->max_eat = -1;
}

void	set_philostruct(t_envl *e, int i)
{
	e->philostructs[i].name = i + 1;
	e->philostructs[i].time_die = e->time_die;
	e->philostructs[i].time_eat = e->time_eat;
	e->philostructs[i].time_sleep = e->time_sleep;
	e->philostructs[i].max_eat = e->max_eat;
	e->philostructs[i].print = e->print;
	e->philostructs[i].fork_right = e->forks + i;
	if (!i)
		e->philostructs[i].fork_left = e->forks + e->n_philo;
	else
		e->philostructs[i].fork_left = e->forks + i - 1;
}
