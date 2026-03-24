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