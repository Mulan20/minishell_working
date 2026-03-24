#include "../../include/minishell.h"

static int	has_n_flag(char **args, int *i)
{
	int	has_n;

	has_n = 0;
	while (args[*i] && ft_strcmp(args[*i], "-n") == 0)
	{
		has_n = 1;
		(*i)++;
	}
	return (has_n);
}

static void	print_echo_args(t_command *cmd, char **env, int start,
		int last_status)
{
	int		i;
	int		first;
	char	*expanded;

	i = start;
	first = 1;
	while (cmd->args[i])
	{
		if (!first)
			printf(" ");
		expanded = expand_string(cmd->args[i], env, last_status);
		printf("%s", expanded);
		free(expanded);
		first = 0;
		i++;
	}
}

int	builtin_echo(t_command *cmd, char **env, int last_status)
{
	int	i;
	int	newline;

	i = 1;
	newline = 1;
	if (has_n_flag(cmd->args, &i))
		newline = 0;
	print_echo_args(cmd, env, i, last_status);
	if (newline)
		printf("\n");
	fflush(stdout);
	return (0);
}