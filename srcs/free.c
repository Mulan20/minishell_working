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

void	free_commands(t_command *cmds, int count)
{
	int	i;
	int	j;

	if (!cmds)
		return ;
	i = 0;
	while (i < count)
	{
		if (cmds[i].args)
		{
			j = 0;
			while (cmds[i].args[j])
			{
				free(cmds[i].args[j]);
				j++;
			}
			free(cmds[i].args);
		}
		if (cmds[i].infile)
			free(cmds[i].infile);
		if (cmds[i].outfile)
			free(cmds[i].outfile);
		if (cmds[i].heredoc)
			free(cmds[i].heredoc);
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
