/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   8_errors.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wmardin <wmardin@student.42wolfsburg.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/21 18:32:22 by wmardin           #+#    #+#             */
/*   Updated: 2022/11/13 06:29:11 by wmardin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers_bonus.h"

/*
Didn't use printf because I wanted to write to std_err.
Other useful functions to write to std_err not allowed.
Return is necessary to terminate the main (exit not allowed).
retval is necessary to silence warnings using -O3 flag.
(Didn't figure out how to directly void write...)
*/
void	input_error_exit(char *msg)
{
	int		retval;

	retval = write (2, msg, ft_strlen(msg));
	(void)retval;
	exit(EXIT_FAILURE);
}

void	input_error_philnumber_exit(char *msg)
{
	int		retval;

	retval = write (2, msg, ft_strlen(msg));
	retval = write (2, MAX_PHILO, ft_strlen(MAX_PHILO));
	retval = write (2, ".\n", 2);
	(void)retval;
}

void	exec_error_exit(char *msg, t_envl *e)
{
	int		retval;

	retval = write (2, msg, ft_strlen(msg));
	(void)retval;
	shutdown(e);
	exit(EXIT_FAILURE);
}
