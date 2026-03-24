#include "../include/minishell.h"

void	wait_all(int *last_status, pid_t last_pid)
{
	pid_t	pid;
	int		status;

	while (1)
	{
		pid = wait(&status);
		if (pid == -1)
			break ;
		if (pid == last_pid)
			set_status_from_wait(status, last_status);
	}
}