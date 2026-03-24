/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkham <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/24 17:07:49 by nkham             #+#    #+#             */
/*   Updated: 2026/03/24 17:08:50 by nkham            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// #include "../../include/minishell.h"

// static void	handle_export_var(char ***env, char *arg)
// {
// 	char	*equals_pos;
// 	char	*var_name;
// 	char	*var_value;

// 	equals_pos = ft_strchr(arg, '=');
// 	if (equals_pos)
// 	{
// 		var_name = ft_substr(arg, 0, equals_pos - arg);
// 		var_value = ft_strdup(equals_pos + 1);
// 		if (var_name && var_value)
// 			add_or_update_env(env, var_name, var_value);
// 		free(var_name);
// 		free(var_value);
// 	}
// 	else
// 	{
// 		if (!env_var_exists(*env, arg))
// 			add_or_update_env(env, arg, "");
// 	}
// }

// int	builtin_export(t_command *cmd, char ***env)
// {
// 	int	i;

// 	if (!cmd->args[1])
// 		return (builtin_env(*env));
// 	i = 1;
// 	while (cmd->args[i])
// 	{
// 		handle_export_var(env, cmd->args[i]);
// 		i++;
// 	}
// 	return (0);
// }

#include "../../include/minishell.h"

static char	*strip_surrounding_quotes(char *s)
{
	int		len;
	char	q;

	if (!s)
		return (NULL);
	len = ft_strlen(s);
	if (len >= 2 && ((s[0] == '"' && s[len - 1] == '"')
			|| (s[0] == '\'' && s[len - 1] == '\'')))
	{
		q = s[0];
		(void)q;
		return (ft_substr(s, 1, len - 2));
	}
	return (ft_strdup(s));
}

static int	is_valid_ident(char *s)
{
	int	i;

	if (!s || !s[0])
		return (0);
	if (!((s[0] >= 'A' && s[0] <= 'Z')
			|| (s[0] >= 'a' && s[0] <= 'z')
			|| s[0] == '_'))
		return (0);
	i = 1;
	while (s[i] && s[i] != '=')
	{
		if (!((s[i] >= 'A' && s[i] <= 'Z')
				|| (s[i] >= 'a' && s[i] <= 'z')
				|| (s[i] >= '0' && s[i] <= '9')
				|| s[i] == '_'))
			return (0);
		i++;
	}
	return (1);
}

static void	handle_export_var(char ***env, char *arg)
{
	char	*equals_pos;
	char	*var_name;
	char	*raw_value;
	char	*var_value;

	if (!is_valid_ident(arg))
		return ;
	equals_pos = ft_strchr(arg, '=');
	if (equals_pos)
	{
		var_name = ft_substr(arg, 0, equals_pos - arg);
		raw_value = equals_pos + 1;
		var_value = strip_surrounding_quotes(raw_value);
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
	int	ret;

	if (!cmd->args[1])
		return (builtin_env(*env));
	i = 1;
	ret = 0;
	while (cmd->args[i])
	{
		if (!is_valid_ident(cmd->args[i]))
		{
			printf("minishell: export: `%s': not a valid identifier\n",
				cmd->args[i]);
			ret = 1;
		}
		else
			handle_export_var(env, cmd->args[i]);
		i++;
	}
	return (ret);
}
