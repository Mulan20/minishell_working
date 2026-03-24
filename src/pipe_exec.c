#include "../include/minishell.h"

int	exec_command_child(t_command *cmd, char **env)
{
	char	*path;

	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	if (!cmd || !cmd->args || !cmd->args[0])
		exit(0);
	if (is_builtin(cmd->args[0]))
		exit(execute_builtin(cmd, &env, 0));
	path = find_executable(cmd->args[0], env);
	if (!path)
		return (printf("minishell: %s: command not found\n", cmd->args[0]),
			exit(127), 0);
	execve(path, cmd->args, env);
	perror("execve failed");
	free(path);
	exit(126);
}

// static void	child_setup_pipes(t_command *cmd, int i, int n, int pipe_fds[][2])
// {
// 	if (cmd->pipe_in)
// 		connect_prev(pipe_fds[i - 1][0]);
// 	if (cmd->pipe_out && i < n - 1)
// 		connect_next(pipe_fds[i][1]);
// 	close_pipes(n, pipe_fds);
// }

// static void	child_setup_io(t_command *cmd, int i, int n, int pipe_fds[][2])
// {
// 	if (cmd->infile)
// 		redirect_input(cmd);
// 	else if (cmd->heredoc)
// 		handle_heredoc(cmd);
// 	if (cmd->outfile)
// 		redirect_output(cmd);
// 	child_setup_pipes(cmd, i, n, pipe_fds);
// }

static void	child_setup_io(t_command *cmd, int i, int n, int pipe_fds[][2])
{
	if (cmd->pipe_in)
		connect_prev(pipe_fds[i - 1][0]);
	if (cmd->pipe_out && i < n - 1)
		connect_next(pipe_fds[i][1]);
	if (cmd->infile)
		redirect_input(cmd);
	else if (cmd->heredoc)
		handle_heredoc(cmd);
	if (cmd->outfile)
		redirect_output(cmd);
	close_pipes(n, pipe_fds);
}

static int	parent_update_prev(t_command *cmds, int i, int pipe_fds[][2], int prev)
{
	if (prev != -1)
		close(prev);
	if (cmds[i].pipe_out)
	{
		if (pipe_fds[i][1] != -1)
			close(pipe_fds[i][1]);
		return (pipe_fds[i][0]);
	}
	return (-1);
}

pid_t	fork_all(t_command *cmds, int n, int pipe_fds[][2], char **env)
{
	int		i;
	pid_t	pid;
	pid_t	last_pid;
	int		prev;

	i = 0;
	last_pid = -1;
	prev = -1;
	while (i < n)
	{
		pid = fork();
		if (pid == -1)
			return (perror("Fork failed"), -1);
		if (pid == 0)
		{
			child_setup_io(&cmds[i], i, n, pipe_fds);
			exec_command_child(&cmds[i], env);
			exit(1);
		}
		if (i == n - 1)
			last_pid = pid;
		prev = parent_update_prev(cmds, i, pipe_fds, prev);
		i++;
	}
	if (prev != -1)
		close(prev);
	return (last_pid);
}