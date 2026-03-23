#include "../include/minishell.h"

void    handle_sig_in(int sig)
{
    g_signal = sig;
    write(1, "\n", 1);
    rl_on_new_line(); //to tell readline tht I am on a new line
    rl_replace_line("", 0); //to clear the current input line
    rl_redisplay(); //to redisplaythe prompt
}

void    handle_sigquit(int sig)
{
    (void)sig; //this does nt as Ctrl-\ does nth. 
}

void    setup_sig(void)
{
    struct sigaction sa;

    sa.sa_handler = handle_sig_in;
    sigemptyset(&sa.sa_mask); //to empty all signals from the set
    sa.sa_flags = SA_RESTART;
    sigaction(SIGINT, &sa, NULL);
    sa.sa_handler = handle_sigquit;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(SIGQUIT, &sa, NULL);

    signal(SIGTERM, SIG_IGN);
}