/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkham <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/23 11:40:31 by nkham             #+#    #+#             */
/*   Updated: 2026/03/23 12:49:34 by nkham            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

volatile sig_atomic_t	g_signal = 0;

static int	is_empty_input(char *input)
{
	int	i;

	i = 0;
	while (input[i] == ' ' || input[i] == '\t')
		i++;
	return (input[i] == '\0');
}

static void	process_input(char *input, char ***env)
{
	t_token		*tokens;
	int			cmd_count;
	t_command	*cmds;

	tokens = define_token(input);
	if (!tokens)
		return ;
	cmd_count = count_commands(tokens);
	cmds = fill_commands(tokens);
	if (cmds)
	{
		set_pipestat(cmds, cmd_count);
		exec_pipeline(cmds, cmd_count, env);
		free_commands(cmds, cmd_count);
	}
	free_tokens(tokens);
}

int	main(int ac, char **av, char **envp)
{
	char		*input;
	char		**env;

	(void)ac;
	(void)av;
	env = copy_env(envp);
	if (!env)
		return (1);
	setup_sig();
	while (1)
	{
		if (g_signal == SIGINT)
		{
			g_signal = 0;
			continue;
		}
		input = get_user_input();
		if (!input)
			break ;
		if (is_empty_input(input))
		{
			free(input);
			continue;
		}
		process_input(input, &env);
		free(input);
		g_signal = 0;
	}
	rl_clear_history();
	free_env(env);
	return (0);
}
