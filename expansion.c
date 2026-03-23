/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkham <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/23 12:23:09 by nkham             #+#    #+#             */
/*   Updated: 2026/03/23 12:23:10 by nkham            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static int	is_valid_char(char c)
{
	return ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')
		|| (c >= '0' && c <= '9') || c == '_' || c == '?');
}

static char	*get_env_var(char *name, char **envp)
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

static char	*expand_var(char *name, char **envp)
{
	char	*val;

	if (ft_strcmp(name, "?") == 0)
	{
		char *result = malloc(20);
		if (result)
			sprintf(result, "%d", get_last_exit_status());
		return (result);
	}
	val = get_env_var(name, envp);
	if (val)
		return (ft_strdup(val));
	return (ft_strdup(""));
}

char	*expand_string(char *str, char **envp)
{
	char	*result;
	char	*var_name;
	char	*expanded_var;
	int		i;
	int		j;
	int		k;
	int		in_single_quote;
	int		in_double_quote;

	if (!str)
		return (ft_strdup(""));
	if (!envp)
		return (ft_strdup(str));
	result = malloc(4096);
	if (!result)
		return (NULL);
	i = 0;
	j = 0;
	in_single_quote = 0;
	in_double_quote = 0;
	while (str[i] && j < 4090)
	{
		if (str[i] == '\'' && !in_double_quote)
		{
			in_single_quote = !in_single_quote;
			i++;
			continue;
		}
		else if (str[i] == '"' && !in_single_quote)
		{
			in_double_quote = !in_double_quote;
			i++;
			continue;
		}
		else if (str[i] == '$' && !in_single_quote && str[i + 1])
		{
			i++;
			if (str[i] == '?')
			{
				expanded_var = malloc(20);
				if (expanded_var)
					sprintf(expanded_var, "%d", get_last_exit_status());
				i++;
			}
			else if (is_valid_char(str[i]))
			{
				var_name = malloc(256);
				if (!var_name)
					return (NULL);
				k = 0;
				while (str[i] && is_valid_char(str[i]) && k < 255)
				{
					var_name[k++] = str[i++];
				}
				var_name[k] = '\0';
				expanded_var = expand_var(var_name, envp);
				free(var_name);
			}
			else
			{
				result[j++] = '$';
				continue;
			}
			if (expanded_var)
			{
				int val_len = ft_strlen(expanded_var);
				if (j + val_len < 4090)
				{
					ft_strcpy(result + j, expanded_var);
					j += val_len;
				}
				free(expanded_var);
			}
		}
		else
		{
			result[j++] = str[i++];
		}
	}
	result[j] = '\0';
	return (result);
}