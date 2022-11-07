/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   1_setup_1.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wmardin <wmardin@student.42wolfsburg.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/24 22:05:12 by wmardin           #+#    #+#             */
/*   Updated: 2022/11/07 10:38:13 by wmardin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers_bonus.h"

void	setup(t_envl *e, int argc, char **argv)
{
	check_input(argc, argv);
	parse_input(e, argc, argv);
	init_envelopestruct(e);
	close_semaphores(e);
	unlink_semaphores(e);
	open_semaphores(e);
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

void	parse_input(t_envl *e, int argc, char **argv)
{
	e->n_philosophers = ft_atoi(argv[1]);
	e->time_to_die = ft_atoi(argv[2]);
	e->time_to_eat = ft_atoi(argv[3]);
	e->time_to_sleep = ft_atoi(argv[4]);
	if (argc == 6)
		e->times_to_eat = ft_atoi(argv[5]);
	else
		e->times_to_eat = -1;
}
