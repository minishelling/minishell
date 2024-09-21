#include "../include/minishell.h"

// static void	test_handle(t_shell *shell)
// {
// 	env_print_node(env_find_node(shell->env_list, "PWD"));
// 	env_update_node(shell->env_list, "PWD", "NEW_VALUE", true);
// 	env_print_node(env_find_node(shell->env_list, "PWD"));
// }

int main(int argc, char **argv, char **envp)
{
	t_shell shell;
	int status;

	(void)argv;
	(void)envp;
	if (argc > 1) 
	{
		write(2, "Mini_shared: Error: too many arguments\n", 39);
		exit(EXIT_FAILURE);
	}
	shell.env_list = NULL;
	if (env_init_list(&shell.env_list, envp))
	{
		write(2, "Mini_shared: Error: Failed to initialize\n", 42);
		exit(EXIT_FAILURE);
	}
	while (1) 
	{
		shell.input = readline(MINISHARED_PROMPT);
		if (!shell.input)  // Check for EOF or error
			break;
			
		if (ft_strncmp(shell.input, "", 1))
		{
			add_history(shell.input);
			status = parse(&shell);
			//printf ("status is %d\n", status);
			if (status != PARSING_OK)
				handle_error(&shell, status, NULL);
			else
			{
				free(shell.input);
				int exit_code = execute(&shell, shell.tree, NULL, 0);
				//printf ("final exit code %d\n", exit_code);
				(void)exit_code;
				//free_token_list(shell.token);
				free_tree(shell.tree);
				shell.tree = NULL;
			}
			shell.input = NULL;
		}

	}
	exit(EXIT_SUCCESS);
}
