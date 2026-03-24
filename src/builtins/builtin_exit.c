/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkham <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/24 17:05:47 by nkham             #+#    #+#             */
/*   Updated: 2026/03/24 17:11:16 by nkham            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static int	ft_isdigit(char c)
{
	if (c < '0' || c > '9')
		return (0);
	return (1);
}

static int	is_numeric(char *str)
{
	int	i;

	i = 0;
	if (str[i] == '-' || str[i] == '+')
		i++;
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}

int	builtin_exit(t_command *cmd)
{
	printf("exit\n");
	if (cmd->args[1] && cmd->args[2])
	{
		printf("exit: too many arguments\n");
		return (1);
	}
	if (cmd->args[1])
	{
		if (!is_numeric(cmd->args[1]))
		{
			printf("exit: %s: numeric argument required\n", cmd->args[1]);
			exit(2);
		}
		exit(ft_atoi(cmd->args[1]));
	}
	exit(0);
	return (0);
}
