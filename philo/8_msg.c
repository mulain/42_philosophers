/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   8_msg.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wmardin <wmardin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/21 18:32:22 by wmardin           #+#    #+#             */
/*   Updated: 2023/01/22 11:44:37 by wmardin          ###   ########.fr       */
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
void	msg_input_error(char *msg)
{
	int		retval;

	retval = write (2, msg, ft_strlen(msg));
	(void)retval;
}

void	msg_input_error_philnumber(char *msg)
{
	int		retval;

	retval = write (2, msg, ft_strlen(msg));
	retval = write (2, MAX_PHILO, ft_strlen(MAX_PHILO));
	retval = write (2, ".\n", 2);
	(void)retval;
}

void	msg_exec_error(char *msg, t_envl *e)
{
	int		retval;

	retval = write (2, msg, ft_strlen(msg));
	(void)retval;
	shutdown(e);
}

int	ft_strlen(char *string)
{
	int		i;

	i = 0;
	while (string[i])
		i++;
	return (i);
}
