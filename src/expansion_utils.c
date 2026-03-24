/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkham <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/24 17:37:18 by nkham             #+#    #+#             */
/*   Updated: 2026/03/24 17:37:19 by nkham            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	is_valid_char(char c)
{
	return ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')
		|| (c >= '0' && c <= '9') || c == '_' || c == '?');
}

char	*get_env_var(char *name, char **envp)
{
	int	i;
	int	var_len;

	if (!name || !envp)
		return (NULL);
	var_len = ft_strlen(name);
	i = 0;
	while (envp[i])
	{
		if (ft_strncmp(envp[i], name, var_len) == 0 && envp[i][var_len] == '=')
			return (envp[i] + var_len + 1);
		i++;
	}
	return (NULL);
}

char	*expand_status(int last_status)
{
	return (ft_itoa(last_status));
}

char	*expand_var(char *name, char **envp, int last_status)
{
	char	*val;

	if (ft_strcmp(name, "?") == 0)
		return (expand_status(last_status));
	val = get_env_var(name, envp);
	if (val)
		return (ft_strdup(val));
	return (ft_strdup(""));
}
