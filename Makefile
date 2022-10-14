# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: wmardin <wmardin@student.42wolfsburg.de>   +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/10/01 20:50:46 by wmardin           #+#    #+#              #
#    Updated: 2022/10/13 22:00:16 by wmardin          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC = gcc
CFLAGS = -pthreads
ERRORFLAGS = -Wall -Wextra -Werror
DEFINE =

NAME =	philosophers
SRC =	00_main+setup.c\
		02_utils.c\
		07_shutdown.c\
		08_errors.c\
		09_helperfunctions.c

COLOR = \033[94m
COLOR_END = \033[0m

%.o: %.c

LIBFT = libft/libft.a

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