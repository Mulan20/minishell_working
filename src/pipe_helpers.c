#include "../include/minishell.h"

void	create_pipes(t_command *cmds, int cmd_count, int pipe_fds[][2])
{
	int	i;

	i = 0;
	while (i < cmd_count - 1)
	{
		pipe_fds[i][0] = -1;
		pipe_fds[i][1] = -1;
		if (cmds[i].pipe_out && pipe(pipe_fds[i]) == -1)
		{
			perror("pipe");
			return ;
		}
		i++;
	}
}

void	close_pipes(int cmd_count, int pipe_fds[][2])
{
	int	i;

	i = 0;
	while (i < cmd_count - 1)
	{
		if (pipe_fds[i][0] != -1)
			close(pipe_fds[i][0]);
		if (pipe_fds[i][1] != -1)
			close(pipe_fds[i][1]);
		i++;
	}
}

void	connect_prev(int prev_pipe_fd)
{
	if (prev_pipe_fd == -1)
		return ;
	if (dup2(prev_pipe_fd, STDIN_FILENO) == -1)
	{
		perror("dup2 stdin");
		exit(1);
	}
	close(prev_pipe_fd);
}

void	connect_next(int pipe_write_fd)
{
	if (pipe_write_fd == -1)
		return ;
	if (dup2(pipe_write_fd, STDOUT_FILENO) == -1)
	{
		perror("dup2 stdout");
		exit(1);
	}
	close(pipe_write_fd);
}

void	set_status_from_wait(int status, int *last_status)
{
	if (WIFEXITED(status))
		*last_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		*last_status = 128 + WTERMSIG(status);
}
