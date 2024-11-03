/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   echo.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: lprieri <lprieri@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/10/31 13:32:06 by lprieri       #+#    #+#                 */
/*   Updated: 2024/10/31 13:32:07 by lprieri       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static void	print_echo_chars(char **strs_arr, bool no_newline);
static int	newline_option(char *option_arg);
static int	skip_newline_options(char **cmd_args);

/**
 * @brief Prints the arguments provided to standard output, with 
 * an option to omit a trailing newline.
 * 
 * This function replicates the behavior of the `echo` command. If 
 * the first argument after the command name is the option `-n`, 
 * it omits the newline character at the end of the output.
 * 
 * @param[in] shell A pointer to the shell structure. (Unused in this function)
 * @param[in] cmd_args The command arguments as an array of strings. The first
 *                     element is the command name, followed by optional flags
 *                     and strings to print.
 * 
 * @return Returns `FAILURE` if `cmd_args` is NULL, `SUCCESS` otherwise.
 * 
 * @note If the `-n` option is specified one or more times after the 
 *       command name, the function omits the newline at the end of the output.
 */
t_ecode	echo_builtin(t_shell *shell, char **cmd_args)
{
	bool	no_newline;
	char	**strs_arr;

	(void) shell;
	if (!cmd_args)
		return (FAILURE);
	if (!cmd_args[1])
	{
		ft_putchar_fd('\n', 1);
		return (SUCCESS);
	}
	if (cmd_args[1] && newline_option(cmd_args[1]))
	{
		no_newline = true;
		strs_arr = &cmd_args[skip_newline_options(cmd_args)];
	}
	else
	{
		no_newline = false;
		strs_arr = &cmd_args[1];
	}
	print_echo_chars(strs_arr, no_newline);
	return (SUCCESS);
}

/**
 * @brief Prints all characters of the provided echo arguments,
 *        except for backslashes (`\`) and handles the optional
 *        newline character at the end of the output.
 * 
 * This function iterates through each string in the `strs_arr` array
 * and prints each character to standard output. It replaces the newline 
 * at the end of the output based on the `no_newline` flag.
 * 
 * @param[in] strs_arr An array of strings containing the characters
 *                     to be printed. Each string is treated 
 *                     separately, and spaces are added between 
 *                     strings.
 * @param[in] no_newline A boolean value indicating whether to omit 
 *                       the newline character after printing all strings.
 * 
 * @return void
 */
static void	print_echo_chars(char **strs_arr, bool no_newline)
{
	int	i;
	int	j;

	i = 0;
	while (strs_arr[i])
	{
		j = 0;
		while (strs_arr[i][j])
		{
			if (strs_arr[i][j] != '\\')
				ft_putchar_fd(strs_arr[i][j], STDOUT_FILENO);
			j++;
		}
		if (strs_arr[i + 1])
			ft_putchar_fd(' ', STDOUT_FILENO);
		i++;
	}
	if (no_newline == false)
		ft_putchar_fd('\n', STDOUT_FILENO);
}

/**
 * @brief Checks if the newline option is activated for the echo command.
 * 
 * This function examines the first argument of the echo command to determine 
 * whether it specifies the `-n` option, which indicates that a newline 
 * character should not be printed at the end of the output. 
 * 
 * @param[in] option_arg The first argument of the echo command to be checked 
 *                       for the newline option.
 * 
 * @return Returns 1 if the option argument is valid (i.e., it starts with 
 *         `-n` and has no other characters); returns 0 if the option is 
 *         not valid or if it does not conform to the expected format.
 */
static int	newline_option(char *option_arg)
{
	size_t	i;

	if (!ft_strncmp(option_arg, "-n", 2))
	{
		i = 2;
		while (option_arg[i])
		{
			if (option_arg[i] != 'n')
			{
				return (0);
			}
			i++;
		}
		return (1);
	}
	return (0);
}

/**
 * @brief Skips any newline option arguments in the echo command.
 * 
 * This function iterates through the command arguments starting from the 
 * first argument after the command name. It identifies and counts 
 * consecutive arguments that are valid newline options (i.e., those 
 * starting with `-n` followed solely by `n` characters). 
 * The function returns the index of the first non-newline option argument.
 * 
 * @param[in] cmd_args An array of command arguments provided to the 
 *                     echo command.
 * 
 * @return Returns the index of the first argument that is not a valid 
 *         newline option, or the index of the end of the arguments if 
 *         all are newline options.
 */
static int	skip_newline_options(char **cmd_args)
{
	size_t	i;
	size_t	j;

	i = 1;
	while (cmd_args[i])
	{
		if (!ft_strncmp(cmd_args[i], "-n", 2))
		{
			j = 2;
			while (cmd_args[i][j])
			{
				if (cmd_args[i][j] != 'n')
					return (i);
				j++;
			}
			i++;
		}
		else
			return (i);
	}
	return (i);
}
