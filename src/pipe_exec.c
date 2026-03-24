/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_exec.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkham <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/24 17:29:08 by nkham             #+#    #+#             */
/*   Updated: 2026/03/24 17:29:10 by nkham            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static void	child_setup_io(t_command *cmd, int i, t_fork_ctx *ctx)
{
	if (cmd->pipe_in)
		connect_prev(ctx->pipe_fds[i - 1][0]);
	if (cmd->pipe_out && i < ctx->n - 1)
		connect_next(ctx->pipe_fds[i][1]);
	if (cmd->infile)
		redirect_input(cmd);
	else if (cmd->heredoc)
		handle_heredoc(cmd);
	if (cmd->outfile)
		redirect_output(cmd);
	close_pipes(ctx->n, ctx->pipe_fds);
}

static int	parent_update_prev(t_command *cmds, int i, int pipe_fds[][2],
	int prev)
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

static int	fork_step(t_fork_ctx *ctx, int i, pid_t *last_pid, int *prev)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
		return (perror("Fork failed"), -1);
	if (pid == 0)
	{
		child_setup_io(&ctx->cmds[i], i, ctx);
		exec_command_child(&ctx->cmds[i], ctx->env, ctx->last_status);
		exit(1);
	}
	if (i == ctx->n - 1)
		*last_pid = pid;
	*prev = parent_update_prev(ctx->cmds, i, ctx->pipe_fds, *prev);
	return (0);
}

static pid_t	fork_loop(t_fork_ctx *ctx, pid_t *last_pid)
{
	int	i;
	int	prev;

	i = 0;
	prev = -1;
	*last_pid = -1;
	while (i < ctx->n)
	{
		if (fork_step(ctx, i, last_pid, &prev) == -1)
			return (-1);
		i++;
	}
	if (prev != -1)
		close(prev);
	return (0);
}

// pid_t	fork_all(t_command *cmds, int n, int pipe_fds[][2], char **env)
// {
// 	struct s_fork_ctx	ctx;
// 	pid_t				last_pid;

// 	ctx.cmds = cmds;
// 	ctx.n = n;
// 	ctx.pipe_fds = pipe_fds;
// 	ctx.env = env;
// 	ctx.last_status = last_status;
// 	if (fork_loop(&ctx, &last_pid) == -1)
// 		return (-1);
// 	return (last_pid);
// }

pid_t	fork_all(t_fork_ctx *ctx)
{
	pid_t	last_pid;

	if (fork_loop(ctx, &last_pid) == -1)
		return (-1);
	return (last_pid);
}
