#include "../../include/minishell.h"

static void	handle_export_var(char ***env, char *arg)
{
	char	*equals_pos;
	char	*var_name;
	char	*var_value;

	equals_pos = ft_strchr(arg, '=');
	if (equals_pos)
	{
		var_name = ft_substr(arg, 0, equals_pos - arg);
		var_value = ft_strdup(equals_pos + 1);
		if (var_name && var_value)
			add_or_update_env(env, var_name, var_value);
		free(var_name);
		free(var_value);
	}
	else
	{
		if (!env_var_exists(*env, arg))
			add_or_update_env(env, arg, "");
	}
}

int	builtin_export(t_command *cmd, char ***env)
{
	int	i;

	if (!cmd->args[1])
		return (builtin_env(*env));
	i = 1;
	while (cmd->args[i])
	{
		handle_export_var(env, cmd->args[i]);
		i++;
	}
	return (0);
}
