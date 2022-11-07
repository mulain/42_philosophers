/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   1_setup_2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wmardin <wmardin@student.42wolfsburg.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/02 11:11:57 by wmardin           #+#    #+#             */
/*   Updated: 2022/11/07 12:28:27 by wmardin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers_bonus.h"

/*
We are not allowed to use sem_init, only sem_open.
That means it is not possible to specify a process specific semaphore. So it
is necessary to make a distinct semaphore for each process to act as a lock for
the writing / reading of the eating-variables.
Another note: I don't believe it is functionally relevant to protect this data
from race conditions. However the subject stated that no races are allowed,
so here we are.
*/
void	init_envelopestruct(t_envl *e)
{
	int		i;
	char	*i_alpha;

	e->starttime = calc_starttime(e);
	e->last_eat = e->starttime;
	e->times_eaten = 0;
	e->pids = malloc(e->n_philosophers * sizeof(pid_t));
	e->le_locks_names = NULL; //consistent about init nulling
	e->le_locks_names = malloc((e->n_philosophers + 1) * sizeof(char *));
	e->last_eat_locks = malloc(e->n_philosophers * sizeof(sem_t *));
	if (!e->pids || !e->le_locks_names || !e->last_eat_locks)
		exec_error_exit(ERR_MALLOC, e);
	i = 0;
	while (i < e->n_philosophers)
	{
		i_alpha = zero_or_pos_itoa(i);
		e->le_locks_names[i] = ft_strjoin("/last_eat", i_alpha);
		free (i_alpha);
		if (!e->le_locks_names[i])
			exec_error_exit(ERR_MALLOC, e);
		i++;
	}
	e->le_locks_names[i] = NULL;
}

time_t	calc_starttime(t_envl *e)
{
	int		offset;

	offset = e->n_philosophers * 20;
	if (offset > 1000)
		offset = 1000;
	if (offset < 100)
		offset = 100;
	return (get_time_ms() + offset);
}
