# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: wmardin <wmardin@student.42wolfsburg.de>   +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/10/01 20:50:46 by wmardin           #+#    #+#              #
#    Updated: 2022/11/02 11:24:57 by wmardin          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC = gcc
CFLAGS = -pthread
ERRORFLAGS = -Wall -Wextra -Werror
DEFINE =

NAME =	philosophers
SRC =	0_main.c\
		1_setup_1.c\
		1_setup_2.c\
		1_setup_3.c\
		2_thread_philosopher_1.c\
		2_thread_philosopher_2.c\
		3_thread_monitor.c\
		6_utils.c\
		7_shutdown.c\
		8_errors.c

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