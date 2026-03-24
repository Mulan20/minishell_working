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

char	*get_path(char **env)
{
	int	i;

	i = 0;
	while (env[i])
	{
		if (ft_strncmp(env[i], "PATH=", 5) == 0)
			return (env[i] + 5);
		i++;
	}
	return (NULL);
}

int	contain_path(char *cmd)
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

char	*join_path_cmd(char *dir, char *cmd)
{
	char	*full;
	int		len_dir;
	int		len_cmd;

	len_dir = ft_strlen(dir);
	len_cmd = ft_strlen(cmd);
	full = malloc(len_dir + len_cmd + 2);
	if (!full)
		return (NULL);
	ft_strcpy(full, dir);
	ft_strcat(full, "/");
	ft_strcat(full, cmd);
	return (full);
}

char	*find_in_paths(char **paths, char *cmd)
{
	char	*full;
	char	*result;
	int		i;

	i = 0;
	while (paths[i])
	{
		full = join_path_cmd(paths[i], cmd);
		if (!full)
			return (NULL);
		if (access(full, X_OK) == 0)
		{
			result = ft_strdup(full);
			free(full);
			return (result);
		}
		free(full);
		i++;
	}
	return (NULL);
}

char	*find_in_path(char *cmd, char **env)
{
	char	*path;
	char	**paths;
	char	*result;

	path = get_path(env);
	if (!path)
		return (NULL);
	paths = ft_split(path, ':');
	if (!paths)
		return (NULL);
	result = find_in_paths(paths, cmd);
	free_split(paths);
	return (result);
}

