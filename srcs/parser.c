#include "../include/minishell.h"

void	init_command(t_command *cmd)
{
	cmd->args = NULL;
	cmd->infile = NULL;
	cmd->outfile = NULL;
	cmd->heredoc = NULL;
	cmd->append = 0;
	cmd->pipe_in = 0;
	cmd->pipe_out = 0;
}

// int	count_word(t_token *tokens)
// {
// 	int	count;

// 	count = 0;
// 	while (tokens && tokens->type != PIPE)
// 	{
// 		if (tokens->type == WORD)
// 			count++;
// 		tokens = tokens->next;
// 	}
// 	return (count);
// }
int	count_word(t_token *tokens)
{
	int	count;
	t_token	*prev;

	count = 0;
	prev = NULL;
	while (tokens && tokens->type != PIPE)
	{
		if (tokens->type == WORD)
		{
			// If previous token was a redirection, skip this word (it's a target)
			if (prev == NULL || (prev->type != REDIR_IN 
				&& prev->type != REDIR_OUT
				&& prev->type != APPEND 
				&& prev->type != HEREDOC))
			{
				count++;
			}
		}
		prev = tokens;
		tokens = tokens->next;
	}
	return (count);
}

// char	*ft_strdup(char *str)
// {
// 	int		i;
// 	char	*res;

// 	i = 0;
// 	if (!str)
// 		return (NULL);
// 	while (str[i])
// 		i++;
// 	res = malloc(sizeof(char) * i + 1);
// 	if (!res)
// 		return (NULL);
// 	i = 0;
// 	while (str[i])
// 	{
// 		res[i] = str[i];
// 		i++;
// 	}
// 	res[i] = '\0';
// 	return (res);
// }

t_token	*process_redirection(t_command *cmd, t_token *current)
{
	if (!current->next)
		return (NULL);
	if (current->type == REDIR_IN && current->next->type == WORD)
	{
		if (cmd->infile)
			free(cmd->infile);
		cmd->infile = ft_strdup(current->next->value);  
	}
	else if (current->type == REDIR_OUT && current->next->type == WORD)
	{
		if (cmd->outfile)
			free(cmd->outfile);
		cmd->outfile = ft_strdup(current->next->value);
		cmd->append = 0;
	}
	else if (current->type == APPEND && current->next->type == WORD)
	{
		if (cmd->outfile)
			free(cmd->outfile);
		cmd->outfile = ft_strdup(current->next->value);
		cmd->append = 1;
	}
	else if (current->type == HEREDOC && current->next->type == WORD)
	{
		if (cmd->heredoc)
			free(cmd->heredoc);
		cmd->heredoc = ft_strdup(current->next->value);
	}
	return (current->next);
}

// void	add_word_arg(t_command *cmd, char *value, int *index)
// {
// 	if (!value)
// 		return ;
// 	cmd->args[*index] = ft_strdup(value);
// 	(*index)++;
// }

char	*remove_quotes(char *str)
{
	int		len;
	char	*result;
	int		i;

	if (!str || ft_strlen(str) < 2)
		return (ft_strdup(str));
	
	// Check if starts and ends with matching quotes
	if ((str[0] == '"' || str[0] == '\'') && str[ft_strlen(str) - 1] == str[0])
	{
		len = ft_strlen(str) - 2;
		result = malloc(len + 1);
		if (!result)
			return (NULL);
		i = 0;
		while (i < len)
		{
			result[i] = str[i + 1];
			i++;
		}
		result[len] = '\0';
		return (result);
	}
	return (ft_strdup(str));
}

void	add_word_arg(t_command *cmd, char *value, int *index)
{
	if (!value)
		return;
	// cmd->args[*index] = remove_quotes(value);
	cmd->args[*index] = ft_strdup(value);
	(*index)++;
}
int	count_commands(t_token *tokens)
{
	int	count;

	count = 1;
	if (!tokens)
		return (0);
	while (tokens)
	{
		if (tokens->type == PIPE)
			count++;
		tokens = tokens->next;
	}
	return (count);
}

t_token	*fill_one(t_command *cmd, t_token *tokens)
{
	t_token	*current;
	int		ac;
	int		i;

	if (!tokens)
		return (NULL);
	init_command(cmd);
	ac = count_word(tokens);
	cmd->args = malloc(sizeof(char *) * (ac + 1));
	if (!cmd->args)
		return (NULL);
	current = tokens;
	i = 0;
	while (current && current->type != PIPE)
	{
		if (current->type == WORD)
		{
			add_word_arg(cmd, current->value, &i);
			current = current->next;
		}
		else if (current->type == REDIR_IN || current->type == REDIR_OUT
			|| current->type == APPEND || current->type == HEREDOC)
		{
			current = process_redirection(cmd, current);
			if (!current)
				break ;
			if (current && current->type == WORD)
				current = current->next;
		}
		else
			current = current->next;
	}
	cmd->args[i] = NULL;
	return (current);
}

t_command	*fill_commands(t_token *tokens)
{
	int			cmd_count;
	t_command	*cmds;
	int			i;

	cmd_count = count_commands(tokens);
	cmds = malloc(sizeof(t_command) * cmd_count);
	if (!cmds || !tokens)
		return (NULL);
	i = 0;
	while (i < cmd_count)
	{
		tokens = fill_one(&cmds[i], tokens);
		if (tokens && tokens->type == PIPE)
			tokens = tokens->next;
		i++;
	}
	return (cmds);
}

void	set_pipestat(t_command *cmds, int count)
{
	int	i;

	i = 0;
	while (i < count)
	{
		cmds[i].pipe_in = (i != 0);
		cmds[i].pipe_out = (i != count - 1);
		i++;
	}
}

static void	print_cmd_args(t_command *cmd)
{
	int	j;

	if (!cmd->args)
		return ;
	j = 0;
	while (cmd->args[j])
	{
		printf("  arg[%d]: %s\n", j, cmd->args[j]);
		j++;
	}
}

static void	print_cmd_redirs(t_command *cmd)
{
	if (cmd->infile)
		printf("  Input from: %s\n", cmd->infile);
	if (cmd->outfile)
		printf("  Output to: %s (append: %d)\n", cmd->outfile, cmd->append);
	if (cmd->heredoc)
		printf("  Heredoc delimiter: %s\n", cmd->heredoc);
}

void	print_cmds(t_command *cmds, int count)
{
	int	i;

	i = 0;
	while (i < count)
	{
		printf("Command %d:\n", i);
		print_cmd_args(&cmds[i]);
		print_cmd_redirs(&cmds[i]);
		printf("  pipe_in: %d\n", cmds[i].pipe_in);
		printf("  pipe_out: %d\n", cmds[i].pipe_out);
		i++;
	}
	// print_builtin(cmds, count);
}