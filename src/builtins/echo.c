#include "../../include/minishell.h"

static void print_echo_chars(char **strs_arr, bool no_newline);
static t_ecode newline_option(char *option_arg);

/**
 * @brief Prints the arguments provided, with the option
 * to not print a trailing newline character at the end.
 * @param shell A pointer to the shell structure.
 * @param cmd_args The command arguments, containing the function name,
 * the option, and the strings to be printed.
 * @return If the cmd_args variable is NULL it returns FAILURE.
 * Returns SUCCESS otherwise.
 */
t_ecode echo_builtin(t_shell *shell, char **cmd_args)
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
		strs_arr = &cmd_args[2];
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
 * @brief Prints all the characters of the echo arguments,
 * except the newline option argument, and the '\' characters.
 * @param strs_arr An array with the strings to be printed.
 * @param no_newline The boolean variable that indicates if no newline
 * will be printed by the end of printing all the strings.
 * @return void
 */
static void print_echo_chars(char **strs_arr, bool no_newline)
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
 * @brief Checks if the newline option is activated,
 * if it is it wouldn't print a newline character at the end.
 * @param option_arg The first argument of the echo command,
 * to be checked if it is a valid newline option.
 * @return If the option argument is valid, it returns 1
 * which will trigger the no_newline boolean in the echo builtin.
 * If the option isn't valid it returns 0.
 */
static int newline_option(char *option_arg)
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
	}
	return (1);
}
