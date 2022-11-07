/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   0_main.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wmardin <wmardin@student.42wolfsburg.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/03 09:03:05 by wmardin           #+#    #+#             */
/*   Updated: 2022/11/07 11:38:29 by wmardin          ###   ########.fr       */
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

	setup(&e, argc, argv);
	launch_philoforks(&e);
	launch_monitorthreads(&e);
	wait_children(&e);
	join_threads(&e);
	shutdown(&e);
}

void	launch_philoforks(t_envl *e)
{
	int		i;

	i = 0;
	while (i < e->n_philosophers)
	{
		e->id = i + 1;
		e->pids[i] = fork();
		if (e->pids[i] == -1)
			exec_error_exit("Error: fork\n", e);
		if (e->pids[i] == 0)
		{
			e->philofunction(e);
			printf("shouldnt be here\n"); //dont forget
			exit(EXIT_SUCCESS);
		}
		i++;
	}
}

void	launch_monitorthreads(t_envl *e)
{
	if (pthread_create(&e->eatmonitor, NULL, eatmonitor, e))
		exec_error_exit(ERR_THREAD_CREATE, e);
	if (pthread_create(&e->stopmonitor, NULL, stopmonitor, e))
		exec_error_exit(ERR_THREAD_CREATE, e);

}

void	wait_children(t_envl *e)
{
	int		i;

	i = 0;
	while (i < e->n_philosophers)
	{
		if (waitpid(e->pids[i], NULL, 0) == -1)
			exec_error_exit(ERR_WAITPID, e);
		i++;
	}
}

void	join_threads(t_envl *e)
{
	if (pthread_join(e->eatmonitor, NULL))
		exec_error_exit(ERR_THREAD_JOIN, e);
	if (pthread_join(e->stopmonitor, NULL))
		exec_error_exit(ERR_THREAD_JOIN, e);
}

/*
Keeps waiting (reducing) the allsated semaphore until all philosophers
have posted to it once.
Then waits on the printlock and doesn't post it again so no further broadcasts
are possible (just to be thorough).
*/
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
	sem_wait(e->printlock);
	sem_post(e->stoplock);
	return (NULL);
	// what if this isent reachad, will the thread be joind?
}

/*
Waits for the binary semaphore stoplock to be available (i.e. get posted once).
That means a stop condition has been reached and the simulation should end.
Kills the child processes and returns.
*/
void	*stopmonitor(void *arg)
{
	t_envl		*e;
	int			i;

	e = (t_envl *)arg;
	i = 0;
	sem_wait(e->stoplock);
	printf("stop detected in main\n");
	while (i < e->n_philosophers)
	{
		kill(e->pids[i], SIGKILL);
		i++;
	}
	return (NULL);
}

time_t	get_time_ms(void)
{
	struct timeval		time;

	gettimeofday(&time, NULL);
	return (time.tv_sec * 1000 + time.tv_usec / 1000);
}
