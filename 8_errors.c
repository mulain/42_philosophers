/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   8_errors.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wmardin <wmardin@student.42wolfsburg.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/21 18:32:22 by wmardin           #+#    #+#             */
/*   Updated: 2022/10/30 13:09:33 by wmardin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	input_error_exit(char *msg)
{
	printf("%s\n", msg);
	exit(EXIT_FAILURE);
}

void	input_error_philo_exit(char *msg)
{
	printf("%s %i.\n", msg, MAX_PHILO);
	exit(EXIT_FAILURE);
}

void	exec_error_exit(char *msg, t_envl *e)
{
	printf("%s\n", msg);
	shutdown(e);
	exit(EXIT_FAILURE);
}
