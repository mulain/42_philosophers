/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   1_setup_3.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wmardin <wmardin@student.42wolfsburg.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/29 12:49:04 by wmardin           #+#    #+#             */
/*   Updated: 2022/10/29 12:52:52 by wmardin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	calc_thinktime(t_envl *e)
{
	e->common.time_to_think = 0;
}
