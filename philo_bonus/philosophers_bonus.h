/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers_bonus.h                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wmardin <wmardin@student.42wolfsburg.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/21 17:59:59 by wmardin           #+#    #+#             */
/*   Updated: 2022/11/06 16:14:18 by wmardin          ###   ########.fr       */
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
# include <fcntl.h>
#include <sys/stat.h>

# define MAX_PHILO "200"
# define ERR_ARG_COUNT "Wrong argument count. Usage:\n\
./philo <number_of_philosophers> <time_to_die> <time_to_eat> <time_to_sleep> \
[<number_of_times_each_philosopher_must_eat>]\n"
# define ERR_PHILNUMBER "<number_of_philosophers> must be from 1 to "
# define ERR_TIMES "<time_to_[...]> and <number_of_times_each_philosopher_must_eat> \
must be positive integers in int range or zero: >= 0 and < 2147483648.\n"
# define ERR_MALLOC "Error: malloc\n"
# define ERR_SEM_OPEN "Error: semopen\n"
# define ERR_THREAD_CREATE "Error: pthread_create\n"
# define ERR_THREAD_JOIN "Error: pthread_join\n"

typedef struct envelope
{
	int			n_philosophers;
	int			time_to_die;
	int			time_to_eat;
	int			time_to_sleep;
	int			times_to_eat;
	int			times_eaten;
	time_t		last_eat;
	int			id;
	time_t		starttime;
	pid_t		*pids;
	bool		stop; //prolly not needed, cuase semt stop
	bool		sem_init;
	void		*(*philofunction)(); // adapt this to not need casting
	char		**le_locks_names;
	sem_t		**last_eat_locks;
	sem_t		*allsated;
	sem_t		*printlock;
	sem_t		*stoplock;
	sem_t		*forks;
}	t_envl;

//0_main.c
int		main(int argc, char **argv);
bool	launch_threads(t_envl *e);
bool	collect_threads(t_envl *e);
void	shutdown(t_envl *e);
void	free2darray_char(char **array);
void	unlink_semaphores(t_envl *e);
time_t	get_time_ms(void);

//1_setup_1.c
void	setup(t_envl *e, int argc, char **argv);
void	check_input(int argc, char **argv);
void	parse_input(t_envl *e, int argc, char **argv);

//1_setup_2.c
void	init_envelopestruct(t_envl *e);
void	open_semaphores(t_envl *e);
bool	init_philostructs(t_envl *e);
int		calc_starttime(t_envl *e);

//1_setup_3.c
int		is_one_to_maxphilo(char *input);
int		is_digits(char *input);
int		is_intsize(char *argv);
int		ft_strncmp(char *s1, char *s2, size_t n);
int		ft_atoi(char *nptr);

//2_process_philosopher_1.c
void	*philosopher(void);
void	*philosopher_solo(void);

//6_utils_1.c
char	*ft_strjoin(char *s1, char *s2);
void	*ft_calloc(size_t nmemb, size_t size);
void	*ft_memset(void *s, int c, size_t n);
size_t	ft_strlcpy(char *dst, char *src, size_t size);
int		ft_strlen(char *string);

//6_utils_2.c
char	*zero_or_pos_itoa(int n);
char	*ft_strdup(char *s);

//8_errors.c
void	input_error_exit(char *msg);
void	input_error_philnumber_exit(char *msg);
void	exec_error_exit(char *msg, t_envl *e);

#endif