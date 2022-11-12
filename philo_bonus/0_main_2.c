/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   0_main_2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wmardin <wmardin@student.42wolfsburg.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/07 13:08:00 by wmardin           #+#    #+#             */
/*   Updated: 2022/11/11 20:01:37 by wmardin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers_bonus.h"

/*
Keeps waiting on (reducing) the eaten_enough semaphore until all philosophers
have posted to it once.
Sets the stop_sim to signal the end of the simulation.
*/
void	*eatmonitor(void *arg)
{
	t_envl		*e;
	int			i;

	e = (t_envl *)arg;
	wait_timetarget(e->starttime);
	if (e->times_to_eat == 0)
		sem_post(e->stop_sim);
	i = 0;
	while (i < e->n_philosophers)
	{
		sem_wait(e->eaten_enough);
		i++;
	}
	sem_post(e->stop_sim);
	return (NULL);
}

/*
Waits for the binary semaphore stop_sim to be available (i.e. get posted once).
That means a stop condition has been reached and the simulation should end.
Kills the child processes and posts to the eaten_enough semaphore, because:
If the stop signal was sent from a philo process (death of hunger), the other
monitor thread in the main process, the eatenenough_checker, will still be stuck
at its semaphore.
Sending a signal to kill it is not allowed (pthread_cancel). So the stopmonitor
posts n_philosophers to the eatmonitors semaphore which will allow it to
continue.
*/
void	*stopmonitor(void *arg)
{
	t_envl		*e;
	int			i;

	e = (t_envl *)arg;
	i = 0;
	sem_wait(e->stop_sim);
	kill_children(e);
	while (i < e->n_philosophers)
	{
		sem_post(e->eaten_enough);
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
