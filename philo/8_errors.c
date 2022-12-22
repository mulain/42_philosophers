/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   8_errors.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wmardin <wmardin@student.42wolfsburg.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/21 18:32:22 by wmardin           #+#    #+#             */
/*   Updated: 2022/12/22 18:24:14 by wmardin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

/*
Didn't use printf because I wanted to write to std_err.
Other useful functions to write to std_err not allowed.
Return is necessary to terminate the main (exit not allowed).
Storing write return in retval and voiding it to silence warnings
when compiling with -O2 / -O3.
*/
bool	input_error_exit(char *msg)
{
	int		retval;

	retval = write (2, msg, ft_strlen(msg));
	(void)retval;
	return (false);
}

bool	input_error_philnumber_exit(char *msg)
{
	int		retval;

	retval = write (2, msg, ft_strlen(msg));
	retval = write (2, MAX_PHILO, ft_strlen(MAX_PHILO));
	retval = write (2, ".\n", 2);
	(void)retval;
	return (false);
}

bool	exec_error_exit(char *msg, t_envl *e)
{
	int		retval;

	retval = write (2, msg, ft_strlen(msg));
	(void)retval;
	shutdown(e);
	return (false);
}

int	ft_strlen(char *string)
{
	int		i;

	i = 0;
	while (string[i])
		i++;
	return (i);
}
