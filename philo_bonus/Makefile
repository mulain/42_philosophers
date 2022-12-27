# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: wmardin <wmardin@student.42wolfsburg.de>   +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/11/02 17:30:04 by wmardin           #+#    #+#              #
#    Updated: 2022/12/21 18:01:55 by wmardin          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC = gcc
CFLAGS = -pthread -O3
ERRORFLAGS = -Wall -Wextra -Werror
DEFINE =

NAME =	philo_bonus
SRC =	0_main_1.c\
		0_main_2.c\
		1_setup.c\
		2_process_philosopher_1.c\
		2_process_philosopher_2.c\
		5_semaphores.c\
		6_utils_1.c\
		6_utils_2.c\
		6_utils_3.c\
		8_errors.c\
		9_shutdown.c

COLOR = \033[94m
COLOR_END = \033[0m

%.o: %.c

$(NAME):
	@$(CC) $(CFLAGS) $(ERRORFLAGS) $(SRC) $(DEFINE) -o $(NAME)
	@echo "$(COLOR)$(NAME) created."

all: $(NAME)

clean:
	@$(RM) $(OBJ)
	@echo "$(COLOR)clean performed."

fclean: clean
	@$(RM) $(NAME) a.out
	@echo "$(COLOR)fclean performed."

re: fclean all

noflag:
	@$(CC) $(CFLAGS) $(SRC) $(DEFINE) -o $(NAME)
	@echo "$(COLOR)$(NAME) created w/o error flags."

test:
	@$(CC) $(CFLAGS) $(SRC) -o test

git:
	git add .
	git commit -m "auto add & push"
	git push
	@echo "$(COLOR)git auto add & push performed."