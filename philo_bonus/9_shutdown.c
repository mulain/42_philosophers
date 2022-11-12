/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   9_shutdown.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wmardin <wmardin@student.42wolfsburg.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/07 08:33:45 by wmardin           #+#    #+#             */
/*   Updated: 2022/11/11 16:04:04 by wmardin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers_bonus.h"

void	shutdown(t_envl *e)
{
	kill_children(e);
	close_semaphores(e);
	unlink_semaphores(e);
	if (e->pids)
		free (e->pids);
}

void	kill_children(t_envl *e)
{
	int		i;

	i = 0;
	while (i < e->n_philosophers)
	{
		kill(e->pids[i], SIGKILL);
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
