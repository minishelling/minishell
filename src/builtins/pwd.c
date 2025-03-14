/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   pwd.c                                              :+:    :+:            */
/*                                                     +:+                    */
/*   By: lprieri <lprieri@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/10/31 13:32:12 by lprieri       #+#    #+#                 */
/*   Updated: 2024/10/31 13:32:13 by lprieri       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/**
 * @brief Prints the current working directory to standard output.
 * 
 * @param shell A pointer to the shell structure.
 * @param cmd_args The array of arguments of the given command.
 * 
 * @return 0 on success. 1 on failure.
 */
t_ecode	pwd_builtin(t_shell *shell, char **cmd_args)
{
	char	*pwd;
	size_t	option_len;

	(void) shell;
	pwd = getcwd(NULL, PATH_MAX);
	if (!pwd)
		return (FAILURE);
	if (cmd_args && cmd_args[1] && cmd_args[1][0] == '-')
	{
		option_len = ft_strlen(cmd_args[1]);
		if (!ft_strncmp(cmd_args[1], "-", option_len)
			|| !ft_strncmp(cmd_args[1], "--", option_len))
				;
		else
		{
			handle_builtin_err(cmd_args[0], cmd_args[1], "invalid option");
			handle_builtin_err(cmd_args[0], "usage", "pwd");
			return (FAILURE);
		}
	}
	ft_putstr_fd(pwd, STDOUT_FILENO);
	ft_putchar_fd('\n', STDOUT_FILENO);
	ft_free((void **) &pwd);
	return (SUCCESS);
}
