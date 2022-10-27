/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wmardin <wmardin@student.42wolfsburg.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/21 17:59:59 by wmardin           #+#    #+#             */
/*   Updated: 2022/10/25 16:46:51 by wmardin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H

# define PHILOSOPHERS_H
# include <stdlib.h>
# include <stdio.h>
# include <unistd.h>
# include <pthread.h>
# include <sys/time.h>

typedef struct common
{
	int					time_to_die;
	int					time_to_eat;
	int					time_to_sleep;
	int					max_eat;
	time_t				starttime;
	pthread_mutex_t		*printlock;
}	t_common;

typedef struct philostruct
{
	int					id;
	t_common			*info;
	time_t				timestamp;
	pthread_mutex_t		*fork_right;
	pthread_mutex_t		*fork_left;
}	t_philo;

typedef struct envelope
{
	int					n_philosophers;
	pthread_t			*threads;
	pthread_mutex_t		*forks;
	t_common			*info;
	t_philo				*philostructs;
}	t_envl;

//0_main.c
int		main(int argc, char **argv);

//1_setup.c
void	setup(t_envl *e, int argc, char **argv);
void	check_input(int argc, char **argv);
void	parse_input(t_envl *e, int argc, char **argv);
void	set_philostruct(t_envl *e, int i);

//1a_setup.c
void	init_mutexes(t_envl *e);

//2_threadfunctions.c
void	*philosopher(void *arg);
void	taken_a_fork(int timestamp, int name, pthread_mutex_t *print);
void	is_eating(int timestamp, int name, pthread_mutex_t *print);

//6_utils.c
int		ispositiveint(char *input);
int		ispositiveintorzero(char *input);
int		ft_atoi(const char *nptr);

//8_errors.c
void	error_exit(char *msg);

#endif