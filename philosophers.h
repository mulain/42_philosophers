/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wmardin <wmardin@student.42wolfsburg.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/21 17:59:59 by wmardin           #+#    #+#             */
/*   Updated: 2022/10/28 19:37:58 by wmardin          ###   ########.fr       */
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
	pthread_mutex_t		*printlock; //really needed?
}	t_common;

typedef struct philostruct
{
	int					id;
	t_common			*common;
	pthread_mutex_t		*fork_right;
	pthread_mutex_t		*fork_left;
	time_t				last_eat;
	int					n_eat;
}	t_philo;

typedef struct envelope
{
	int					n_philosophers;
	pthread_t			*threads;
	pthread_mutex_t		*forks;
	t_common			*common;
	t_philo				*philostructs;
}	t_envl;

//0_main.c
int		main(int argc, char **argv);
time_t	get_time_ms(void);

//1_setup.c
void	setup(t_envl *e, int argc, char **argv);
void	check_input(int argc, char **argv);
void	parse_input(t_envl *e, int argc, char **argv);
void	init_mutexes(t_envl *e);
void	set_philostructs(t_envl *e);

//1a_setup_util.c
int		is_positivenum(char *input);
int		is_digits(char *input);
int		is_intsize(char *argv);
int		ft_strncmp(const char *s1, const char *s2, size_t n);
int		ft_atoi(const char *nptr);

//2_threadfunctions.c
void	*philosopher(void *arg);
void	wait_start(t_philo *p);
void	take_fork_right(t_philo *p);
void	take_fork_left(t_philo *p);
void	eat(t_philo *p);
void	p_sleep(t_philo *p);

//6_utils.c
time_t	get_timestamp(time_t starttime);
void	broadcast(time_t timestamp, char *msg);

//8_errors.c
void	error_exit(char *msg);

#endif