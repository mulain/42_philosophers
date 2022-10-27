/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   1a_setup.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wmardin <wmardin@student.42wolfsburg.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/25 16:22:47 by wmardin           #+#    #+#             */
/*   Updated: 2022/10/27 18:49:01 by wmardin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"


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


void	init_mutexes(t_envl *e)
{
	int		i;

	e->forks = malloc(e->n_philosophers * sizeof(pthread_mutex_t));
	i = 0;
	while (i < e->n_philosophers)
	{
		pthread_mutex_init(e->forks + i, NULL);
		i++;
	}
}
