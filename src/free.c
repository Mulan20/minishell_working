/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkham <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/23 12:23:09 by nkham             #+#    #+#             */
/*   Updated: 2026/03/23 13:28:15 by nkham            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	free_tokens(t_token *tokens)
{
	t_token	*tk;

	while (tokens)
	{
		tk = tokens;
		tokens = tokens->next;
		if (tk->value)
			free(tk->value);
		free(tk);
	}
}

static void	free_cmd_args(t_command *cmd)
{
	int	j;

	if (!cmd->args)
		return ;
	j = 0;
	while (cmd->args[j])
	{
		free(cmd->args[j]);
		j++;
	}
	free(cmd->args);
	cmd->args = NULL;
}

static void	free_cmd_redirs(t_command *cmd)
{
	free(cmd->infile);
	free(cmd->outfile);
	free(cmd->heredoc);
	cmd->infile = NULL;
	cmd->outfile = NULL;
	cmd->heredoc = NULL;
}

void	free_commands(t_command *cmds, int count)
{
	int	i;

	if (!cmds)
		return ;
	i = 0;
	while (i < count)
	{
		free_cmd_args(&cmds[i]);
		free_cmd_redirs(&cmds[i]);
		i++;
	}
	free(cmds);
}

void	free_split(char **split)
{
	int	i;

	i = 0;
	if (!split)
		return ;
	while (split[i])
	{
		free(split[i]);
		i++;
	}
	free(split);
}
