#include "../include/minishell.h"

int	exec_command_child(t_command *cmd, char **env)
{
	char	*path;

	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	if (!cmd || !cmd->args || !cmd->args[0])
		exit(0);
	if (is_builtin(cmd->args[0]))
		exit(execute_builtin(cmd, &env, 0));
	path = find_executable(cmd->args[0], env);
	if (!path)
	{
		printf("minishell: %s: command not found\n", cmd->args[0]);
		exit(127);
	}
	execve(path, cmd->args, env);
	perror("execve failed");
	free(path);
	exit(126);
}
