#include "../../include/minishell.h"

int	builtin_exit(t_command *cmd)
{
	int	status;

	printf("exit\n");
	if (cmd->args[1])
	{
		status = atoi(cmd->args[1]);
		exit(status);
	}
	exit(0);
	return (0);
}
