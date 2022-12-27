/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   1_setup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wmardin <wmardin@student.42wolfsburg.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/24 22:05:12 by wmardin           #+#    #+#             */
/*   Updated: 2022/12/27 12:28:42 by wmardin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

/*
exit function not allowed, so there has to be a return to stop execution if
necessary.
*/
bool	setup(t_envl *e, int argc, char **argv)
{
	if (check_input(argc, argv))
		return (true);
	if (init_envelopestruct(e, argc, argv))
		return (true);
	if (init_philostructs(e))
		return (true);
	return (false);
}

/*
input:			philo	n_phil	die		eat		sleep	max_eat
argv_index:		argv_0	argv_1	argv_2	argv_3	argv_4	argv_5
argc_value:		argc_1	argc_2	argc_3	argc_4	argc_5	argc_6
*/
bool	check_input(int argc, char **argv)
{
	int		i;

	if (argc != 5 && argc != 6)
		return (msg_input_error(ERR_ARG_COUNT), true);
	if (!is_one_to_maxphilo(argv[1]))
		return (msg_input_error_philnumber(ERR_PHILNUMBER), true);
	i = 2;
	while (i < argc)
	{
		if (!is_digits(argv[i]) || !is_intsize(argv[i]))
			return (msg_input_error(ERR_TIMES), true);
		i++;
	}
	return (false);
}

bool	init_envelopestruct(t_envl *e, int argc, char **argv)
{
	e->n_philosophers = ft_atoi(argv[1]);
	e->philo = malloc(e->n_philosophers * sizeof(t_philo));
	if (!e->philo)
		return (msg_exec_error(ERR_MALLOC, e), true);
	e->mutex_init = false;
	e->time_to_die = ft_atoi(argv[2]);
	e->time_to_eat = ft_atoi(argv[3]);
	e->time_to_sleep = ft_atoi(argv[4]);
	if (argc == 6)
		e->times_to_eat = ft_atoi(argv[5]);
	else
		e->times_to_eat = -1;
	e->starttime = calc_starttime(e);
	e->stop = false;
	if (pthread_mutex_init(&e->printlock, NULL))
		return (msg_exec_error(ERR_MUTEX_INIT, e), true);
	if (pthread_mutex_init(&e->stoplock, NULL))
		return (false);
	if (e->n_philosophers == 1)
		e->philofunction = philosopher_solo;
	else
		e->philofunction = philosopher;
	return (false);
}

bool	init_philostructs(t_envl *e)
{
	int		i;

	i = -1;
	while (++i < e->n_philosophers)
	{
		e->philo[i].id = i + 1;
		e->philo[i].times_eaten = 0;
		e->philo[i].starttime = e->starttime;
		e->philo[i].last_eat = e->starttime;
		e->philo[i].time_to_die = e->time_to_die;
		e->philo[i].time_to_eat = e->time_to_eat;
		e->philo[i].time_to_sleep = e->time_to_sleep;
		e->philo[i].times_to_eat = e->times_to_eat;
		pthread_mutex_init(&e->philo[i].fork, NULL);
		e->philo[i].fork_left = &e->philo[i].fork;
		e->philo[i].fork_right = &e->philo[(i + 1) % e->n_philosophers].fork;
		pthread_mutex_init(&e->philo[i].last_eat_lock, NULL);
		e->philo[i].printlock = &e->printlock;
		e->philo[i].stoplock = &e->stoplock;
		e->philo[i].stop = &e->stop;
	}
	return (false);
}

time_t	calc_starttime(t_envl *e)
{
	int		offset;

	offset = e->n_philosophers * 50;
	if (offset > 2000)
		offset = 2000;
	else if (offset < 100)
		offset = 100;
	return (get_time_ms() + offset);
}
