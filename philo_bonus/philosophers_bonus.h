/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers_bonus.h                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wmardin <wmardin@student.42wolfsburg.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/21 17:59:59 by wmardin           #+#    #+#             */
/*   Updated: 2022/11/04 08:22:57 by wmardin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_BONUS_H

# define PHILOSOPHERS_BONUS_H
# include <stdlib.h>
# include <stdio.h>
# include <unistd.h>
# include <semaphore.h>
# include <sys/time.h>
# include <stdbool.h>
# include <sys/wait.h>
# include <signal.h>

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

typedef struct philosopher_process
{
	int			id;
	int			time_to_die;
	int			time_to_eat;
	int			time_to_sleep;
	int			times_to_eat;
	time_t		starttime;
	sem_t		printlock;
	sem_t		stoplock;
	t_global	*global;
	sem_t		*forks;
	time_t		last_eat;
	sem_t		*last_eat_lock;
	int			times_eaten;
}	t_philo;

typedef struct envelope
{
	int			n_philosophers;
	pid_t		*pids;
	sem_t		forks;
	sem_t		*last_eat_locks;
	t_global	global;
	bool		sem_init;
	t_philo		philo;
	void		*(*philofunction)();
}	t_envl;

//0_main.c
int		main(int argc, char **argv);
bool	launch_threads(t_envl *e);
bool	collect_threads(t_envl *e);
void	shutdown(t_envl *e);
time_t	get_time_ms(void);

//1_setup_1.c
void	setup(t_envl *e, int argc, char **argv);
void	check_input(int argc, char **argv);
void	parse_input(t_envl *e, int argc, char **argv);

//1_setup_2.c
void	init_envelopestruct(t_envl *e);
bool	init_mutexes(t_envl *e);
bool	init_philostructs(t_envl *e);

//1_setup_3.c
int		is_one_to_maxphilo(char *input);
int		is_digits(char *input);
int		is_intsize(char *argv);
int		ft_strncmp(const char *s1, const char *s2, size_t n);
int		ft_atoi(const char *nptr);

//2_process_philosopher_1.c
void	philosopher(void);
void	philosopher_solo(void);

//8_errors.c
void	input_error_exit(char *msg);
void	input_error_philnumber_exit(char *msg);
bool	exec_error_exit(char *msg, t_envl *e);
int		ft_strlen(char *string);

#endif