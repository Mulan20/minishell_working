#include "../include/minishell.h"

static t_token	*skip_redir_block(t_command *cmd, t_token *cur)
{
	cur = process_redirection(cmd, cur);
	if (!cur)
		return (NULL);
	if (cur->type == WORD)
		cur = cur->next;
	return (cur);
}

t_token	*fill_one(t_command *cmd, t_token *tokens)
{
	t_token	*cur;
	int		ac;
	int		i;

	if (!tokens)
		return (NULL);
	init_command(cmd);
	ac = count_word(tokens);
	cmd->args = malloc(sizeof(char *) * (ac + 1));
	if (!cmd->args)
		return (NULL);
	cur = tokens;
	i = 0;
	while (cur && cur->type != PIPE)
	{
		if (cur->type == WORD)
			(add_word_arg(cmd, cur->value, &i), cur = cur->next);
		else if (cur->type == REDIR_IN || cur->type == REDIR_OUT
			|| cur->type == APPEND || cur->type == HEREDOC)
			cur = skip_redir_block(cmd, cur);
		else
			cur = cur->next;
	}
	cmd->args[i] = NULL;
	return (cur);
}

t_command	*fill_commands(t_token *tokens)
{
	int			n;
	t_command	*cmds;
	int			i;

	n = count_commands(tokens);
	cmds = malloc(sizeof(t_command) * n);
	if (!cmds || !tokens)
		return (NULL);
	i = 0;
	while (i < n)
	{
		tokens = fill_one(&cmds[i], tokens);
		if (tokens && tokens->type == PIPE)
			tokens = tokens->next;
		i++;
	}
	return (cmds);
}