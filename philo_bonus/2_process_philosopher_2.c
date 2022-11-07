/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   2_process_philosopher_2.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wmardin <wmardin@student.42wolfsburg.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/04 08:07:36 by wmardin           #+#    #+#             */
/*   Updated: 2022/11/07 10:50:42 by wmardin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers_bonus.h"


void	take_forks(t_envl *e)
{
	sem_wait(e->forks);
	broadcast("has taken a fork", e);
	sem_wait(e->forks);
	broadcast("has taken a fork", e);
}

void	release_forks(t_envl *e)
{
	sem_post(e->forks);
	sem_post(e->forks);
}

/*
printf itself doesn't require a mutex guard in order for its printing to not be
interrupted by another print. The mutex is to prevent a call with a higher
timestamp from being printed before a call with a lower timestamp.
*/
time_t	broadcast(char *msg, t_envl *e)
{
	time_t		now;

	sem_wait(e->printlock);
	now = get_time_ms();
	printf("%li %i %s\n", now - e->starttime, e->id, msg);
	sem_post(e->printlock);
	return (now);
}
