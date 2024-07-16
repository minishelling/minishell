#include "../../include/minishell.h"

void	complexer(t_shell *shell)
{
	shell->token = lexer(shell->input);
	// if (mini->token == NULL)
	// 	...
	list_token_print(shell->token);
	shell->syntax = syntax(shell->token, shell->env_list);
	if (shell->syntax)
	{
		// mini_error_test(syntax_error, 258, mini->syntax->str);
		// list_token_free_list(mini->token, list_token_free_node_str);
		return ;
	}
	shell->token = expander(shell->token, shell->env_list);
	list_token_print(shell->token);
	// if (appender(shell) == false)
	// 	return (list_token_free_list(shell->token, list_token_free_node_str), \
	// 			mini_error_test(error_print, 1, "expander: unable to expand"));
	// mini->cmd_list = parser(mini);
	// list_cmd_print(mini->cmd_list);
	// if (mini->cmd_list == NULL)
	// 	mini_error_test(error_print, 1, "parser: unable to parse");
	// mini->token = NULL;


}

/* IF mini->syntax contains a token the syntax has encountered a error at this
 * token.
 * returning at this point still need to be implemented
bash-3.2$ ./a.out echo "$VAR" $VAR a"$VAR""b"

arg[0]:	|./a.out|
arg[1]:	|echo|
arg[2]:	||
arg[3]:	|ab|

	list_token_print(mini->token);
*/
