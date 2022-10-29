/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   7_shutdown.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wmardin <wmardin@student.42wolfsburg.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/24 11:40:17 by wmardin           #+#    #+#             */
/*   Updated: 2022/10/29 23:47:30 by wmardin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	shutdown(t_envl *e)
{
	int		i;

	free(e->forks);
	free(e->threads);
	free(e->philostructs);
	pthread_mutex_destroy(&e->common.printlock);
	i = 0;
	while (i < e->n_philosophers)
	{
		pthread_mutex_destroy(&e->forks[i]);
		i++;
	}
}
