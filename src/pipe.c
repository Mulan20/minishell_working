// #include "../include/minishell.h"

// static void	create_pipes(t_command *cmds, int cmd_count, int pipe_fds[][2])
// {
// 	int	count;

// 	count = 0;
// 	while (count < cmd_count - 1)
// 	{
// 		pipe_fds[count][0] = -1;
// 		pipe_fds[count][1] = -1;
// 		if (cmds[count].pipe_out)
// 		{
// 			if (pipe(pipe_fds[count]) == -1)
// 			{
// 				perror("pipe");
// 				return ;
// 			}
// 		}
// 		count++;
// 	}
// }

// static void	close_pipes(int cmd_count, int pipe_fds[][2])
// {
// 	int	i;

// 	i = 0;
// 	while (i < cmd_count - 1)
// 	{
// 		if (pipe_fds[i][0] != -1)
// 			close(pipe_fds[i][0]);
// 		if (pipe_fds[i][1] != -1)
// 			close(pipe_fds[i][1]);
// 		i++;
// 	}
// }

// static void	connect_prev(int index, int prev_pipe_fd)
// {
// 	if (index > 0 && prev_pipe_fd != -1)
// 	{
// 		if (dup2(prev_pipe_fd, STDIN_FILENO) == -1)
// 		{
// 			perror("dup2 stdin");
// 			exit(1);
// 		}
// 		close(prev_pipe_fd);
// 	}
// }

// static void	connect_next(int index, int pipe_fds[][2], int cmd_count)
// {
// 	if (index < cmd_count - 1 && pipe_fds[index][1] != -1)
// 	{
// 		if (dup2(pipe_fds[index][1], STDOUT_FILENO) == -1)
// 		{
// 			perror("dup2 stdout");
// 			exit(1);
// 		}
// 		close(pipe_fds[index][1]);
// 	}
// }

// static int	exec_command_child(t_command *cmd, char **env)
// {
// 	char	*path;

// 	if (!cmd || !cmd->args || !cmd->args[0])
// 		return (0);
// 	if (is_builtin(cmd->args[0]))
// 		exit(execute_builtin(cmd, &env));
// 	else
// 	{
// 		path = find_executable(cmd->args[0], env);
// 		if (!path)
// 		{
// 			printf("minishell: %s: command not found\n", cmd->args[0]);
// 			exit(127);
// 		}
// 		execve(path, cmd->args, env);
// 		perror("execve failed");
// 		free(path);
// 		exit(126);
// 	}
// }

// static void	fork_all(t_command *cmds, int cmd_count, int pipe_fds[][2],
// 		char **env)
// {
// 	int		i;
// 	pid_t	pid;
// 	int		prev_pipe_fd;

// 	i = 0;
// 	prev_pipe_fd = -1;
// 	while (i < cmd_count)
// 	{
// 		pid = fork();
// 		if (pid == -1)
// 		{
// 			perror("Fork failed");
// 			return ;
// 		}
// 		if (pid == 0)
// 		{
// 			if (cmds[i].infile || cmds[i].heredoc)
// 			{
// 				if (cmds[i].infile)
// 					redirect_input(&cmds[i]);
// 				if (cmds[i].heredoc)
// 					handle_heredoc(&cmds[i]);
// 			}
// 			else if (i > 0 && prev_pipe_fd != -1)
// 				connect_prev(i, prev_pipe_fd);
// 			if (cmds[i].outfile)
// 				redirect_output(&cmds[i]);
// 			else if (i < cmd_count - 1 && pipe_fds[i][1] != -1)
// 				connect_next(i, pipe_fds, cmd_count);
// 			close_pipes(cmd_count, pipe_fds);
// 			exec_command_child(&cmds[i], env);
// 			exit(i);
// 		}
// 		else
// 		{
// 			// if (cmds[i].pipe_out && pipe_fds[i][0] != -1)
// 			// {
// 			// 	if (prev_pipe_fd != -1)
// 			// 		close(prev_pipe_fd);
// 			// 	prev_pipe_fd = pipe_fds[i][0];
// 			// }
// 			// if (cmds[i].pipe_out && pipe_fds[i][1] != -1)
// 			// {
// 			// 	close(pipe_fds[i][1]);
// 			// 	pipe_fds[i][1] = -1;
// 			// }
// 			if (prev_pipe_fd != -1)
// 			close(prev_pipe_fd);
	
