/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_redir.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkham <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/24 17:31:15 by nkham             #+#    #+#             */
/*   Updated: 2026/03/24 17:31:17 by nkham            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

t_token	*process_redirection(t_command *cmd, t_token *current)
{
	if (!current || !current->next)
		return (NULL);
	if (current->next->type != WORD)
		return (current->next);
	if (current->type == REDIR_IN)
		return (free(cmd->infile),
			cmd->infile = ft_strdup(current->next->value),
			current->next);
	if (current->type == HEREDOC)
		return (free(cmd->heredoc),
			cmd->heredoc = ft_strdup(current->next->value),
			current->next);
	free(cmd->outfile);
	cmd->outfile = ft_strdup(current->next->value);
	cmd->append = (current->type == APPEND);
	return (current->next);
}

char	*remove_quotes(char *str)
{
	int		len;
	char	*res;
	int		i;

	if (!str)
		return (NULL);
	len = ft_strlen(str);
	if (len < 2)
		return (ft_strdup(str));
	if ((str[0] == '\'' || str[0] == '"') && str[len - 1] == str[0])
	{
		res = malloc(len - 1);
		if (!res)
			return (NULL);
		i = 0;
		while (i < len - 2)
		{
			res[i] = str[i + 1];
			i++;
		}
		res[i] = '\0';
		return (res);
	}
	return (ft_strdup(str));
}

// void	add_word_arg(t_command *cmd, char *value, int *index)
// {
// 	char	*clean;

// 	if (!value)
// 		return ;
// 	clean = remove_quotes(value);
// 	if (!clean)
// 		return ;
// 	cmd->args[*index] = clean;
// 	(*index)++;
// }
void	add_word_arg(t_command *cmd, char *value, int *index)
{
	if (!value)
		return ;
	cmd->args[*index] = ft_strdup(value);
	if (!cmd->args[*index])
		return ;
	(*index)++;
}
