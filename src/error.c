#include "../include/minishell.h"

const char	*get_error_messages(int e)
{
	const char	*error_messages[] = {
		"Error\nSyntax error\n",

	};

	return (error_messages[e]);
}

// void	exit_nicely(t_shell *shell, void *param)
// {
// 		//nothing for now

// }

// void	handle_error(t_shell *shell, int err_no, void *param)
// {
// 	const char	*err_msg;

// 	err_msg = get_error_messages(err_no);
// 	ft_printf("%s\n", err_msg);
// 	exit_nicely(shell, param);
// 	exit(EXIT_FAILURE);
// }