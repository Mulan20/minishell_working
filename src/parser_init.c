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