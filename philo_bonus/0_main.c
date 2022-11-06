/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   0_main.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wmardin <wmardin@student.42wolfsburg.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/03 09:03:05 by wmardin           #+#    #+#             */
/*   Updated: 2022/11/06 17:26:56 by wmardin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers_bonus.h"

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
			exit(EXIT_SUCCESS);
		}
		i++;
	}

	shutdown(&e);
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
