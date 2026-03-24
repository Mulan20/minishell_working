/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_mutate.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkham <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/24 17:38:13 by nkham             #+#    #+#             */
/*   Updated: 2026/03/24 17:38:14 by nkham            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static char	**env_remove_at(char **env, int index)
{
	int		count;
	char	**new_env;
	int		i;
	int		j;

	count = 0;
	while (env[count])
		count++;
	new_env = malloc(sizeof(char *) * count);
	if (!new_env)
		return (NULL);
	i = 0;
	j = 0;
	while (i < count)
	{
		if (i != index)
			new_env[j++] = env[i];
		else
			free(env[i]);
		i++;
	}
	new_env[count - 1] = NULL;
	free(env);
	return (new_env);
}

void	remove_env_var(char ***env, char *var)
{
	int		index;
	char	**new_env;

	if (!env || !*env || !var)
		return ;
	index = get_env_index(*env, var);
	if (index == -1)
		return ;
	new_env = env_remove_at(*env, index);
	if (!new_env)
		return ;
	*env = new_env;
}
