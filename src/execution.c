/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkham <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/23 12:23:09 by nkham             #+#    #+#             */
/*   Updated: 2026/03/23 12:48:27 by nkham            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	exec_command(t_command *cmd, char **env)
{
	if (!cmd || !cmd->args || !cmd->args[0])
		return (0);
	if (is_builtin(cmd->args[0]))
		return (execute_builtin(cmd, &env, 0));
	else
		return (execute_external(cmd, env));
}
