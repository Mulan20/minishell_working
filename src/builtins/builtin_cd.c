#include "../../include/minishell.h"

static char	*get_cd_path(t_command *cmd, char ***env)
{
	if (!cmd->args[1] || ft_strcmp(cmd->args[1], "~") == 0)
		return (get_env_value(*env, "HOME"));
	if (ft_strcmp(cmd->args[1], "-") == 0)
		return (get_env_value(*env, "OLDPWD"));
	return (cmd->args[1]);
}

static int	handle_cd_error(char *path)
{
	printf("minishell: cd: %s: %s\n", path, strerror(errno));
	return (1);
}

static void	update_cd_env(char ***env, char *old_pwd)
{
	char	*old_pwd_ptr;
	char	new_pwd[1024];

	old_pwd_ptr = get_env_value(*env, "PWD");
	if (old_pwd_ptr)
		add_or_update_env(env, "OLDPWD", old_pwd_ptr);
	else
		add_or_update_env(env, "OLDPWD", old_pwd);
	if (getcwd(new_pwd, sizeof(new_pwd)))
		add_or_update_env(env, "PWD", new_pwd);
}

int	builtin_cd(t_command *cmd, char ***env)
{
	char	*path;
	char	old_pwd[1024];

	path = get_cd_path(cmd, env);
	if (!path)
	{
		if (!cmd->args[1] || ft_strcmp(cmd->args[1], "~") == 0)
			printf("minishell: cd: HOME not set\n");
		else if (cmd->args[1] && ft_strcmp(cmd->args[1], "-") == 0)
			printf("minishell: cd: OLDPWD not set\n");
		return (1);
	}
	if (cmd->args[1] && ft_strcmp(cmd->args[1], "-") == 0)
		printf("%s\n", path);
	if (!getcwd(old_pwd, sizeof(old_pwd)))
		return (1);
	if (chdir(path) == -1)
		return (handle_cd_error(path));
	update_cd_env(env, old_pwd);
	return (0);
}
