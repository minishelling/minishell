#include "../include/minishell.h"

char	*get_error_messages(int e)
{
	char	*error_messages[] = {
		"OK",
		"Syntax error: unclosed quotes\n",
		"Syntax error: unclosed parenthesis\n",
		"Syntax error near unexpected token `|` or  `||`\n",
		"Syntax error near unexpected token `&` or `&&`\n",
		"Syntax error near unexpected token `<` or `>`\n",
		"Syntax error near unexpected token `;`\n",
		"Syntax error near unexpected token `)`\n",
		"Syntax error\n"
	};

	return (error_messages[e]);
}

// void	clean_nicely(t_shell *shell, void *param)
// {
// 		//nothing yet

// }

void	handle_error(t_shell *shell, int err_no, void *param)
{
	char	*err_msg;
	char 	*err_prompt;
	char 	*whole_msg;

	(void)shell;
	(void)param;

	err_prompt = ERR_PROMPT;
	err_msg = get_error_messages(err_no);
	whole_msg = ft_strjoin(err_prompt, err_msg);
	// if (!whole_msg)
		// I'm not sure
	ft_printf("%s", whole_msg);
	free (whole_msg);
	//clean_nicely(shell, param);
}