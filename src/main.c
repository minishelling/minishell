#include "../include/minishell.h"


int main(int argc, char **argv, char **envp)
{
	t_shell shell;
	int status;

	(void)argv;
	if (argc > 1) 
	{
		ft_putstr_fd("Mini_shared: Error: too many arguments\n", 2);
		exit(EXIT_FAILURE);
	}
	shell.tree= NULL;
	shell.env_list = NULL;
	shell.exit_code = 0;
	if (create_env_list(&shell.env_list, envp))
	{
		ft_putstr_fd("Mini_shared: Error: Failed to initialize\n", 2);
		exit(EXIT_FAILURE);
	}
	while (1) 
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
		shell.input = readline(MINISHARED_PROMPT);
		if (g_signalcode == SIGINT)
		{
			shell.exit_code = 130;
			g_signalcode = 0;
		}
		if (!shell.input)
			break;
		if (ft_strncmp(shell.input, "", 1))
		{
			add_history(shell.input);
			status = parse(&shell);
			//printf ("status is %d\n", status);
			if (status != PARSING_OK)
				handle_parsing_err(&shell, status);
			else
			{
				init_signals(PARENT_NON_INTERACTIVE);
				shell.exit_code = traverse_tree_and_execute(&shell, shell.tree, NULL, 0);
				// printf ("last exit_code %d\n", shell.exit_code);
				clean_nicely(&shell);
			}
			//close_all_fds_in_process();
			//free(shell.input) ???
			shell.input = NULL;
		}
	}
	free_env_list(&shell.env_list);
	ft_putstr_fd("exit\n", STDOUT_FILENO);
	exit(EXIT_SUCCESS);
}
