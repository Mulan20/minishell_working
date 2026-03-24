#include "../include/minishell.h"

char	*find_executable(char *cmd, char **env)
{
	if (!cmd || !cmd[0])
		return (NULL);
	if (contain_path(cmd))
	{
		if (access(cmd, X_OK) == 0)
			return (ft_strdup(cmd));
		return (NULL);
	}
	return (find_in_path(cmd, env));
}
