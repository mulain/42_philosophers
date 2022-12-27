/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers_bonus.h                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wmardin <wmardin@student.42wolfsburg.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/21 17:59:59 by wmardin           #+#    #+#             */
/*   Updated: 2022/12/27 21:47:21 by wmardin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_BONUS_H
# define PHILOSOPHERS_BONUS_H

# include <stdlib.h>
# include <stdio.h>
# include <unistd.h>
# include <pthread.h>
# include <semaphore.h>
# include <stdbool.h>
# include <signal.h>
# include <fcntl.h>
# include <sys/wait.h>
# include <sys/time.h>
# include <sys/stat.h>

# define MAX_PHILO "200"
# define ERR_ARG_COUNT "Wrong argument count. Usage:\n\
./philo <number_of_philosophers> <time_to_die> <time_to_eat> <time_to_sleep> \
[<number_of_times_each_philosopher_must_eat>]\n"
# define ERR_PHILNUMBER "<number_of_philosophers> must be from 1 to "
# define ERR_TIMES "<time_to_[...]> and <number_of_times_each_philosopher_must_eat> \
must be positive integers in int range or zero: >= 0 and < 2147483648.\n"
# define ERR_MALLOC "Error: malloc\n"
# define ERR_SEM_OPEN "Error: sem_open\n"
# define ERR_THREAD_CREATE "Error: pthread_create\n"
# define ERR_THREAD_JOIN "Error: pthread_join\n"
# define ERR_WAITPID "Error: waitpid\n"

typedef struct envelope
{
	int			n_philosophers;
	int			time_to_die;
	int			time_to_eat;
	int			time_to_sleep;
	int			times_to_eat;
	int			id;
	time_t		starttime;
	time_t		last_eat;
	int			times_eaten;
	pid_t		*pids;
	pthread_t	eatmonitor;
	pthread_t	stopmonitor;
	sem_t		*eatdata_lock;
	sem_t		*eaten_enough;
	sem_t		*print;
	sem_t		*stop_sim;
	sem_t		*forks;
	sem_t		**eat_locks;
}	t_envl;

//0_main.c
int		main(int argc, char **argv);
void	launch_subentities(t_envl *e);
void	land_subentities(t_envl *e);
void	*eatmonitor(void *arg);
void	*stopmonitor(void *arg);

//1_setup.c
void	setup(t_envl *e, int argc, char **argv);
void	check_input(int argc, char **argv);
void	init_envelopestruct(t_envl *e, int argc, char **argv);
time_t	calc_starttime(t_envl *e);

//2_process_philosopher_1.c
void	philosopher(t_envl *e);
void	open_semaphore_philo(t_envl *e, int id, sem_t **eatdata_lock);
void	*monitor(void *arg);
void	eat_sleep_think(t_envl *e);
int		calc_thinktime(t_envl *e);

//2_process_philosopher_2.c
void	take_forks(t_envl *e);
void	release_forks(t_envl *e);
time_t	broadcast(char *msg, t_envl *e);
void	wait_timetarget(time_t timetarget);

//5_semaphores.c
void	open_semaphores_global(t_envl *e);
void	unlink_semaphores_global(t_envl *e);
void	close_semaphores_all(t_envl *e);
void	open_semaphores_philo(t_envl *e);

//6_utils_1.c
int		is_one_to_maxphilo(char *input);
int		is_digits(char *input);
int		is_intsize(char *argv);
int		ft_strncmp(char *s1, char *s2, size_t n);
int		ft_atoi(char *nptr);

//6_utils_2.c
char	*ft_strjoin(char *s1, char *s2);
void	*ft_calloc(size_t nmemb, size_t size);
void	*ft_memset(void *s, int c, size_t n);
size_t	ft_strlcpy(char *dst, char *src, size_t size);
int		ft_strlen(char *string);

//6_utils_3.c
char	*zero_or_pos_itoa(int n);
char	*ft_strdup(char *s);
time_t	get_time_ms(void);

//8_errors.c
void	input_error_exit(char *msg);
void	input_error_philnumber_exit(char *msg);
void	exec_error_exit(char *msg, t_envl *e);

//9_shutdown.c
void	shutdown(t_envl *e);
void	kill_children(t_envl *e);
void	free2darray_char(char **array);

#endif