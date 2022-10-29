/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wmardin <wmardin@student.42wolfsburg.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/21 17:59:59 by wmardin           #+#    #+#             */
/*   Updated: 2022/10/29 16:28:51 by wmardin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H

# define PHILOSOPHERS_H
# include <stdlib.h>
# include <stdio.h>
# include <unistd.h>
# include <pthread.h>
# include <sys/time.h>
# include <stdbool.h>

# define MSG_ARG_COUNT "Wrong number of arguments. Usage:\n\
./philosophers <number_of_philosophers> <time_to_die> <time_to_eat> <time_to_sleep> \
[<number_of_times_each_philosopher_must_eat>]"
# define MSG_NUMBER_PHIL "<number_of_philosophers> must be a positive integer in \
int range: > 0 and < 2147483648."
# define MSG_TIMES "<time_to_[...]> and <number_of_times_each_philosopher_must_eat> \
must be positive integers in int range or zero: >= 0 and < 2147483648."

typedef struct common
{
	int					time_to_die;
	int					time_to_eat;
	int					time_to_sleep;
	int					times_to_eat;
	int					time_to_think;
	bool				stop;
	time_t				starttime;
	pthread_mutex_t		printlock;
}	t_common;

typedef struct philostruct
{
	int					id;
	t_common			*common;
	pthread_mutex_t		*fork_right;
	pthread_mutex_t		*fork_left;
	time_t				last_eat;
	int					times_eaten;
}	t_philo;

typedef struct envelope
{
	int					n_philosophers;
	pthread_t			*threads;
	pthread_t			monitor;
	pthread_mutex_t		*forks;
	t_common			common;
	t_philo				*philostructs;
}	t_envl;

//0_main.c
int		main(int argc, char **argv);
time_t	get_time_ms(void);

//1_setup_1.c
void	setup(t_envl *e, int argc, char **argv);
void	check_input(int argc, char **argv);
void	parse_input(t_envl *e, int argc, char **argv);
void	init_mutexes(t_envl *e);
void	set_philostructs(t_envl *e);

//1_setup_2.c
int		is_positivenum(char *input);
int		is_digits(char *input);
int		is_intsize(char *argv);
int		ft_strncmp(const char *s1, const char *s2, size_t n);
int		ft_atoi(const char *nptr);

//1_setup_3.c
void	calc_thinktime(t_envl *e);

//2_thread_philosopher.c
void	*philosopher(void *arg);
void	take_forks(t_philo *p);
void	eat_sleep_think(t_philo *p);
void	release_forks(t_philo *p);

//3_thread_monitor.c
void	*monitor(void *arg);

//4_thread_utils.c
time_t	broadcast(char *msg, t_philo *p);
bool	wait_timetarget(time_t timetarget, t_philo *p);

//6_utils.c
time_t	get_timestamp(time_t starttime);


//8_errors.c
void	error_exit(char *msg);

#endif