/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   9_shutdown.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wmardin <wmardin@student.42wolfsburg.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/07 08:33:45 by wmardin           #+#    #+#             */
/*   Updated: 2022/11/07 09:02:48 by wmardin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers_bonus.h"

void	shutdown(t_envl *e)
{

	if (e->pids)
		free (e->pids);
	if (e->last_eat_locks)
		free (e->last_eat_locks);
	free2darray_char(e->le_locks_names);
	//if (e->sem_init)
	close_semaphores(e);
	unlink_semaphores(e);
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