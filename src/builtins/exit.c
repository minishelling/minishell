/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   exit.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: lprieri <lprieri@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/10/31 13:32:09 by lprieri       #+#    #+#                 */
/*   Updated: 2024/10/31 13:32:10 by lprieri       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static t_ecode	handle_end_of_options(t_shell *shell,
					char **cmd_args, size_t argc);
static t_ecode	handle_argc_2(t_shell *shell, char **cmd_args);
static t_ecode	handle_argc_gt_2(t_shell *shell, char **cmd_args);

/**
 * @brief Exits the minishell.
 * 
 * It handles multiple arguments, including the 'end of options' arg.
 * 
 * @param shell A pointer to the shell structure.
 * @param cmd_args An array with the command's arguments.
 * 
 * @return On success it exits the shell with SUCCESS as the default exit code,
 * or the given exit code if applicable.
 * If many arguments were passed, it checks whether the first argument
 * is a natural number, and if so it does NOT exit and returns FAILURE.
 * Otherwise it prints an error msg and exits with the code EXIT_NUM_ARG_REQ(2).
 */
t_ecode	exit_builtin(t_shell *shell, char **cmd_args)
{
	size_t	arg_count;
	int8_t	exit_code;

	arg_count = ft_str_count(cmd_args);
	exit_code = SUCCESS;
	if (arg_count == 2 && !ft_strncmp(cmd_args[1], "--", 3))
		exit_code = handle_end_of_options(shell, cmd_args, arg_count);
	else if (arg_count == 2)
		exit_code = handle_argc_2(shell, cmd_args);
	else if (arg_count > 2)
		exit_code = handle_argc_gt_2(shell, cmd_args);
	else
	{
		ft_putstr_fd("exit\n", 2);
		clean_nicely_and_exit(shell, SUCCESS);
	}
	return (exit_code);
}

/**
 * @brief Handles the case where the first argument is '--'.
 * 
 * @param shell A pointer to the shell structure.
 * @param cmd_args An array with the command's arguments.
 * @param argc The number of arguments.
 * 
 * @return It exits with SUCCESS as default or the given error code if valid.
 * If the code is invalid, it exits with the code 2 and prints an error msg.
 * If the code is valid but there are many arguments it returns FAILURE,
 * printing an error message as well.
 */
static t_ecode	handle_end_of_options(t_shell *shell,
					char **cmd_args, size_t argc)
{
	(void) shell;
	if (argc == 2)
	{
		ft_putstr_fd("exit\n", 2);
		clean_nicely_and_exit(shell, SUCCESS);
	}
	else if (argc == 3)
	{
		if (ft_is_natural(cmd_args[2]))
		{
			ft_putstr_fd("exit\n", 2);
			clean_nicely_and_exit(shell, ft_atoi(cmd_args[2]));
		}
	}
	if (!ft_is_natural(cmd_args[2]))
	{
		ft_putstr_fd("exit\n", 2);
		handle_builtin_err("exit", cmd_args[2], "numeric argument required");
		clean_nicely_and_exit(shell, EXIT_NUM_ARG_REQ);
	}
	ft_putstr_fd("exit\n", 2);
	handle_builtin_err(cmd_args[0], cmd_args[2], "too many arguments");
	return (FAILURE);
}

/**
 * @brief Handles the case where there are only 2 arguments.
 * 
 * @param shell A pointer to the shell structure.
 * @param cmd_args An array with the command's arguments.
 * 
 * @return If the argument provided is a natural number
 * it exits with that number as the exit code.
 * Else it will print an error msg and exit with the code EXIT_NUM_ARG_REQ (2).
 */
static t_ecode	handle_argc_2(t_shell *shell, char **cmd_args)
{
	(void) shell;
	if (ft_is_natural(cmd_args[1]))
	{
		ft_putstr_fd("exit\n", 2);
		clean_nicely_and_exit(shell, ft_atoi(cmd_args[1]));
	}
	ft_putstr_fd("exit\n", 2);
	handle_builtin_err("exit", cmd_args[1], "numeric argument required");
	clean_nicely_and_exit(shell, EXIT_NUM_ARG_REQ);
	return (ERROR);
}

/**
 * @brief Handles the case where there are more than 2 arguments.
 * 
 * @param shell A pointer to the shell structure.
 * @param cmd_args An array with the command's arguments.
 * 
 * @return If the first argument is not a natural number, it prints an error msg
 * and exits with the code EXIT_NUM_ARG_REQ (2).
 * Else it will print an error message and return FAILURE.
 */
static t_ecode	handle_argc_gt_2(t_shell *shell, char **cmd_args)
{
	(void) shell;
	if (!ft_is_natural(cmd_args[1]))
	{
		ft_putstr_fd("exit\n", 2);
		handle_builtin_err("exit", cmd_args[1], "numeric argument required");
		clean_nicely_and_exit(shell, EXIT_NUM_ARG_REQ);
	}
	ft_putstr_fd("exit\n", 2);
	handle_builtin_err(cmd_args[0], cmd_args[1], "too many arguments");
	return (FAILURE);
}
