/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_scan.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkham <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/24 17:37:28 by nkham             #+#    #+#             */
/*   Updated: 2026/03/24 18:55:31 by nkham            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	toggle_quotes(char c, int *sq, int *dq)
{
	if (c == '\'' && !(*dq))
		return ((*sq = !(*sq)), 1);
	if (c == '"' && !(*sq))
		return ((*dq = !(*dq)), 1);
	return (0);
}

static char	*read_var_name(t_expand *e)
{
	char	*name;
	int		k;

	name = malloc(256);
	if (!name)
		return (NULL);
	k = 0;
	while (e->str[e->i] && is_valid_char(e->str[e->i]) && k < 255)
		name[k++] = e->str[e->i++];
	name[k] = '\0';
	return (name);
}

static int	append_str(t_expand *e, char *src, int limit)
{
	int	len;

	if (!src)
		return (0);
	len = ft_strlen(src);
	if (e->j + len >= limit)
		return (-1);
	ft_strcpy(e->out + e->j, src);
	e->j += len;
	return (0);
}

static int	expand_question(t_expand *e)
{
	char	*val;

	val = expand_status(e->last_status);
	if (!val)
		return (-1);
	if (append_str(e, val, 4090) == -1)
		return (free(val), -1);
	free(val);
	e->i++;
	return (0);
}

int	expand_dollar(t_expand *e)
{
	char	*name;
	char	*val;

	e->i++;
	if (e->str[e->i] == '?')
		return (expand_question(e));
	if (!is_valid_char(e->str[e->i]))
		return (e->out[e->j++] = '$', 0);
	name = read_var_name(e);
	if (!name)
		return (-1);
	val = expand_var(name, e->envp, e->last_status);
	free(name);
	if (!val)
		return (-1);
	if (append_str(e, val, 4090) == -1)
		return (free(val), -1);
	free(val);
	return (0);
}
