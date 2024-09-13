#include "../include/minishell.h"

// static void	test_handle(t_shell *shell)
// {
// 	env_print_node(env_find_node(shell->env_list, "PWD"));
// 	env_update_node(shell->env_list, "PWD", "NEW_VALUE", true);
// 	env_print_node(env_find_node(shell->env_list, "PWD"));
// }

// Main function for testing purposes.
// int	main(int argc, char **argv, char **envp)
// {
// 	t_shell shell;

// 	(void) argv;
// 	if (argc > 1)
// 	{
// 		write(2, "Mini_shared: Error: too many arguments\n", 39);
// 		exit(EXIT_FAILURE);
// 	}
// 	shell.env_list = NULL;
// 	if (env_init_list(&shell.env_list, envp))
// 	{
// 		write(2, "Mini_shared: Error: Failed to initialize\n", 42);
// 		exit(EXIT_FAILURE);
// 	}
// 	while (1)
// 	{
// 		shell.input = readline(MINISHARED_PROMPT);
// 		if (!shell.input)
// 			break ;
// 		if (ft_strncmp(shell.input, "", 1))
// 		{
// 			add_history(shell.input);

// 			//TEST HERE
// 			test_handle(&shell);






// 			shell.input = NULL;
// 		}
// 	}
// 	exit(EXIT_SUCCESS);
// }


int main(int argc, char **argv, char **envp)
{
	t_shell shell;
	t_token *temp;
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
			ping_lisandro(&shell, shell.tree, NULL);
			//printf ("status is %d\n", status);
			if (status != PARSING_OK)
				handle_error(&shell, status, NULL);
			else
			{
				while (shell.token) 
				{
					temp = shell.token;
					shell.token = shell.token->next;
					if(temp->str)
						free(temp->str);
					if (temp)
						free(temp);
				}
				free(shell.input);
			}
			shell.input = NULL;
		}
	}
	exit(EXIT_SUCCESS);
}
