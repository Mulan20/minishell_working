#include "../include/minishell.h"

static int	get_env_index(char **env, char *var)
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