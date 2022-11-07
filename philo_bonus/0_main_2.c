/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   0_main_2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wmardin <wmardin@student.42wolfsburg.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/07 13:08:00 by wmardin           #+#    #+#             */
/*   Updated: 2022/11/07 13:08:16 by wmardin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers_bonus.h"

/*
Keeps waiting on (reducing) the allsated semaphore until all philosophers
have posted to it once.
Then waits on the printlock and doesn't post it again so no further broadcasts
are possible. Finally sets the stoplock to signal the end of the simulation.
*/
void	*eatmonitor(void *arg)
{
	t_envl		*e;
	int			i;

	e = (t_envl *)arg;
	wait_timetarget(e->starttime);
	if (e->times_to_eat == 0)
	{
		sem_wait(e->printlock);
		sem_post(e->stoplock);
	}
	i = 0;
	while (i < e->n_philosophers)
	{
		sem_wait(e->allsated);
		i++;
	}
	sem_wait(e->printlock);
	sem_post(e->stoplock);
	return (NULL);
}

/*
Waits for the binary semaphore stoplock to be available (i.e. get posted once).
That means a stop condition has been reached and the simulation should end.
Kills the child processes and returns.
If the stop signal was sent from a philo process (death of hunger), the other
monitor thread in the main process, the eatenenough_checker, will still be stuck
at its semaphore.
Sending a signal to kill it is not allowed (pthread_cancel). So the stopmonitor
posts n_philosophers to the eatmonitors semaphore which will allow it to
continue. It also needs the printlock to terminate, so that is also posted.
*/
void	*stopmonitor(void *arg)
{
	t_envl		*e;
	int			i;

	e = (t_envl *)arg;
	i = 0;
	sem_wait(e->stoplock);
	while (i < e->n_philosophers)
	{
		kill(e->pids[i], SIGKILL);
		sem_post(e->allsated);
		i++;
	}
	sem_post(e->printlock);
	return (NULL);
}

time_t	get_time_ms(void)
{
	struct timeval		time;

	gettimeofday(&time, NULL);
	return (time.tv_sec * 1000 + time.tv_usec / 1000);
}
