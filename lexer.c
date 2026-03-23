#include "../include/minishell.h"

char	*get_token_type(t_token_type type)
{
	if (type == WORD)
		return ("WORD");
	if (type == PIPE)
		return ("PIPE");
	if (type == REDIR_IN)
		return ("REDIR_IN");
	if (type == REDIR_OUT)
		return ("REDIR_OUT");
	if (type == APPEND)
		return ("APPEND");
	if (type == HEREDOC)
		return ("HEREDOC");
	return ("UNKNOWN");
}

char	*ft_substr(char *s, int start, int len)
{
	char	*res;
	int		i;

	res = malloc(len + 1);
	if (!res)
		return (NULL);
	i = 0;
	while (i < len)
	{
		res[i] = s[start + i];
		i++;
	}
	res[i] = '\0';
	return (res);
}

void	skip_spaces(char *input, int *i)
{
	while (input[*i] == ' ' || input[*i] == '\t')
		(*i)++;
}

int	is_operator(char c)
{
	return (c == '|' || c == '<' || c == '>');
}

t_token	*create_token(t_token_type type, char *value)
{
	t_token	*new;

	new = malloc(sizeof(t_token));
	if (!new)
		return (NULL);
	new->type = type;
	new->value = value;
	new->next = NULL;
	return (new);
}

void	add_token(t_token **list, t_token *new)
{
	t_token	*temp;

	if (!*list)
	{
		*list = new;
		return ;
	}
	temp = *list;
	while (temp->next)
		temp = temp->next;
	temp->next = new;
}

void	print_tokens(t_token *tokens)
{
	while (tokens)
	{
		printf("TYPE: %s VALUE: %s\n",
			get_token_type(tokens->type), 
			tokens->value ? tokens->value : "(null)");
		tokens = tokens->next;
	}
}

static t_token	*handle_pipe(t_token *tokens)
{
	t_token	*new;

	new = create_token(PIPE, NULL);
	if (!new)
	{
		free_tokens(tokens);
		return (NULL);
	}
	add_token(&tokens, new);
	return (tokens);
}

static t_token	*handle_redir_out(t_token *tokens, char *input, int *i)
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
	{
		free_tokens(tokens);
		return (NULL);
	}
	add_token(&tokens, new);
	return (tokens);
}

static t_token	*handle_redir_in(t_token *tokens, char *input, int *i)
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
	{
		free_tokens(tokens);
		return (NULL);
	}
	add_token(&tokens, new);
	return (tokens);
}

// static t_token	*handle_word(t_token *tokens, char *input, int *i)
// {
// 	int		start;
// 	t_token	*new;

// 	start = *i;
// 	while (input[*i] && input[*i] != ' ' && !is_operator(input[*i]))
// 		(*i)++;
// 	new = create_token(WORD, ft_substr(input, start, *i - start));
// 	if (!new)
// 	{
// 		free_tokens(tokens);
// 		return (NULL);
// 	}
// 	add_token(&tokens, new);
// 	return (tokens);
// }

static t_token	*handle_word(t_token *tokens, char *input, int *i)
{
	int		start;
	t_token	*new;
	char	quote;

	start = *i;
	quote = 0;
	
	// Handle quoted strings
	if (input[*i] == '"' || input[*i] == '\'')
	{
		quote = input[*i];
		(*i)++;  // Skip opening quote
		while (input[*i] && input[*i] != quote)
			(*i)++;
		if (input[*i] != quote)
			{
				fprintf(stderr, "minishell: unclosed quote\n");
				return (NULL);  // Return NULL to signal error
			}
		if (input[*i] == quote)
			(*i)++;  // Skip closing quote
	}
	else
	{
		// Handle unquoted words (stop at space or operator)
		while (input[*i] && input[*i] != ' ' && !is_operator(input[*i]))
			(*i)++;
	}
	
	new = create_token(WORD, ft_substr(input, start, *i - start));
	if (!new)
	{
		free_tokens(tokens);
		return (NULL);
	}
	add_token(&tokens, new);
	return (tokens);
}
t_token	*define_token(char *input)
{
	t_token	*tokens;
	int		i;

	tokens = NULL;
	i = 0;
	if (!input)
		return (NULL);
	while (input[i])
	{
		skip_spaces(input, &i);
		if (!input[i])
			break ;
		if (input[i] == '|')
		{
			tokens = handle_pipe(tokens);
			i++;
		}
		else if (input[i] == '>')
			tokens = handle_redir_out(tokens, input, &i);
		else if (input[i] == '<')
			tokens = handle_redir_in(tokens, input, &i);
		else
			tokens = handle_word(tokens, input, &i);
		if (!tokens)
			return (NULL);

	}
	return (tokens);
}