// 			// parent: keep read end for next command (if a pipe was created)
// 			if (cmds[i].pipe_out)
// 				prev_pipe_fd = pipe_fds[i][0];
// 			else
// 				prev_pipe_fd = -1;
		
// 			// parent: close write end immediately
// 			if (cmds[i].pipe_out && pipe_fds[i][1] != -1)
// 				close(pipe_fds[i][1]);
// 			}
// 			i++;
// 	}
// }

// static void	wait_all(int *last_status)
// {
// 	pid_t	pid;
// 	int		status;

// 	while (1)
// 	{
// 		pid = wait(&status);
// 		if (pid == -1)
// 			break ;
// 		if (WIFEXITED(status))
// 		{	*last_status = WEXITSTATUS(status);}
// 			// set_last_exit_status(WEXITSTATUS(status));
// 		else if (WIFSIGNALED(status))
// 		{	*last_status = 128 + WTERMSIG(status);}
// 			// set_last_exit_status(128 + WTERMSIG(status));
// 	}
// }

// static void	execute_single_command(t_command *cmd, char ***env, int *last_status)
// {
// 	pid_t	pid;
// 	int		status;

// 	if (is_builtin(cmd->args[0]))
// 	{
// 		if (cmd->infile || cmd->outfile || cmd->heredoc)
// 		{
// 			pid = fork();
// 			if (pid == 0)
// 			{
// 				if (cmd->infile)
// 					redirect_input(cmd);
// 				if (cmd->outfile)
// 					redirect_output(cmd);
// 				if (cmd->heredoc)
// 					handle_heredoc(cmd);
// 				exit(execute_builtin(cmd, env));
// 			}
// 			else
// 			{
// 				waitpid(pid, &status, 0);
// 				// set_last_exit_status(WEXITSTATUS(status));
// 				*last_status = WEXITSTATUS(status);
// 			}
// 		}
// 		else
// 		{	*last_status = execute_builtin(cmd, env);}
// 			// set_last_exit_status(execute_builtin(cmd, env));
// 		return ;
// 	}
// 	pid = fork();
// 	if (pid == -1)
// 	{
// 		perror("fork failed");
// 		return ;
// 	}
// 	if (pid == 0)
// 	{
// 		if (cmd->infile)
// 			redirect_input(cmd);
// 		if (cmd->outfile)
// 			redirect_output(cmd);
// 		if (cmd->heredoc)
// 			handle_heredoc(cmd);
// 		exec_command_child(cmd, *env);
// 		exit(0);
// 	}
// 	else
// 	{
// 		waitpid(pid, &status, 0);
// 		// set_last_exit_status(WEXITSTATUS(status));
// 		*last_status = WEXITSTATUS(status);
// 	}
// }

// static void	execute_pipeline(t_command *cmds, int cmd_count, char **env, int *last_status)
// {
// 	int	(*pipe_fds)[2];

// 	pipe_fds = malloc(sizeof(int[2]) * (cmd_count - 1));
// 	if (!pipe_fds)
// 	{
// 		perror("malloc");
// 		return ;
// 	}
// 	create_pipes(cmds, cmd_count, pipe_fds);
// 	fork_all(cmds, cmd_count, pipe_fds, env);
// 	wait_all(last_status);
// 	free(pipe_fds);
// }

// void	exec_pipeline(t_command *cmds, int cmd_count, char ***env, int *last_status)
// {
// 	if (!cmds || cmd_count <= 0)
// 		return ;
// 	if (cmd_count == 1)
// 		execute_single_command(&cmds[0], env, last_status);
// 	else
// 		execute_pipeline(cmds, cmd_count, *env, last_status);
// }

#include "../include/minishell.h"

static void	create_pipes(t_command *cmds, int cmd_count, int pipe_fds[][2])
{
	int	i;

	i = 0;
	while (i < cmd_count - 1)
	{
		pipe_fds[i][0] = -1;
		pipe_fds[i][1] = -1;
		if (cmds[i].pipe_out)
		{
			if (pipe(pipe_fds[i]) == -1)
			{
				perror("pipe");
				return ;
			}
		}
		i++;
	}
}

