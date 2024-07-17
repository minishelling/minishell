#include "../include/minishell.h"


int main(int argc, char **argv, char **envp)
{
    t_shell shell;
    t_token *temp;
    t_env	*env_list;


    (void)argv;
    (void)envp;
    // env_list = NULL;
	// env_init_list(&env_list, envp);
	// env_print_list(env_list);
	// env_free_list(&env_list);
    if (argc > 1) 
	{
        write(2, "Mini_shared: Error: too many arguments\n", 39);
        exit(EXIT_FAILURE);
    }
    //if (init_shell(envp, &shell)) 
    if (env_init_list(&env_list, envp)) 
	{
        write(2, "Mini_shared: Error: Failed to initialize\n", 42);
        exit(EXIT_FAILURE);
    }
    while (1) 
	{
        shell.input = readline(MINISHARED_PROMPT);
        if (!shell.input)
            break;
        if (strncmp(shell.input, "", 1)) //libft
		{
           complexer(&shell);
           list_token_print(shell.token);

            while (shell.token) 
			{
                temp = shell.token;
                shell.token = shell.token->next;
                free(temp->str);
                free(temp);
            }
            free(shell.input);
            shell.input = NULL;
        }
    }
    exit(EXIT_SUCCESS);
}
