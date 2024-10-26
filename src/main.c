#include "../include/minishell.h"

static void	init_shell(t_shell *shell, int argc, char **argv, char **envp)
{
	(void)argv;
	if (argc > 1)
	{
		ft_putstr_fd("Mini_shared: Error: too many arguments\n", 2);
		exit(EXIT_FAILURE);
	}
	g_signalcode = 0;
	shell->tree = NULL;
	shell->exit_code = 0;
	shell->env_list = NULL;
	if (create_env_list(&shell->env_list, envp))
	{
		ft_putstr_fd("Mini_shared: Error: Failed to initialize\n", 2);
		exit(EXIT_FAILURE);
	}
}

static void	init_signals_in_loop(void)
{
	if (g_signalcode)
	{
		write(STDOUT_FILENO, "\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_on_new_line();
	}
	g_signalcode = 0;
	init_signals(INTERACTIVE);
}

static void	handle_minishell_input(t_shell *shell)
{
	int	status;

	status = 0;
	if (ft_strncmp(shell->input, "", 1))
	{
		add_history(shell->input);
		status = parse(shell);
		if (status != PARSING_OK)
			handle_parsing_err(shell, status);
		else
		{
			init_signals(PARENT_NON_INTERACTIVE);
			shell->exit_code = traverse_tree_and_execute \
			(shell, shell->tree, NULL, 0);
			clean_nicely(shell);
		}
	}
}

static void	run_minishell_loop(t_shell *shell)
{
	while (1)
	{
		init_signals_in_loop();
		shell->input = readline(MINISHARED_PROMPT);
		if (!shell->input)
			break ;
		if (g_signalcode == SIGINT)
		{
			shell->exit_code = 130;
			g_signalcode = 0;
		}
		handle_minishell_input(shell);
	}
	free_env_list(&shell->env_list);
}

int	main(int argc, char **argv, char **envp)
{
	t_shell	shell;

	init_shell(&shell, argc, argv, envp);
	run_minishell_loop(&shell);
	ft_putstr_fd("exit\n", STDOUT_FILENO);
	exit(EXIT_SUCCESS);
}
