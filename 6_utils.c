/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   6_utils.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wmardin <wmardin@student.42wolfsburg.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/21 18:35:45 by wmardin           #+#    #+#             */
/*   Updated: 2022/10/22 21:34:32 by wmardin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

/*
Checks whether the input string represents a positive integer.
*/
int	ispositiveint(char *input)
{
	int		i;

	if (input[0] == '0')
		return (0);
	i = 0;
	while (input[i])
	{
		if (input[i] < 47 || input[i] > 58)
			return (0);
		i++;
	}
	return (1);
}

/*
Checks whether the input string represents a positive integer or 0.
*/
int	ispositiveintorzero(char *input)
{
	int		i;

	i = 0;
	while (input[i])
	{
		if (input[i] < 47 || input[i] > 58)
			return (0);
		i++;
	}
	return (1);
}

int	ft_atoi(const char *nptr)
{
	int	i;
	int	sign;
	int	num;

	i = 0;
	num = 0;
	sign = 1;
	while (nptr[i] == 32 || (nptr[i] > 8 && nptr[i] < 14))
		++i;
	if (nptr[i] == 45 || nptr[i] == 43)
	{
		if (nptr[i] == 45)
			sign = -1;
		++i;
	}
	while (nptr[i] > 47 && nptr[i] < 58)
	{
		num = num * 10 + (nptr[i] - 48);
		++i;
	}
	return (num * sign);
}
