/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   1_setup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wmardin <wmardin@student.42wolfsburg.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/24 22:05:12 by wmardin           #+#    #+#             */
/*   Updated: 2022/10/28 12:58:48 by wmardin          ###   ########.fr       */
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
	if (!is_positiveint(argv[1]) || !is_intsize(argv[1]))
		error_exit("Only ints >0 are valid for n_philosophers.");
	i = 2;
	while (i < argc)
	{
		if (!is_digits(argv[i]) || !is_intsize(argv[1]))
			error_exit("Only ints >= 0 are valid for times / max_eat.");
		i++;
	}
}

/*
Checks whether the input string consists of only digits; digits must
represent a number > 0. Leading 0s are allowed, e.g. 001.
*/
int	is_positiveint(char *input)
{
	int		i;
	int		nonzero;

	i = 0;
	nonzero = 0;
	if (input[0] == '0')
	{
		while (input[i])
		{
			if (input[i] != '0')
				nonzero = 1;
			i++;
		}
		if (!nonzero)
			return (0);
	}
	i = 0;
	while (input[i])
	{
		if (input[i] < '0' || input[i] > '9')
			return (0);
		i++;
	}
	return (1);
}

/*
Checks whether the input string consists of only digits.
*/
int	is_digits(char *input)
{
	int		i;

	i = 0;
	while (input[i])
	{
		if (input[i] < '0' || input[i] > '9')
			return (0);
		i++;
	}
	return (1);
}

int	is_intsize(char *string)
{
	int		len;

	len = 0;
	while (string[len])
		len++;
	if (len > 11)
		return (0);
	if (string[0] == '-')
	{
		if (len == 11)
		{
			if (ft_strncmp("-2147483648", string, 69) < 0)
				return (0);
		}
		return (1);
	}
	if (len == 11)
		return (0);
	if (len == 10)
	{
		if (ft_strncmp("2147483647", string, 420) < 0)
			return (0);
	}
	return (1);
}

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t			i;
	unsigned char	*u_s1;
	unsigned char	*u_s2;

	i = 0;
	u_s1 = (unsigned char *) s1;
	u_s2 = (unsigned char *) s2;
	while (i < n)
	{
		if (u_s1[i] == 0 && u_s2[i] == 0)
			return (0);
		if (u_s1[i] != u_s2[i])
			return (u_s1[i] - u_s2[i]);
		i++;
	}
	return (0);
}
