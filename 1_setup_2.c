/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   1_setup_2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wmardin <wmardin@student.42wolfsburg.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/25 16:22:47 by wmardin           #+#    #+#             */
/*   Updated: 2022/10/29 12:49:01 by wmardin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

/*
Checks whether the input string consists of only digits; digits must
represent a number > 0. Leading 0s are allowed, e.g. 001.
*/
int	is_positivenum(char *input)
{
	int		i;
	int		nonzero;

	i = 0;
	nonzero = 0;
	if (input[0] == '0')
	{
		while (input[i])
		{
			if (input[i] != '0')
				nonzero = 1;
			i++;
		}
		if (!nonzero)
			return (0);
	}
	i = 0;
	while (input[i])
	{
		if (input[i] < '0' || input[i] > '9')
			return (0);
		i++;
	}
	return (1);
}

/*
Checks whether the input string consists of only digits.
*/
int	is_digits(char *input)
{
	int		i;

	i = 0;
	while (input[i])
	{
		if (input[i] < '0' || input[i] > '9')
			return (0);
		i++;
	}
	return (1);
}

int	is_intsize(char *string)
{
	int		len;

	len = 0;
	while (string[len])
		len++;
	if (len > 11)
		return (0);
	if (string[0] == '-')
	{
		if (len == 11)
		{
			if (ft_strncmp("-2147483648", string, 69) < 0)
				return (0);
		}
		return (1);
	}
	if (len == 11)
		return (0);
	if (len == 10)
	{
		if (ft_strncmp("2147483647", string, 420) < 0)
			return (0);
	}
	return (1);
}

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t			i;
	unsigned char	*u_s1;
	unsigned char	*u_s2;

	i = 0;
	u_s1 = (unsigned char *) s1;
	u_s2 = (unsigned char *) s2;
	while (i < n)
	{
		if (u_s1[i] == 0 && u_s2[i] == 0)
			return (0);
		if (u_s1[i] != u_s2[i])
			return (u_s1[i] - u_s2[i]);
		i++;
	}
	return (0);
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
