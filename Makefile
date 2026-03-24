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
       src/parser_count.c \
       src/parser_init.c\
       src/parser_pipe.c\
       src/parser_redir.c\
       src/parser_refill.c\
       src/env_copy_free.c \
       src/env_get.c\
       src/env_mutate.c\
       src/env_update.c\
       src/builtins/builtin_echo.c \
       src/builtins/builtin_cd.c \
       src/builtins/builtin_pwd.c \
       src/builtins/builtin_env.c \
       src/builtins/builtin_exit.c \
       src/builtins/builtin_export.c \
       src/builtins/builtin_unset.c \
       src/builtins/builtin_utils.c \
       src/execution.c \
       src/path.c \
       src/path_utils.c \
       src/fork.c \
       src/pipe.c \
       src/pipe_helpers.c \
       src/pipe_exec.c \
       src/pipe_exec2.c \
       src/pipe_wait.c \
       src/redirect.c \
       src/signals.c \
       src/expansion.c \
       src/utils.c \
       src/utils2.c \
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