static void	close_pipes(int cmd_count, int pipe_fds[][2])
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

static void	connect_prev(int prev_pipe_fd)
{
	if (prev_pipe_fd != -1)
	{
		if (dup2(prev_pipe_fd, STDIN_FILENO) == -1)
		{
			perror("dup2 stdin");
			exit(1);
		}
		close(prev_pipe_fd);
	}
}

static void	connect_next(int pipe_write_fd)
{
	if (pipe_write_fd != -1)
	{
		if (dup2(pipe_write_fd, STDOUT_FILENO) == -1)
		{
			perror("dup2 stdout");
			exit(1);
		}
		close(pipe_write_fd);
	}
}

static int	exec_command_child(t_command *cmd, char **env)
{
	char	*path;

	if (!cmd || !cmd->args || !cmd->args[0])
		return (0);
	if (is_builtin(cmd->args[0]))
		exit(execute_builtin(cmd, &env, 0));
	path = find_executable(cmd->args[0], env);
	if (!path)
	{
		printf("minishell: %s: command not found\n", cmd->args[0]);
		exit(127);
	}
	execve(path, cmd->args, env);
	perror("execve failed");
	free(path);
	exit(126);
}

// static void	fork_all(t_command *cmds, int cmd_count, int pipe_fds[][2],
// 		char **env)
// {
// 	int		i;
// 	pid_t	pid;
// 	int		prev_pipe_fd;

// 	i = 0;
// 	prev_pipe_fd = -1;
// 	while (i < cmd_count)
// 	{
// 		pid = fork();
// 		if (pid == -1)
// 		{
// 			perror("Fork failed");
// 			return ;
// 		}
// 		if (pid == 0)
// 		{
// 			if (cmds[i].infile)
// 				redirect_input(&cmds[i]);
// 			else if (cmds[i].heredoc)
// 				handle_heredoc(&cmds[i]);
// 			else if (i > 0)
// 				connect_prev(prev_pipe_fd);
// 			if (cmds[i].outfile)
// 				redirect_output(&cmds[i]);
// 			else if (i < cmd_count - 1)
// 				connect_next(pipe_fds[i][1]);
// 			close_pipes(cmd_count, pipe_fds);
// 			exec_command_child(&cmds[i], env);
// 			exit(1);
// 		}
// 		else
// 		{
// 			if (prev_pipe_fd != -1)
// 				close(prev_pipe_fd);
// 			if (cmds[i].pipe_out)
// 				prev_pipe_fd = pipe_fds[i][0];
// 			else
// 				prev_pipe_fd = -1;
// 			if (cmds[i].pipe_out && pipe_fds[i][1] != -1)
// 				close(pipe_fds[i][1]);
// 		}
// 		i++;
// 	}
// 	if (prev_pipe_fd != -1)
// 		close(prev_pipe_fd);
// }

static pid_t	fork_all(t_command *cmds, int cmd_count, int pipe_fds[][2],
		char **env)
{
	int		i;
	pid_t	pid;
	pid_t	last_pid;
	int		prev_pipe_fd;

	i = 0;
	last_pid = -1;
	prev_pipe_fd = -1;
	while (i < cmd_count)
	{
		pid = fork();
		if (pid == -1)
		{
			perror("Fork failed");
			return (-1);
		}
		if (pid == 0)
		{
			if (cmds[i].infile)
				redirect_input(&cmds[i]);
			else if (cmds[i].heredoc)
				handle_heredoc(&cmds[i]);
			else if (i > 0)
				connect_prev(prev_pipe_fd);
			if (cmds[i].outfile)
				redirect_output(&cmds[i]);
			else if (i < cmd_count - 1)
				connect_next(pipe_fds[i][1]);
			close_pipes(cmd_count, pipe_fds);
			exec_command_child(&cmds[i], env);
			exit(1);
		}
		else
		{
			if (i == cmd_count - 1)
				last_pid = pid;
			if (prev_pipe_fd != -1)
				close(prev_pipe_fd);
			if (cmds[i].pipe_out)
				prev_pipe_fd = pipe_fds[i][0];
			else
				prev_pipe_fd = -1;
			if (cmds[i].pipe_out && pipe_fds[i][1] != -1)
				close(pipe_fds[i][1]);
		}
		i++;
	}
	if (prev_pipe_fd != -1)
		close(prev_pipe_fd);
	return (last_pid);
}

