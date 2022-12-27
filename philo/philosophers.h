/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wmardin <wmardin@student.42wolfsburg.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/21 17:59:59 by wmardin           #+#    #+#             */
/*   Updated: 2022/12/27 12:46:11 by wmardin          ###   ########.fr       */
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

# define MAX_PHILO "200"
# define ERR_ARG_COUNT "Wrong argument count. Usage:\n\
./philo <number_of_philosophers> <time_to_die> <time_to_eat> <time_to_sleep> \
[<number_of_times_each_philosopher_must_eat>]\n"
# define ERR_PHILNUMBER "<number_of_philosophers> must be from 1 to "
# define ERR_TIMES "<time_to_[...]> and <number_of_times_each_philosopher_must_eat> \
must be positive integers in int range or zero: >= 0 and < 2147483648.\n"
# define ERR_MALLOC "Error: malloc\n"
# define ERR_MUTEX_INIT "Error: pthread_mutex_init\n"
# define ERR_THREAD_CREATE "Error: pthread_create\n"
# define ERR_THREAD_JOIN "Error: pthread_join\n"

typedef struct philosopher_thread
{
	pthread_t			thread;
	pthread_mutex_t		fork;
	int					id;
	int					times_eaten;
	time_t				starttime;
	time_t				last_eat;
	int					time_to_die;
	int					time_to_eat;
	int					time_to_sleep;
	int					times_to_eat;
	pthread_mutex_t		*fork_right;
	pthread_mutex_t		*fork_left;
	pthread_mutex_t		last_eat_lock;
	pthread_mutex_t		*printlock;
	pthread_mutex_t		*stoplock;
	bool				*stop;
}	t_philo;

typedef struct envelope
{
	int					n_philosophers;
	t_philo				*philo;
	pthread_t			monitor;
	int					time_to_die;
	int					time_to_eat;
	int					time_to_sleep;
	int					times_to_eat;
	bool				stop;
	time_t				starttime;
	pthread_mutex_t		printlock;
	pthread_mutex_t		stoplock;
	void				*(*philofunction)();
}	t_envl;

//0_main.c
int		main(int argc, char **argv);
bool	launch_threads(t_envl *e);
bool	collect_threads(t_envl *e);
void	shutdown(t_envl *e);
time_t	get_time_ms(void);

//1_setup_1.c
bool	setup(t_envl *e, int argc, char **argv);
bool	check_input(int argc, char **argv);
void	parse_input(t_envl *e, int argc, char **argv);

//1_setup_2.c
bool	init_envelopestruct(t_envl *e, int argc, char **argv);
bool	init_mutexes(t_envl *e);
bool	init_philostructs(t_envl *e);
time_t	calc_starttime(t_envl *e);

//1_setup_3.c
int		is_one_to_maxphilo(char *input);
int		is_digits(char *input);
int		is_intsize(char *argv);
int		ft_strncmp(const char *s1, const char *s2, size_t n);
int		ft_atoi(const char *nptr);

//2_thread_philosopher_1.c
void	*philosopher(void *arg);
void	eat_sleep_think(t_philo *p);
int		calc_thinktime(t_philo *p);
bool	wait_timetarget(time_t timetarget, t_philo *p);
void	*philosopher_solo(void *arg);

//2_thread_philosopher_2.c
void	switch_forks(t_philo *p);
void	take_forks(t_philo *p);
void	release_forks(t_philo *p);
bool	check_stopped(t_philo *p);
time_t	broadcast(char *msg, t_philo *p);

//3_thread_monitor.c
void	*monitor(void *arg);
bool	check_death(t_envl *e, int i);
bool	check_sated(t_envl *e, int i);
void	set_stop(t_envl *e);

//8_msg.c
void	msg_input_error(char *msg);
void	msg_input_error_philnumber(char *msg);
void	msg_exec_error(char *msg, t_envl *e);
int		ft_strlen(char *string);

#endif