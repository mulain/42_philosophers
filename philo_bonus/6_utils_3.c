/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   6_utils_3.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wmardin <wmardin@student.42wolfsburg.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/03 10:13:51 by wmardin           #+#    #+#             */
/*   Updated: 2022/12/27 16:14:49 by wmardin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers_bonus.h"

/*
Slimmed down version of ft_itoa that only deals with ints >= 0.
Care: It doesn't deal with unsigned ints, only up to MAX_INT.
*/
char	*zero_or_pos_itoa(int n)
{
	int		i;
	int		n_copy;
	char	*out;

	if (n == 0)
		return (ft_strdup("0"));
	i = 0;
	n_copy = n;
	while (n_copy > 0)
	{
		n_copy /= 10;
		i++;
	}
	out = malloc((i + 1) * sizeof(char));
	out[i] = 0;
	i--;
	while (n > 0)
	{
		out[i] = n % 10 + 48;
		i--;
		n /= 10;
	}
	return (out);
}

char	*ft_strdup(char *s)
{
	char	*c;
	int		i;

	i = ft_strlen(s) + 1;
	c = malloc(i);
	if (!c)
		return (NULL);
	ft_strlcpy(c, s, i);
	return (c);
}

time_t	get_time_ms(void)
{
	struct timeval		time;

	gettimeofday(&time, NULL);
	return (time.tv_sec * 1000 + time.tv_usec / 1000);
}