// static void	wait_all(int *last_status)
// {
// 	pid_t	pid;
// 	int		status;

// 	while (1)
// 	{
// 		pid = wait(&status);
// 		if (pid == -1)
// 			break ;
// 		if (WIFEXITED(status))
// 			*last_status = WEXITSTATUS(status);
// 		else if (WIFSIGNALED(status))
// 			*last_status = 128 + WTERMSIG(status);
// 	}
// }

static void	wait_all(int *last_status, pid_t last_pid)
{
	pid_t	pid;
	int		status;

	while (1)
	{
		pid = wait(&status);
		if (pid == -1)
			break ;
		if (pid == last_pid)
		{
			if (WIFEXITED(status))
				*last_status = WEXITSTATUS(status);
			else if (WIFSIGNALED(status))
				*last_status = 128 + WTERMSIG(status);
		}
	}
}

static void	set_status_from_wait(int status, int *last_status)
{
	if (WIFEXITED(status))
		*last_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		*last_status = 128 + WTERMSIG(status);
}

static void	execute_single_command(t_command *cmd, char ***env,
		int *last_status)
{
	pid_t	pid;
	int		status;

	if (is_builtin(cmd->args[0]))
	{
		if (cmd->infile || cmd->outfile || cmd->heredoc)
		{
			pid = fork();
			if (pid == -1)
			{
				perror("fork failed");
				*last_status = 1;
				return ;
			}
			if (pid == 0)
			{
				if (cmd->infile)
					redirect_input(cmd);
				if (cmd->outfile)
					redirect_output(cmd);
				if (cmd->heredoc)
					handle_heredoc(cmd);
				exit(execute_builtin(cmd, env, *last_status));
			}
			waitpid(pid, &status, 0);
			set_status_from_wait(status, last_status);
		}
		else
		{
			*last_status = execute_builtin(cmd, env, *last_status);
		}
		return ;
	}
	pid = fork();
	if (pid == -1)
	{
		perror("fork failed");
		*last_status = 1;
		return ;
	}
	if (pid == 0)
	{
		if (cmd->infile)
			redirect_input(cmd);
		if (cmd->outfile)
			redirect_output(cmd);
		if (cmd->heredoc)
			handle_heredoc(cmd);
		exec_command_child(cmd, *env);
		exit(1);
	}
	waitpid(pid, &status, 0);
	set_status_from_wait(status, last_status);
}

// static void	execute_pipeline(t_command *cmds, int cmd_count, char **env,
// 		int *last_status)
// {
// 	int	(*pipe_fds)[2];

// 	pipe_fds = malloc(sizeof(int[2]) * (cmd_count - 1));
// 	if (!pipe_fds)
// 	{
// 		perror("malloc");
// 		*last_status = 1;
// 		return ;
// 	}
// 	create_pipes(cmds, cmd_count, pipe_fds);
// 	fork_all(cmds, cmd_count, pipe_fds, env);
// 	wait_all(last_status);
// 	free(pipe_fds);
// }

static void	execute_pipeline(t_command *cmds, int cmd_count, char **env,
		int *last_status)
{
	int		(*pipe_fds)[2];
	pid_t	last_pid;

	pipe_fds = malloc(sizeof(int[2]) * (cmd_count - 1));
	if (!pipe_fds)
	{
		perror("malloc");
		*last_status = 1;
		return ;
	}
	create_pipes(cmds, cmd_count, pipe_fds);
	last_pid = fork_all(cmds, cmd_count, pipe_fds, env);
	wait_all(last_status, last_pid);
	free(pipe_fds);
}

void	exec_pipeline(t_command *cmds, int cmd_count, char ***env,
		int *last_status)
{
	if (!cmds || cmd_count <= 0 || !last_status)
		return ;
	if (cmd_count == 1)
		execute_single_command(&cmds[0], env, last_status);
	else
		execute_pipeline(cmds, cmd_count, *env, last_status);
}