/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkham <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/23 14:00:00 by nkham             #+#    #+#             */
/*   Updated: 2026/03/23 12:47:57 by nkham            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

char	**copy_env(char **envp)
{
	int		count;
	char	**new_env;
	int		i;

	count = 0;
	while (envp[count])
		count++;
	new_env = malloc(sizeof(char *) * (count + 1));
	if (!new_env)
		return (NULL);
	i = 0;
	while (i < count)
	{
		new_env[i] = ft_strdup(envp[i]);
		if (!new_env[i])
		{
			free_env(new_env);
			return (NULL);
		}
		i++;
	}
	new_env[i] = NULL;
	return (new_env);
}

void	free_env(char **env)
{
	int	i;

	if (!env)
		return ;
	i = 0;
	while (env[i])
	{
		free(env[i]);
		i++;
	}
	free(env);
}

static int	get_env_index(char **env, char *var)
{
	int		i;
	int		var_len;

	if (!env || !var)
		return (-1);
	var_len = ft_strlen(var);
	i = 0;
	while (env[i])
	{
		if (ft_strncmp(env[i], var, var_len) == 0 && env[i][var_len] == '=')
			return (i);
		i++;
	}
	return (-1);
}

char	*get_env_value(char **env, char *var)
{
	int		index;
	int		len;

	index = get_env_index(env, var);
	if (index == -1)
		return (NULL);
	len = ft_strlen(var);
	return (env[index] + len + 1);
}

int	env_var_exists(char **env, char *var)
{
	return (get_env_index(env, var) != -1);
}

void	add_or_update_env(char ***env, char *var, char *value)
{
	int		index;
	int		old_count;
	char	**new_env;
	char	*new_entry;
	int		i;

	if (!env || !*env || !var)
		return ;
	new_entry = malloc(ft_strlen(var) + ft_strlen(value) + 2);
	if (!new_entry)
		return ;
	ft_strcpy(new_entry, var);
	ft_strcat(new_entry, "=");
	ft_strcat(new_entry, value);
	index = get_env_index(*env, var);
	if (index != -1)
	{
		free((*env)[index]);
		(*env)[index] = new_entry;
	}
	else
	{
		old_count = 0;
		while ((*env)[old_count])
			old_count++;
		new_env = malloc(sizeof(char *) * (old_count + 2));
		if (!new_env)
		{
			free(new_entry);
			return ;
		}
		i = 0;
		while (i < old_count)
		{
			new_env[i] = (*env)[i];
			i++;
		}
		new_env[old_count] = new_entry;
		new_env[old_count + 1] = NULL;
		free(*env);
		*env = new_env;
	}
}

void	remove_env_var(char ***env, char *var)
{
	int		index;
	int		count;
	char	**new_env;
	int		i;
	int		j;

	if (!env || !*env || !var)
		return ;
	index = get_env_index(*env, var);
	if (index == -1)
		return ;
	count = 0;
	while ((*env)[count])
		count++;
	new_env = malloc(sizeof(char *) * count);
	if (!new_env)
		return ;
	i = 0;
	j = 0;
	while (i < count)
	{
		if (i != index)
		{
			new_env[j] = (*env)[i];
			j++;
		}
		else
			free((*env)[i]);
		i++;
	}
	new_env[count - 1] = NULL;
	free(*env);
	*env = new_env;
}
