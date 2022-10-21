/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   6_utils.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wmardin <wmardin@student.42wolfsburg.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/21 18:35:45 by wmardin           #+#    #+#             */
/*   Updated: 2022/10/21 19:27:16 by wmardin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

/*
Checks if the input string is a positive integer.
*/
int	ispositiveint(char *input)
{
	int		i;

	if (input[0] == '0')
		return (0);
	i = 1;
	while (input[i])
	{
		if (input[i] < 47 || input[i] > 58)
			return (0);
		i++;
	}
	return (1);
}


int	ft_strlen(char *s)
{
	int		i;

	i = 0;
	while (s[i])
		i++;
	return (i);
}
