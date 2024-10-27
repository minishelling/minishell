#include "../include/minishell.h"

// Ctrl-C = SIGINT	-> displays a new prompt on a new line.
// Ctrl-D = EOF		-> exits the shell.
// Ctrl-\ = SIGQUIT	-> does nothing.

int	g_signalcode;

void	sigint_handler_interactive(int signal)
{
	g_signalcode = signal;
	write(STDOUT_FILENO, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

void	sigint_handler_parent_non_interactive(int signal)
{
	g_signalcode = signal;
	write(STDOUT_FILENO, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);	
}

void	sigint_handler_heredoc_parent(int signal)
{
	g_signalcode = signal;
}

void	sigint_handler_heredoc_child(int signal)
{
	if (signal == SIGINT)
		close(STDIN_FILENO);
}

void	init_signals(t_signal_mode signal_mode)
{
	struct sigaction	sigquit_struct;
	struct sigaction	sigint_struct;

	ft_bzero(&sigquit_struct, sizeof(struct sigaction));
	ft_bzero(&sigint_struct, sizeof(struct sigaction));
	sigemptyset(&sigint_struct.sa_mask);
	sigemptyset(&sigquit_struct.sa_mask);
	sigquit_struct.sa_handler = SIG_IGN;
	if (signal_mode == INTERACTIVE)
		sigint_struct.sa_handler = sigint_handler_interactive;
	else if (signal_mode == PARENT_NON_INTERACTIVE)
		sigint_struct.sa_handler = SIG_IGN;
	else if (signal_mode == CHILD_NON_INTERACTIVE)
	{
		sigint_struct.sa_handler = SIG_DFL;
		sigquit_struct.sa_handler = SIG_DFL;
	}
	else if (signal_mode == PARENT_HEREDOC)
		sigint_struct.sa_handler = sigint_handler_heredoc_parent;
	else if (signal_mode == CHILD_HEREDOC)
		sigint_struct.sa_handler = sigint_handler_heredoc_child;
	sigaction(SIGINT, &sigint_struct, NULL);
	sigaction(SIGQUIT, &sigquit_struct, NULL);
}
