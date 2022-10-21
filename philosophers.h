/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wmardin <wmardin@student.42wolfsburg.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/21 17:59:59 by wmardin           #+#    #+#             */
/*   Updated: 2022/10/21 19:13:26 by wmardin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H

# define PHILOSOPHERS_H
# include <stdlib.h>
# include <stdio.h>
# include <unistd.h>
# include <pthread.h>

typedef struct envelope
{
	char	**argv;
	int		max_eat;
}	t_envl;

//0_main.c
int		main(int argc, char **argv);
void	setup(t_envl *e, int argc, char **argv);

//6_utils.c
int		ft_strlen(char *s);
int		ispositiveint(char *input);

//8_errors.c
void	error_exit(char *msg);

#endif