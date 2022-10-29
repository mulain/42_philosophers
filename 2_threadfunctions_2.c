/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   2_threadfunctions_2.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wmardin <wmardin@student.42wolfsburg.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/29 14:35:10 by wmardin           #+#    #+#             */
/*   Updated: 2022/10/29 14:48:23 by wmardin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

/*
printf itself doesn't require a mutex guard. The mutex is to prevent a call with
an older timestamp from being printed before a call with a younger timestamp.
Only prints if the simulation hasn't been stopped. Allowed functions don't permit
killing of threads, so they have to be prevented from printing before they can
themselves "find out" the sim has stopped and terminate of their own accord.
*/
time_t	broadcast(char *msg, t_philo *p)
{
	time_t		now;

	pthread_mutex_lock(&p->common->printlock);
	now = get_time_ms();
	if (!p->common->stop)
		printf("%li %i %s\n", now - p->common->starttime, p->id, msg);
	pthread_mutex_unlock(&p->common->printlock);
	return (now);
}
