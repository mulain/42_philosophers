/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   1a_setup.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wmardin <wmardin@student.42wolfsburg.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/25 16:22:47 by wmardin           #+#    #+#             */
/*   Updated: 2022/10/25 16:32:02 by wmardin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	init_mutexes(t_envl *e)
{
	int		i;

	pthread_mutex_init(&e->info->printlock, NULL);
	e->forks = malloc(e->n_philosophers * sizeof(pthread_mutex_t));
	i = 0;
	while (i < e->n_philosophers)
	{
		pthread_mutex_init(e->forks + i, NULL);
		i++;
	}
}
