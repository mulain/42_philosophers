/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   0_main.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wmardin <wmardin@student.42wolfsburg.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/03 09:03:05 by wmardin           #+#    #+#             */
/*   Updated: 2022/11/06 21:51:01 by wmardin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers_bonus.h"

/*
for pid in $(ps -ef | grep "philo" | awk '{print $2}'); do kill -9 $pid; done
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
			e.philofunction(&e);
			printf("shouldnt be here\n");
			exit(EXIT_SUCCESS);
		}
		
		i++;
	}
	launch_monitorthreads(&e);

		//else not needed i gueeees? since the children will end in philo
		//termination thread: check stop semaphore (gets set by children if starvation)
		//termination thread2: check sated varibale and set stop if sated.
		//if stop, take printlock to stop msgs (better: child should broadcast its death
		//and never post back the printlock),
		// kill children, shutdown.

	shutdown(&e);
}

void	launch_monitorthreads(t_envl *e)
{
	if (pthread_create(&e->eatmonitor, NULL, eatmonitor, &e))
		exec_error_exit(ERR_THREAD_CREATE, e);
	if (pthread_create(&e->stopmonitor, NULL, stopmonitor, &e))
		exec_error_exit(ERR_THREAD_CREATE, e);

}

void	*eatmonitor(void *arg)
{
	t_envl		*e;
	int			i;

	e = (t_envl *)arg;
	wait_timetarget(e->starttime);
	printf("hello eatmonitor\n");
	i = 0; //i not necessary, can just decrement nphilo...
	while (i < e->n_philosophers)
	{
		sem_wait(e->allsated);
		printf("semwait allsated iterated, i:%i\n", i);
		i++;
	}
	printf("allsated\n");
	//sem_post(e->stoplock);
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

void	shutdown(t_envl *e)
{

	if (e->last_eat_locks)
		free (e->last_eat_locks);
	if (e->sem_init)
	{
		unlink_semaphores(e);
	}
	free2darray_char(e->le_locks_names);
}

void	unlink_semaphores(t_envl *e)
{
	int		i;
	//close semaphores also here or elsewhere or unlink is enuff?

	sem_unlink("/allsated");
	sem_unlink("/print");
	sem_unlink("/stop");
	sem_unlink("/forks");
	i = 0;
	while (i < e->n_philosophers)
	{
		sem_unlink(e->le_locks_names[i]);
		i++;
	}
}

void	free2darray_char(char **array)
{
	int		i;

	if (!array)
		return ;
	i = 0;
	while (array[i])
	{
		free(array[i]);
		i++;
	}
	free(array);
}

time_t	get_time_ms(void)
{
	struct timeval		time;

	gettimeofday(&time, NULL);
	return (time.tv_sec * 1000 + time.tv_usec / 1000);
}
