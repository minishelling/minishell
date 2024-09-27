#include "../include/minishell.h"

// Ctrl-C = SIGINT	-> displays a new prompt on a new line.
// Ctrl-D = EOF		-> exits the shell.
// Ctrl-\ = SIGQUIT	-> does nothing.

int	g_exitcode = 0;

static void	sig_interactive_handler(int signal);
static void	reset_prompt(void);
static void sig_non_interactive_handler(int signal);
static void print_newline(void);
static void sig_heredoc_handler(int signal);
// static void print_heredoc_newline(void);

void	set_signals(t_signal_mode signal_mode)
{
	struct sigaction	sa_sigint;
	struct sigaction	sa_sigquit;

	sigemptyset(&sa_sigint.sa_mask);
	sigemptyset(&sa_sigquit.sa_mask);
	if (signal_mode == INTERACTIVE)
	{
		sa_sigint.sa_handler = &sig_interactive_handler;
		sa_sigquit.sa_handler = SIG_IGN;
	}
	else if (signal_mode == NON_INTERACTIVE)
	{
		sa_sigint.sa_handler = &sig_non_interactive_handler;
		sa_sigquit.sa_handler = &sig_non_interactive_handler;
	}
	else if (signal_mode == HEREDOC)
	{

		sa_sigint.sa_handler = &sig_heredoc_handler;
		sa_sigquit.sa_handler = SIG_IGN;
	}
	sigaction(SIGINT, &sa_sigint, NULL);
	sigaction(SIGQUIT, &sa_sigquit, NULL);
}

static void	sig_interactive_handler(int signal)
{
	if (signal == SIGINT)
		g_exitcode = EXIT_SIGINT;
	reset_prompt();
}

static void	reset_prompt(void)
{
	write(STDOUT_FILENO, "\n", 1);
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
}

static void sig_non_interactive_handler(int signal)
{
	if (signal == SIGINT)
		g_exitcode = EXIT_SIGINT;
	else if (signal == SIGQUIT)
		g_exitcode = EXIT_SIGQUIT;
	print_newline();
}

static void print_newline(void)
{
	write(STDOUT_FILENO, "\n", 1);
	rl_replace_line("", 0);
	rl_on_new_line();
}

static void sig_heredoc_handler(int signal)
{
	if (signal == SIGINT)
		g_exitcode = EXIT_SIGINT;
	// exit (g_exitcode);
	// print_heredoc_newline();
}

void print_heredoc_newline(void)
{
	write(STDOUT_FILENO, "\n", 1);
	rl_replace_line("", 0);
	rl_on_new_line();
	// rl_redisplay();
}
