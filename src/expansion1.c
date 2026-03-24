/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion1.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkham <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/24 17:36:44 by nkham             #+#    #+#             */
/*   Updated: 2026/03/24 18:54:28 by nkham            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static int	init_expand(t_expand *e, char *str, char **envp, int last_status)
{
	e->out = malloc(4096);
	if (!e->out)
		return (-1);
	e->str = str;
	e->envp = envp;
	e->i = 0;
	e->j = 0;
	e->sq = 0;
	e->dq = 0;
	e->last_status = last_status;
	return (0);
}

static int	handle_single_quote_content(t_expand *e)
{
	e->i++;
	while (e->str[e->i] && e->str[e->i] != '\'')
	{
		e->out[e->j++] = e->str[e->i++];
	}
	if (e->str[e->i] != '\'')
		return (-1);
	e->i++;
	return (0);
}

/* Handle content inside double quotes - expand $ */
static int	handle_double_quote_content(t_expand *e)
{
	e->i++;
	while (e->str[e->i] && e->str[e->i] != '"')
	{
		if (e->str[e->i] == '$' && e->str[e->i + 1])
		{
			if (expand_dollar(e) == -1)
				return (-1);
		}
		else
		{
			e->out[e->j++] = e->str[e->i++];
		}
	}
	if (e->str[e->i] != '"')
		return (-1);
	e->i++;
	return (0);
}

static int	step_expand(t_expand *e)
{
	if (e->str[e->i] == '\'' && !e->dq)
		return (handle_single_quote_content(e));
	if (e->str[e->i] == '"' && !e->sq)
		return (handle_double_quote_content(e));
	if (e->str[e->i] == '$' && !e->sq && e->str[e->i + 1])
		return (expand_dollar(e));
	e->out[e->j++] = e->str[e->i++];
	return (0);
}

char	*expand_string(char *str, char **envp, int last_status)
{
	t_expand	e;

	if (!str)
		return (ft_strdup(""));
	if (!envp)
		return (ft_strdup(str));
	if (init_expand(&e, str, envp, last_status) == -1)
		return (NULL);
	while (e.str[e.i] && e.j < 4090)
	{
		if (step_expand(&e) == -1)
		{
			free(e.out);
			return (NULL);
		}
	}
	e.out[e.j] = '\0';
	return (e.out);
}
