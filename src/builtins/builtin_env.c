#include "../../include/minishell.h"

int	builtin_env(char **env)
{
	int	i;

	if (!env)
		return (1);
	i = 0;
	while (env[i])
	{
		printf("%s\n", env[i]);
		i++;
	}
	return (0);
}
