#include "../include/minishell.h"

// Ctrl-C = SIGINT	-> displays a new prompt on a new line.
// Ctrl-D = EOF		-> exits the shell.
// Ctrl-\ = SIGQUIT	-> does nothing.

typedef enum	signal_mode
{
	INTERACTIVE,
	EXECUTION,
	CHILD,
	HEREDOC
}	t_sigmode;

int	g_signal;

// void	sig_interactive_handler(int signal)
// {
// 	g_signal = signal;
// 	if (signal == SIGINT)
// 	{
// 		write(STDOUT_FILENO, "\n", 1);
// 		rl_on_new_line();
// 		rl_replace_line("", 0);
// 		rl_redisplay();
// 	}
// }

static void	sigint_handler(int sig)
{
	write(STDOUT_FILENO, "^C\n", 3);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
	g_signal = sig;
}

void	init_signals(void)
{
	signal(SIGINT, sigint_handler);
	signal(SIGQUIT, SIG_IGN);
}

// void	init_signals(t_sigmode signal_mode)
// {
// 	// struct sigaction	sa_struct;

// 	if (signal_mode == INTERACTIVE)
// 	{
// 		signal(SIGINT, sig_interactive_handler);
// 	}
// }
