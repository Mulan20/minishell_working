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
	char	*result;

	result = malloc(20);
	if (!result)
		return (NULL);
	sprintf(result, "%d", last_status);
	return (result);
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