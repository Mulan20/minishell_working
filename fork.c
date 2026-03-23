/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fork.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkham <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/23 12:23:09 by nkham             #+#    #+#             */
/*   Updated: 2026/03/23 12:49:21 by nkham            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	fork_execute(char *path, char **args, char **env)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid == -1)
	{
		perror("minishell: fork");
		return (1);
	}
	if (pid == 0)
	{
		execve(path, args, env);
		perror("minishell: execve");
		exit(126);
	}
	else
	{
		waitpid(pid, &status, 0);
		if (WIFEXITED(status))
			return (WEXITSTATUS(status));
	}
	return (1);
}

int	execute_external(t_command *cmd, char **env)
{
	char	*path;
	int		ret;

	if (!cmd || !cmd->args || !cmd->args[0])
		return (1);
	path = find_executable(cmd->args[0], env);
	if (!path)
	{
		printf("minishell: %s: command not found\n", cmd->args[0]);
		return (127);
	}
	ret = fork_execute(path, cmd->args, env);
	free(path);
	return (ret);
}
