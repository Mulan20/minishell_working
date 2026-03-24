#include "../include/minishell.h"

static int	scan_quoted(char *input, int *i)
{
	char	quote;

	quote = input[*i];
	(*i)++;
	while (input[*i] && input[*i] != quote)
		(*i)++;
	if (input[*i] != quote)
		return (fprintf(stderr, "minishell: unclosed quote\n"), -1);
	(*i)++;
	return (0);
}

static void	scan_unquoted(char *input, int *i)
{
	while (input[*i] && input[*i] != ' ' && !is_operator(input[*i]))
		(*i)++;
}

t_token	*handle_word(t_token *tokens, char *input, int *i)
{
	int		start;
	t_token	*new;

	start = *i;
	if (input[*i] == '"' || input[*i] == '\'')
	{
		if (scan_quoted(input, i) == -1)
			return (NULL);
	}
	else
		scan_unquoted(input, i);
	new = create_token(WORD, ft_substr(input, start, *i - start));
	if (!new)
	{
		free_tokens(tokens);
		return (NULL);
	}
	add_token(&tokens, new);
	return (tokens);
}

//for debug purposes
// char	*get_token_type(t_token_type type)
// {
// 	if (type == WORD)
// 		return ("WORD");
// 	if (type == PIPE)
// 		return ("PIPE");
// 	if (type == REDIR_IN)
// 		return ("REDIR_IN");
// 	if (type == REDIR_OUT)
// 		return ("REDIR_OUT");
// 	if (type == APPEND)
// 		return ("APPEND");
// 	if (type == HEREDOC)
// 		return ("HEREDOC");
// 	return ("UNKNOWN");
// }

// void	print_tokens(t_token *tokens)
// {
// 	while (tokens)
// 	{
// 		printf("TYPE: %s VALUE: %s\n",
// 			get_token_type(tokens->type),
// 			tokens->value ? tokens->value : "(null)");
// 		tokens = tokens->next;
// 	}
// }
