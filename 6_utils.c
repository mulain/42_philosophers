/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   6_utils.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wmardin <wmardin@student.42wolfsburg.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/21 18:35:45 by wmardin           #+#    #+#             */
/*   Updated: 2022/10/28 17:41:59 by wmardin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

time_t	get_timestamp(time_t starttime)
{
	struct timeval		time;
	time_t				currenttime_ms;

	gettimeofday(&time, NULL);
	currenttime_ms = time.tv_sec * 1000 + time.tv_usec / 1000;
	return (currenttime_ms - starttime);
}

void	broadcast(time_t timestamp, char *msg)
{
	printf("%li %i %s\n", timestamp, 420, msg);
}
