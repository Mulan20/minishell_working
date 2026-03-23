/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkham <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/23 12:23:09 by nkham             #+#    #+#             */
/*   Updated: 2026/03/23 12:50:44 by nkham            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	redirect_input(t_command *cmd)
{
	int	fd;

	if (!cmd->infile)
		return ;
	fd = open(cmd->infile, O_RDONLY);
	if (fd == -1)
	{
		printf("minishell: %s: %s\n", cmd->infile, strerror(errno));
		exit(1);
	}
	if (dup2(fd, STDIN_FILENO) == -1)
	{
		perror("dup2 stdin");
		close(fd);
		exit(1);
	}
	close(fd);
}

void	redirect_output(t_command *cmd)
{
	int	fd;
	int	flags;

	if (!cmd->outfile)
		return ;
	fflush(stdout);
	if (cmd->append)
		flags = O_WRONLY | O_CREAT | O_APPEND;
	else
		flags = O_WRONLY | O_CREAT | O_TRUNC;
	fd = open(cmd->outfile, flags, 0644);
	if (fd == -1)
	{
		printf("minishell: %s: %s\n", cmd->outfile, strerror(errno));
		exit(1);
	}
	if (dup2(fd, STDOUT_FILENO) == -1)
	{
		perror("dup2 stdout");
		close(fd);
		exit(1);
	}
	close(fd);
}

void	handle_heredoc(t_command *cmd)
{
	int		pipe_fd[2];
	char	*line;

	if (!cmd->heredoc)
		return ;
	if (pipe(pipe_fd) == -1)
	{
		perror("pipe");
		exit(1);
	}
	while (1)
	{
		line = readline("> ");
		if (!line)
			break ;
		if (ft_strcmp(line, cmd->heredoc) == 0)
		{
			free(line);
			break ;
		}
		write(pipe_fd[1], line, ft_strlen(line));
		write(pipe_fd[1], "\n", 1);
		free(line);
	}
	close(pipe_fd[1]);
	if (dup2(pipe_fd[0], STDIN_FILENO) == -1)
	{
		perror("dup2 stdin");
		close(pipe_fd[0]);
		exit(1);
	}
	close(pipe_fd[0]);
}
