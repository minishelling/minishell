/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfeuer <tfeuer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 13:43:26 by tfeuer            #+#    #+#             */
/*   Updated: 2024/10/31 13:43:27 by tfeuer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static void	init_shell(t_shell *shell, int argc, char **argv, char **envp)
{
	if (argc > 2 || (argc == 2 && ft_strncmp(argv[1], "-d", 3) != 0))
	{
		ft_putstr_fd("Usage:  ./minishell \
			\nor	./minishell -v for debug mode\n", 2);
		exit(EXIT_FAILURE);
	}
	g_signalcode = 0;
	shell->token = NULL;
	shell->tree = NULL;
	shell->exit_code = 0;
	shell->env_list = NULL;
	if (argc == 2 && ft_strncmp(argv[1], "-d", 3) == 0)
	{
		shell->debug_mode = ON;
		ft_putstr_fd("Debug mode enabled\n", 1);
	}
	else
		shell->debug_mode = OFF;
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
	else
		ft_free((void **) &shell->input);
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
