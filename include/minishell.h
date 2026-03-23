/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkham <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/23 11:40:31 by nkham             #+#    #+#             */
/*   Updated: 2026/03/23 13:34:37 by nkham            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <sys/wait.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <signal.h>
# include <fcntl.h>
# include <errno.h>
# include <string.h>

extern volatile sig_atomic_t	g_signal;

typedef enum e_token_type
{
	WORD,
	PIPE,
	REDIR_IN,
	REDIR_OUT,
	APPEND,
	HEREDOC
}	t_token_type;

typedef struct s_command
{
	char	**args;
	int		pipe_in;
	int		pipe_out;
	char	*infile;
	char	*outfile;
	char	*heredoc;
	int		append;
}	t_command;

typedef struct s_token
{
	char			*value;
	t_token_type	type;
	struct s_token	*next;
}	t_token;

/* input.c */
char		*get_user_input(void);

/* lexer.c */
t_token		*define_token(char *input);
void		free_tokens(t_token *tokens);
void		print_tokens(t_token *tokens);

/* parser.c */
int			count_commands(t_token *tokens);
t_command	*fill_commands(t_token *tokens);
void		free_commands(t_command *cmds, int count);
void		print_cmds(t_command *cmds, int count);
void		set_pipestat(t_command *cmds, int count);

/* env.c */
char		**copy_env(char **envp);
void		free_env(char **env);
char		*get_env_value(char **env, char *var);
void		add_or_update_env(char ***env, char *var, char *value);
void		remove_env_var(char ***env, char *var);
int			env_var_exists(char **env, char *var);

/* builtins */
int			is_builtin(char *cmd);
int			builtin_echo(t_command *cmd, char **env);
int			builtin_cd(t_command *cmd, char ***env);
int			builtin_pwd(void);
int			builtin_env(char **env);
int			builtin_exit(t_command *cmd);
int			builtin_export(t_command *cmd, char ***env);
int			builtin_unset(t_command *cmd, char ***env);
int			execute_builtin(t_command *cmd, char ***env);
void		print_builtin(t_command *cmds, int count);
/* execution */
int			exec_command(t_command *cmd, char **env);
int			execute_external(t_command *cmd, char **env);
int			fork_execute(char *path, char **args, char **env);
char		*find_executable(char *cmd, char **env);

/* pipe.c */
void		exec_pipeline(t_command *cmds, int cmd_count, char ***env);

/* redirect.c */
void		redirect_input(t_command *cmd);
void		redirect_output(t_command *cmd);
void		handle_heredoc(t_command *cmd);

/* signals.c */
void		setup_sig(void);

/* expansion.c */
char		*expand_string(char *str, char **envp);

/* status.c */
int			get_last_exit_status(void);
void		set_last_exit_status(int status);

/* utils */
char		**ft_split(char const *s, char c);
void		free_split(char **split);
int			ft_strcmp(char *s1, const char *s2);
int			ft_strlen(char *s);
int			ft_strncmp(const char *s1, const char *s2, size_t n);
char		*ft_strchr(const char *s, int c);
char		*ft_strdup(char *str);
char		*ft_strcat(char *dest, const char *src);
char		*ft_substr(char *s, int start, int len);
char		*ft_strcpy(char *s1, char *s2);

#endif
