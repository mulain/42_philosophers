/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   6_utils_2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wmardin <wmardin@student.42wolfsburg.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/03 10:13:51 by wmardin           #+#    #+#             */
/*   Updated: 2022/11/06 12:09:02 by wmardin          ###   ########.fr       */
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
	char	*out;

	if (n == 0)
		return (ft_strdup("0"));
	i = 0;
	while (n > 0)
	{
		n /= 10;
		i++;
	}
	out = ft_calloc(i + 1, sizeof(char));
	i--;
	while (n > 0)
	{
		out[i] = n % 10 + 48;
		i--;
		n /= 10;
	}
	return (out);
}

char	*ft_strdup(const char *s)
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
