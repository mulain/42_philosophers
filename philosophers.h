/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wmardin <wmardin@student.42wolfsburg.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/21 17:59:59 by wmardin           #+#    #+#             */
/*   Updated: 2022/10/22 22:46:35 by wmardin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H

# define PHILOSOPHERS_H
# include <stdlib.h>
# include <stdio.h>
# include <unistd.h>
# include <pthread.h>
# include <sys/time.h>

typedef struct envelope
{
	int				n_philo;
	int				time_die;
	int				time_eat;
	int				time_sleep;
	int				max_eat;
	struct timeval	t;
}	t_envl;

//0_main.c
int		main(int argc, char **argv);
void	setup(t_envl *e, int argc, char **argv);
void	check_input(int argc, char **argv);

//6_utils.c
int		ispositiveint(char *input);
int		ispositiveintorzero(char *input);
int		ft_atoi(const char *nptr);

//8_errors.c
void	error_exit(char *msg);

#endif