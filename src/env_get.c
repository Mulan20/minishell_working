#include "../include/minishell.h"

int	get_env_index(char **env, char *var)
{
	int	i;
	int	var_len;

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
	int	index;
	int	len;

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
