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

typedef struct	s_fork_ctx
{
	t_command	*cmds;
	int			n;
	int			(*pipe_fds)[2];
	char		**env;
}	t_fork_ctx;

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
int			get_env_index(char **env, char *var);
/* builtins */
int			is_builtin(char *cmd);
int			builtin_echo(t_command *cmd, char **env, int last_status);
int			builtin_cd(t_command *cmd, char ***env);
int			builtin_pwd(void);
int			builtin_env(char **env);
int			builtin_exit(t_command *cmd);
int			builtin_export(t_command *cmd, char ***env);
int			builtin_unset(t_command *cmd, char ***env);
int			execute_builtin(t_command *cmd, char ***env, int last_status);

/* execution */
int			exec_command(t_command *cmd, char **env);
int			execute_external(t_command *cmd, char **env);
int			fork_execute(char *path, char **args, char **env);

/* path */
char		*find_executable(char *cmd, char **env);
char	*get_path(char **env);
int		contain_path(char *cmd);
char	*join_path_cmd(char *dir, char *cmd);
char	*find_in_paths(char **paths, char *cmd);
char	*find_in_path(char *cmd, char **env);

/* pipe.c */
void		exec_pipeline(t_command *cmds, int cmd_count, char ***env,
				int *last_status);
void	create_pipes(t_command *cmds, int cmd_count, int pipe_fds[][2]);
void	close_pipes(int cmd_count, int pipe_fds[][2]);
void	connect_prev(int prev_pipe_fd);
void	connect_next(int pipe_write_fd);
void	set_status_from_wait(int status, int *last_status);
int		exec_command_child(t_command *cmd, char **env);
pid_t	fork_all(t_command *cmds, int cmd_count, int pipe_fds[][2], char **env);
void	wait_all(int *last_status, pid_t last_pid);


/* redirect.c */
void		redirect_input(t_command *cmd);
void		redirect_output(t_command *cmd);
void		handle_heredoc(t_command *cmd);

/* signals.c */
void		setup_sig(void);

/* expansion.c */
char		*expand_string(char *str, char **envp, int last_status);

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


void		init_command(t_command *cmd);
int			count_word(t_token *tokens);
t_token		*process_redirection(t_command *cmd, t_token *current);
void		add_word_arg(t_command *cmd, char *value, int *index);
t_token		*fill_one(t_command *cmd, t_token *tokens);
#endif