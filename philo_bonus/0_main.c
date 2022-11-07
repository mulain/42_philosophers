/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   0_main.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wmardin <wmardin@student.42wolfsburg.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/03 09:03:05 by wmardin           #+#    #+#             */
/*   Updated: 2022/11/07 10:52:11 by wmardin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers_bonus.h"

/*
for pid in $(ps -ef | grep "philo" | awk '{print $2}'); do kill -9 $pid; done

else not needed i gueeees? since the children will end in philo
termination thread: check stop semaphore (gets set by children if starvation)
termination thread2: check sated varibale and set stop if sated.
if stop, take printlock to stop msgs (better: child should broadcast its death
and never post back the printlock),
kill children, shutdown.
*/
int	main(int argc, char **argv)
{
	t_envl		e;
	int			i;

	setup(&e, argc, argv);
	i = 0;
	while (i < e.n_philosophers)
	{
		e.id = i + 1;
		e.pids[i] = fork();
		if (e.pids[i] == -1)
			exec_error_exit("Error: fork\n", &e);
		if (e.pids[i] == 0)
		{
			printf("launch philo with id:%i\n", e.id);
			e.philofunction(&e);
			printf("shouldnt be here\n");
			exit(EXIT_SUCCESS);
		}
		printf("pid_i in parent:%i\n", e.pids[i]);
		i++;
	}
	printf("parent starttime:%li\n", e.starttime);

	launch_monitorthreads(&e);
	i = 0;
	while (i < e.n_philosophers)
	{
		waitpid(e.pids[i], NULL, 0);
		i++;
	}
	if (pthread_join(e.eatmonitor, NULL))
		exec_error_exit(ERR_THREAD_JOIN, &e);
	if (pthread_join(e.stopmonitor, NULL))
		exec_error_exit(ERR_THREAD_JOIN, &e);
	shutdown(&e);
}

void	launch_monitorthreads(t_envl *e)
{
	if (pthread_create(&e->eatmonitor, NULL, eatmonitor, e))
		exec_error_exit(ERR_THREAD_CREATE, e);
	if (pthread_create(&e->stopmonitor, NULL, stopmonitor, e))
		exec_error_exit(ERR_THREAD_CREATE, e);

}

void	*eatmonitor(void *arg)
{
	t_envl		*e;
	int			i;

	e = (t_envl *)arg;
	wait_timetarget(e->starttime);
	i = 0;
	while (i < e->n_philosophers)
	{
		sem_wait(e->allsated);
		i++;
	}
	printf("allsated asdasdasd\n");
	sem_post(e->stoplock);
	return (NULL);
}

void	*stopmonitor(void *arg)
{
	t_envl		*e;

	e = (t_envl *)arg;
	wait_timetarget(e->starttime);
	sem_wait(e->stoplock);
	printf("stop detected in main\n");
	//kill
	return (NULL);
}

time_t	get_time_ms(void)
{
	struct timeval		time;

	gettimeofday(&time, NULL);
	return (time.tv_sec * 1000 + time.tv_usec / 1000);
}
