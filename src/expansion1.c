#include "../include/minishell.h"

static int	init_expand(t_expand *e, char *str, char **envp, int last_status)
{
	e->out = malloc(4096);
	if (!e->out)
		return (-1);
	e->str = str;
	e->envp = envp;
	e->i = 0;
	e->j = 0;
	e->sq = 0;
	e->dq = 0;
	e->last_status = last_status;
	return (0);
}

static int	step_expand(t_expand *e)
{
	if (toggle_quotes(e->str[e->i], &e->sq, &e->dq))
		return (e->i++, 0);
	if (e->str[e->i] == '$' && !e->sq && e->str[e->i + 1])
		return (expand_dollar(e));
	e->out[e->j++] = e->str[e->i++];
	return (0);
}

char	*expand_string(char *str, char **envp, int last_status)
{
	t_expand	e;

	if (!str)
		return (ft_strdup(""));
	if (!envp)
		return (ft_strdup(str));
	if (init_expand(&e, str, envp, last_status) == -1)
		return (NULL);
	while (e.str[e.i] && e.j < 4090)
	{
		if (step_expand(&e) == -1)
			return (free(e.out), NULL);
	}
	e.out[e.j] = '\0';
	return (e.out);
}