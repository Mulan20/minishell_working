#include "../include/minishell.h"

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