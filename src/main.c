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
	shell.env_list = NULL;
	shell.exit_code = 0;
	if (init_env_list(&shell.env_list, envp))
	{
		ft_putstr_fd("Mini_shared: Error: Failed to initialize\n", 2);
		exit(EXIT_FAILURE);
	}
	while (1) 
	{
		g_signalcode = 0;
		init_signals(INTERACTIVE);
		shell.input = readline(MINISHARED_PROMPT);
		if (g_signalcode == SIGINT)
		{
			shell.exit_code = 130;
			g_signalcode = 0;
		}
		if (!shell.input)  // Check for EOF or error
			break;
		if (ft_strncmp(shell.input, "", 1))
		{
			add_history(shell.input);
			status = parse(&shell);
			//printf ("status is %d\n", status);
			if (status != PARSING_OK)
				handle_parsing_err(&shell, status, NULL);
			else
			{
				init_signals(PARENT_NON_INTERACTIVE);
				free(shell.input);
				shell.exit_code = pre_execute(&shell, shell.tree, NULL, 0);
				free_tree(shell.tree);
				shell.tree = NULL;
			}
			//close_all_fds_in_process();
			shell.input = NULL;
		}
	}
	exit(EXIT_SUCCESS);
}
