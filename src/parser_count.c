/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_count.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkham <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/24 17:30:39 by nkham             #+#    #+#             */
/*   Updated: 2026/03/24 17:30:40 by nkham            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static int	is_redir(t_token_type type)
{
	return (type == REDIR_IN || type == REDIR_OUT
		|| type == APPEND || type == HEREDOC);
}

int	count_word(t_token *tokens)
{
	int		count;
	t_token	*prev;

	count = 0;
	prev = NULL;
	while (tokens && tokens->type != PIPE)
	{
		if (tokens->type == WORD && (!prev || !is_redir(prev->type)))
			count++;
		prev = tokens;
		tokens = tokens->next;
	}
	return (count);
}

int	count_commands(t_token *tokens)
{
	int	count;

	if (!tokens)
		return (0);
	count = 1;
	while (tokens)
	{
		if (tokens->type == PIPE)
			count++;
		tokens = tokens->next;
	}
	return (count);
}
