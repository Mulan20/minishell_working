/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_update.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkham <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/24 17:38:02 by nkham             #+#    #+#             */
/*   Updated: 2026/03/24 17:38:04 by nkham            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static char	*make_env_entry(char *var, char *value)
{
	char	*new_entry;

	if (!var || !value)
		return (NULL);
	new_entry = malloc(ft_strlen(var) + ft_strlen(value) + 2);
	if (!new_entry)
		return (NULL);
	ft_strcpy(new_entry, var);
	ft_strcat(new_entry, "=");
	ft_strcat(new_entry, value);
	return (new_entry);
}

static char	**env_grow_and_add(char **env, char *new_entry)
{
	int		old_count;
	char	**new_env;
	int		i;

	old_count = 0;
	while (env && env[old_count])
		old_count++;
	new_env = malloc(sizeof(char *) * (old_count + 2));
	if (!new_env)
		return (NULL);
	i = 0;
	while (i < old_count)
	{
		new_env[i] = env[i];
		i++;
	}
	new_env[old_count] = new_entry;
	new_env[old_count + 1] = NULL;
	free(env);
	return (new_env);
}

void	add_or_update_env(char ***env, char *var, char *value)
{
	int		index;
	char	*new_entry;
	char	**grown;

	if (!env || !*env || !var || !value)
		return ;
	new_entry = make_env_entry(var, value);
	if (!new_entry)
		return ;
	index = get_env_index(*env, var);
	if (index != -1)
	{
		free((*env)[index]);
		(*env)[index] = new_entry;
		return ;
	}
	grown = env_grow_and_add(*env, new_entry);
	if (!grown)
	{
		free(new_entry);
		return ;
	}
	*env = grown;
}
