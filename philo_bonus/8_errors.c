/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   8_errors.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wmardin <wmardin@student.42wolfsburg.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/21 18:32:22 by wmardin           #+#    #+#             */
/*   Updated: 2022/11/04 08:01:05 by wmardin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

/*
Didn't use printf because I wanted to write to std_err.
Other useful functions to write to std_err not allowed.
Return is necessary to terminate the main (exit not allowed).
*/
void	input_error_exit(char *msg)
{
	write (2, msg, ft_strlen(msg));
	exit(EXIT_FAILURE);
}

void	input_error_philnumber_exit(char *msg)
{
	write (2, msg, ft_strlen(msg));
	write (2, MAX_PHILO, ft_strlen(MAX_PHILO));
	write (2, ".\n", 2);
	exit(EXIT_FAILURE);
}

void	exec_error_exit(char *msg, t_envl *e)
{
	write (2, msg, ft_strlen(msg));
	write (2, "\n", 1);
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
