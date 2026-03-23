# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: nkham <marvin@42.fr>                       +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/03/23 12:00:00 by nkham             #+#    #+#              #
#    Updated: 2026/03/23 13:31:25 by nkham            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell
CC = cc
CFLAGS = -Wall -Wextra -Werror
RM = rm -f

SRCS = src/main.c \
       src/input.c \
       src/lexer.c \
       src/parser.c \
       src/env.c \
       src/builtins.c \
       src/execution.c \
       src/path.c \
       src/fork.c \
       src/pipe.c \
       src/redirect.c \
       src/signals.c \
       src/expansion.c \
       src/status.c \
       src/utils.c \
       src/ft_split.c \
       src/free.c

OBJS = $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -lreadline -o $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) -Iinclude -c $< -o $@

clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re
