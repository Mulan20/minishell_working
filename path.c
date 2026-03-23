/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkham <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/23 12:23:09 by nkham             #+#    #+#             */
/*   Updated: 2026/03/23 12:48:41 by nkham            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static char	*get_path(char **env)
{
	int	i;

	i = 0;
	while (env[i])
	{
		if (strncmp(env[i], "PATH=", 5) == 0)
			return (env[i] + 5);
		i++;
	}
	return (NULL);
}

static int	contain_path(char *cmd)
{
	int	i;

	i = 0;
	while (cmd[i])
	{
		if (cmd[i] == '/')
			return (1);
		i++;
	}
	return (0);
}

static char	*find_in_path(char *cmd, char **env)
{
	char	*path;
	char	**paths;
	char	*full_path;
	char	*result;
	int		i;

	path = get_path(env);
	if (!path)
		return (NULL);
	paths = ft_split(path, ':');
	if (!paths)
		return (NULL);
	i = 0;
	while (paths[i])
	{
		full_path = malloc(ft_strlen(paths[i]) + ft_strlen(cmd) + 2);
		if (!full_path)
		{
			free_split(paths);
			return (NULL);
		}
		sprintf(full_path, "%s/%s", paths[i], cmd);
		if (access(full_path, X_OK) == 0)
		{
			result = ft_strdup(full_path);
			free(full_path);
			free_split(paths);
			return (result);
		}
		free(full_path);
		i++;
	}
	free_split(paths);
	return (NULL);
}

char	*find_executable(char *cmd, char **env)
{
	if (!cmd || !cmd[0])
		return (NULL);
	if (contain_path(cmd))
	{
		if (access(cmd, X_OK) == 0)
			return (strdup(cmd));
		return (NULL);
	}
	return (find_in_path(cmd, env));
}
