/* header ... */
#include "../include/minishell.h"

static t_token	*token_err(t_token *tokens)
{
	free_tokens(tokens);
	return (NULL);
}

t_token	*handle_pipe(t_token *tokens)
{
	t_token	*new;

	new = create_token(PIPE, NULL);
	if (!new)
		return (token_err(tokens));
	add_token(&tokens, new);
	return (tokens);
}



t_token	*handle_redir_out(t_token *tokens, char *input, int *i)
{
	t_token	*new;

	if (input[*i + 1] == '>')
	{
		new = create_token(APPEND, NULL);
		(*i)++;
	}
	else
		new = create_token(REDIR_OUT, NULL);
	(*i)++;
	if (!new)
		return (token_err(tokens));
	add_token(&tokens, new);
	return (tokens);
}

t_token	*handle_redir_in(t_token *tokens, char *input, int *i)
{
	t_token	*new;

	if (input[*i + 1] == '<')
	{
		new = create_token(HEREDOC, NULL);
		(*i)++;
	}
	else
		new = create_token(REDIR_IN, NULL);
	(*i)++;
	if (!new)
		return (token_err(tokens));
	add_token(&tokens, new);
	return (tokens);
}
