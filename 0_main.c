/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   0_main.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wmardin <wmardin@student.42wolfsburg.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/21 17:59:27 by wmardin           #+#    #+#             */
/*   Updated: 2022/10/27 17:25:08 by wmardin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

/*
• When a philosopher has finished eating, they put their forks back on the table and
start sleeping. Once awake, they start thinking again. The simulation stops when
a philosopher dies of starvation.

• Your(s) program(s) should take the following arguments:
number_of_philosophers time_to_die time_to_eat time_to_sleep
[number_of_times_each_philosopher_must_eat]
◦ number_of_philosophers: The number of philosophers and also the number
of forks.
◦ time_to_die (in milliseconds): If a philosopher didn’t start eating time_to_die
milliseconds since the beginning of their last meal or the beginning of the simulation, they die.
◦ time_to_eat (in milliseconds): The time it takes for a philosopher to eat.
During that time, they will need to hold two forks.
◦ time_to_sleep (in milliseconds): The time a philosopher will spend sleeping.
◦ number_of_times_each_philosopher_must_eat (optional argument): If all
philosophers have eaten at least number_of_times_each_philosopher_must_eat
times, the simulation stops. If not specified, the simulation stops when a
philosopher dies.
• Each philosopher has a number ranging from 1 to number_of_philosophers.
• Philosopher number 1 sits next to philosopher number number_of_philosophers.
Any other philosopher number N sits between philosopher number N - 1 and philosopher number N + 1.

About the logs of your program:
• Any state change of a philosopher must be formatted as follows:
◦ timestamp_in_ms X has taken a fork
◦ timestamp_in_ms X is eating
◦ timestamp_in_ms X is sleeping
◦ timestamp_in_ms X is thinking
◦ timestamp_in_ms X died
Replace timestamp_in_ms with the current timestamp in milliseconds
and X with the philosopher number.
• A displayed state message should not be mixed up with another message.
• A message announcing a philosopher died should be displayed no more than 10 ms
after the actual death of the philosopher.
• Again, philosophers should avoid dying!

mandatory:
The specific rules for the mandatory part are:
• Each philosopher should be a thread.
• There is one fork between each pair of philosophers. Therefore, if there are several
philosophers, each philosopher has a fork on their left side and a fork on their right
side. If there is only one philosopher, there should be only one fork on the table.
• To prevent philosophers from duplicating forks, you should protect the forks state
with a mutex for each of them.
*/

/*
eat -> sleep -> think -> eat...
2 number of philosophers = number of forks (range: 1 to numb of phil)
3 time to die (ms)
4 time to eat (2 forks required, left and right of philo)
5 time to sleep (ms)
6 (number of times each philosopher must eat)
	optional, sim stops if all have eaten
	at least this number of times. if not specified,
	sim runs til one philo dies

old msg:
"Wrong number of arguments.\nFormat:\n./philosophers\
 n_philosophers time_to_die time_to_eat time_to_sleep (max_eat)"

 Allowed functions
memset,
printf, write,
malloc, free,
usleep, gettimeofday,
pthread_create, pthread_detach, pthread_join,
pthread_mutex_init, pthread_mutex_destroy, pthread_mutex_lock, pthread_mutex_unlock


implement isofintsize check

*/

int	main(int argc, char **argv)
{
	t_envl		e;
	int			i;

	setup(&e, argc, argv);
	i = 0;
	while (i < e.n_philosophers)
	{
		if (pthread_create(e.threads + i, NULL, philosopher,
				&e.philostructs[i]))
			error_exit("Error: pthread_create");
		i++;
	}
	i = 0;
	while (i < e.n_philosophers)
	{
		if (pthread_join(e.threads[i], NULL))
			error_exit("Error: pthread_join");
		i++;
	}
	printf("cray\n");
}
