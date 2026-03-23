#include "../../include/minishell.h"

int	builtin_unset(t_command *cmd, char ***env)
{
	int	i;

	if (!cmd->args[1])
		return (0);
	i = 1;
	while (cmd->args[i])
	{
		remove_env_var(env, cmd->args[i]);
		i++;
	}
	return (0);
}
