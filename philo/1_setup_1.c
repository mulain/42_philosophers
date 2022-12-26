/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   1_setup_1.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wmardin <wmardin@student.42wolfsburg.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/24 22:05:12 by wmardin           #+#    #+#             */
/*   Updated: 2022/12/25 19:51:15 by wmardin          ###   ########.fr       */
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