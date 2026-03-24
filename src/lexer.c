#include "../include/minishell.h"

static t_token	*lex_fail(t_token *tokens)
{
	free_tokens(tokens);
	return (NULL);
}

static t_token	*handle_token(t_token *tokens, char *input, int *i)
{
	if (input[*i] == '|')
		return (handle_pipe(tokens));
	else if (input[*i] == '>')
		return (handle_redir_out(tokens, input, i));
	else if (input[*i] == '<')
		return (handle_redir_in(tokens, input, i));
	else
		return (handle_word(tokens, input, i));
}

static t_token	*process_token(t_token *tokens, char *input, int *i)
{
	t_token	*tmp;
	char	c;

	c = input[*i];
	tmp = handle_token(tokens, input, i);
	if (!tmp)
		return (NULL);
	if (c == '|')
		(*i)++;
	return (tmp);
}

t_token	*define_token(char *input)
{
	t_token	*tokens;
	t_token	*tmp;
	int		i;

	if (!input)
		return (NULL);
	tokens = NULL;
	i = 0;
	while (input[i])
	{
		skip_spaces(input, &i);
		if (!input[i])
			break ;
		tmp = process_token(tokens, input, &i);
		if (!tmp)
			return (lex_fail(tokens));
		tokens = tmp;
	}
	return (tokens);
}