/* builtins.c - Updated version */

#include "../include/minishell.h"

int	is_builtin(char *cmd)
{
	if (!cmd)
		return (0);
	if (ft_strcmp(cmd, "echo") == 0)
		return (1);
	if (ft_strcmp(cmd, "cd") == 0)
		return (1);
	if (ft_strcmp(cmd, "pwd") == 0)
		return (1);
	if (ft_strcmp(cmd, "export") == 0)
		return (1);
	if (ft_strcmp(cmd, "unset") == 0)
		return (1);
	if (ft_strcmp(cmd, "env") == 0)
		return (1);
	if (ft_strcmp(cmd, "exit") == 0)
		return (1);
	return (0);
}

int	builtin_pwd(void)
{
	char	cwd[1024];

	if (getcwd(cwd, sizeof(cwd)))
	{
		printf("%s\n", cwd);
		return (0);
	}
	perror("pwd");
	return (1);
}

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

// int	builtin_cd(t_command *cmd)
// {
// 	char	*path;

// 	if (!cmd->args[1])
// 	{
// 		path = getenv("HOME");
// 		if (!path)
// 		{
// 			printf("minishell: cd: HOME not set\n");
// 			return (1);
// 		}
// 	}
// 	else
// 		path = cmd->args[1];
// 	if (chdir(path) == -1)
// 	{
// 		printf("minishell: cd: %s: %s\n", path, strerror(errno));
// 		return (1);
// 	}
// 	return (0);
// }

int	builtin_cd(t_command *cmd, char ***env)
{
	char	*path;
	char	old_pwd[1024];
	char	*old_pwd_ptr;
	char	new_pwd[1024];

	if (!cmd->args[1] || ft_strcmp(cmd->args[1], "~") == 0)
	{
		path = get_env_value(*env, "HOME");
		if (!path)
		{
			printf("minishell: cd: HOME not set\n");
			return (1);
		}
	}
	else if (ft_strcmp(cmd->args[1], "-") == 0)
	{
		path = get_env_value(*env, "OLDPWD");
		if (!path)
		{
			printf("minishell: cd: OLDPWD not set\n");
			return (1);
		}
		printf("%s\n", path);
	}
	else
		path = cmd->args[1];
	if (!getcwd(old_pwd, sizeof(old_pwd)))
	{
		perror("getcwd");
		return (1);
	}
	if (chdir(path) == -1)
	{
		printf("minishell: cd: %s: %s\n", path, strerror(errno));
		return (1);
	}
	old_pwd_ptr = get_env_value(*env, "PWD");
	if (old_pwd_ptr)
		add_or_update_env(env, "OLDPWD", old_pwd_ptr);
	else
		add_or_update_env(env, "OLDPWD", old_pwd);
	if (getcwd(new_pwd, sizeof(new_pwd)))
		add_or_update_env(env, "PWD", new_pwd);
	return (0);
}

int	builtin_echo(t_command *cmd, char **env)
{
	int		i;
	int		newline;
	int		first;
	char	*expanded;

	i = 1;
	newline = 1;
	first = 1;
	if (cmd->args[i] && ft_strcmp(cmd->args[i], "-n") == 0)
	{
		newline = 0;
		i++;
	}
	while (cmd->args[i])
	{
		if (!first)
			printf(" ");
		expanded = expand_string(cmd->args[i], env);
		printf("%s", expanded);
		free(expanded);
		first = 0;
		i++;
	}
	if (newline)
		printf("\n");
	fflush(stdout);
	return (0);
}

int	builtin_export(t_command *cmd, char ***env)
{
	int		i;
	char	*equals_pos;
	char	*var_name;
	char	*var_value;

	if (!cmd->args[1])
		return (builtin_env(*env));
	i = 1;
	while (cmd->args[i])
	{
		equals_pos = ft_strchr(cmd->args[i], '=');
		if (equals_pos)
		{
			var_name = ft_substr(cmd->args[i], 0, equals_pos - cmd->args[i]);
			var_value = ft_strdup(equals_pos + 1);
			if (var_name && var_value)
				add_or_update_env(env, var_name, var_value);
			free(var_name);
			free(var_value);
		}
		else
		{
			if (!env_var_exists(*env, cmd->args[i]))
				add_or_update_env(env, cmd->args[i], "");
		}
		i++;
	}
	return (0);
}

int	execute_builtin(t_command *cmd, char ***env)
{
	if (!cmd || !cmd->args || !cmd->args[0])
		return (1);
	if (ft_strcmp(cmd->args[0], "echo") == 0)
		return (builtin_echo(cmd, *env));
	if (ft_strcmp(cmd->args[0], "cd") == 0)
		return (builtin_cd(cmd, env));
	if (ft_strcmp(cmd->args[0], "pwd") == 0)
		return (builtin_pwd());
	if (ft_strcmp(cmd->args[0], "env") == 0)
		return (builtin_env(*env));
	if (ft_strcmp(cmd->args[0], "exit") == 0)
		return (builtin_exit(cmd));
	if (ft_strcmp(cmd->args[0], "export") == 0)
		return (builtin_export(cmd, env));
	if (ft_strcmp(cmd->args[0], "unset") == 0)
		return (builtin_unset(cmd, env));
	return (1);
}
