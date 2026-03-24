/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkham <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/24 17:27:50 by nkham             #+#    #+#             */
/*   Updated: 2026/03/24 17:27:53 by nkham            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static void	apply_redirs(t_command *cmd)
{
	if (cmd->infile)
		redirect_input(cmd);
	if (cmd->outfile)
		redirect_output(cmd);
	if (cmd->heredoc)
		handle_heredoc(cmd);
}

static void	run_builtin_single(t_command *cmd, char ***env, int *st)
{
	pid_t	pid;
	int		status;

	if (!cmd->infile && !cmd->outfile && !cmd->heredoc)
		return (*st = execute_builtin(cmd, env, *st), (void)0);
	pid = fork();
	if (pid == -1)
		return (perror("fork failed"), *st = 1, (void)0);
	if (pid == 0)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		apply_redirs(cmd);
		exit(execute_builtin(cmd, env, *st));
	}
	waitpid(pid, &status, 0);
	set_status_from_wait(status, st);
}

static void	run_external_single(t_command *cmd, char ***env, int *st)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid == -1)
		return (perror("fork failed"), *st = 1, (void)0);
	if (pid == 0)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		apply_redirs(cmd);
		exec_command_child(cmd, *env, *st);
		exit(1);
	}
	waitpid(pid, &status, 0);
	set_status_from_wait(status, st);
}

static void	execute_pipeline(t_command *cmds, int n, char **env, int *st)
{
	t_fork_ctx	ctx;
	int			(*pipe_fds)[2];
	pid_t		last_pid;

	pipe_fds = malloc(sizeof(int [2]) * (n - 1));
	if (!pipe_fds)
		return (perror("malloc"), *st = 1, (void)0);
	create_pipes(cmds, n, pipe_fds);
	ctx.cmds = cmds;
	ctx.n = n;
	ctx.pipe_fds = pipe_fds;
	ctx.env = env;
	ctx.last_status = *st;
	last_pid = fork_all(&ctx);
	wait_all(st, last_pid);
	free(pipe_fds);
}

void	exec_pipeline(t_command *cmds, int cmd_count, char ***env, int *st)
{
	if (!cmds || cmd_count <= 0 || !env || !st)
		return ;
	if (cmd_count == 1 && is_builtin(cmds[0].args[0]))
		run_builtin_single(&cmds[0], env, st);
	else if (cmd_count == 1)
		run_external_single(&cmds[0], env, st);
	else
		execute_pipeline(cmds, cmd_count, *env, st);
}
