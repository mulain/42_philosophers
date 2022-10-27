/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   1_setup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wmardin <wmardin@student.42wolfsburg.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/24 22:05:12 by wmardin           #+#    #+#             */
/*   Updated: 2022/10/27 18:48:53 by wmardin          ###   ########.fr       */
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
	if (!ispositiveint(argv[1]) || !isofintsize(argv[1]))
		error_exit("Only positive ints are valid for n_philosophers.");
	i = 2;
	while (i < argc)
	{
		if (!ispositiveintorzero(argv[i]) || !isofintsize(argv[1]))
			error_exit("Only ints >= 0 are valid for time / max_eat.");
		i++;
	}
}

/*
Checks whether the input string represents a positive integer.
*/
int	is_positiveint(char *input)
{
	int		i;

	if (input[0] == '0')
		return (0);
	i = 0;
	while (input[i])
	{
		if (input[i] < 47 || input[i] > 58)
			return (0);
		i++;
	}
	return (1);
}

/*
Checks whether the input string represents a positive integer or 0.
*/
int	is_positiveintorzero(char *input)
{
	int		i;

	i = 0;
	while (input[i])
	{
		if (input[i] < 47 || input[i] > 58)
			return (0);
		i++;
	}
	return (1);
}

int	is_intsize(char *argv)
{
	int		len;

	len = ft_strlen(argv);
	if (len > 11)
		return (0);
	if (argv[0] == '-')
	{
		if (len == 11)
		{
			if (ft_strncmp("-2147483648", argv, 69) < 0)
				return (0);
		}
		return (1);
	}
	if (len == 11)
		return (0);
	if (len == 10)
	{
		if (ft_strncmp("2147483647", argv, 420) < 0)
			return (0);
	}
	return (1);
}
