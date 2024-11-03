/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   executor.c                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: lprieri <lprieri@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/10/31 13:32:30 by lprieri       #+#    #+#                 */
/*   Updated: 2024/10/31 13:32:31 by lprieri       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int			executor(t_shell *shell, t_cmd *cmd);
static void	handle_non_builtin(t_shell *shell, t_cmd *cmd);
static void	handle_redirs_in_child(t_shell *shell, t_cmd *cmd);
static void	run_child(t_shell *shell, t_cmd *cmd);
static void	do_parent_duties(t_shell *shell, t_cmd *cmd);

/**
 * @brief Executes a given command by determining its type.
 * 
 * The function checks if the command is an arithmetic expansion,
 * a built-in command, or an executable file. It performs the 
 * following steps:
 * - If the command is an arithmetic expansion (indicated by 
 *   the syntax `((`), an error message is printed and it 
 *   returns a syntax failure code.
 * - If the command is a built-in, the corresponding built-in 
 *   handler is invoked to execute it.
 * - If the command is neither, the non-built-in handler is 
 *   invoked to execute the command as an external program.
 * 
 * On successful execution of the command, it returns 
 * `SUCCESS`, updating the shell's exit code appropriately. 
 * If any issues arise during execution, it returns the 
 * corresponding exit code.
 * 
 * @param shell A pointer to the shell structure containing the
 *              state and environment for command execution.
 * @param cmd A pointer to the command node containing command 
 *            arguments and type information.
 * 
 * @return Returns `SUCCESS` if the command was successfully executed.
 * 		   If the command is an arithmetic expansion, it returns
 * 		   `SYNTAX_FAILURE`. For other cases, it returns 
 * 		   the corresponding exit code set during execution.
 */
int	executor(t_shell *shell, t_cmd *cmd)
{
	t_builtin	is_builtin;

	if (!cmd->args[0])
	{
		shell->exit_code = SUCCESS;
		return (shell->exit_code);
	}
	if (!ft_strncmp(cmd->args[0], "((", 3))
	{
		ft_putstr_fd(\
		"This is an arithmetic expansion. We don't do that here.\n", 2);
		shell->exit_code = SYNTAX_FAILURE;
		return (shell->exit_code);
	}
	is_builtin = check_builtin(cmd->args[0]);
	if (is_builtin == NON_BUILTIN)
		handle_non_builtin(shell, cmd);
	else
		handle_builtin(shell, cmd);
	return (shell->exit_code);
}

/**
 * @brief Forks the current process to execute a non-built-in command 
 * in a child process.
 * 
 * This function forks the shell process to create a child process
 * for executing commands that are not built-in. In the child process, 
 * `run_child` is called to set up and execute the command.
 * If the command cannot be found or executed, appropriate error messages 
 * are printed, and the child exits with the corresponding failure code.
 * 
 * In the parent process, it waits for the child to complete, captures 
 * the exit status, and closes open file descriptors associated with the command.
 * 
 * @param[in] shell Pointer to the shell structure.
 * @param[in] cmd Pointer to the command structure containing the 
 *                command's arguments and related information.
 * 
 * @return void
 * 
 * @note If the `fork()` call fails, the function calls 
 * `clean_nicely_and_exit()` to handle cleanup and exits the program.
 * 
 * @note In the child process, if the command is identified as a path
 *       and cannot be found or accessed, the function distinguishes 
 *       between “file not found” and “permission denied” errors and 
 *       sets the exit code accordingly.
 */
static void	handle_non_builtin(t_shell *shell, t_cmd *cmd)
{
	shell->parent = fork();
	if (shell->parent == -1)
		clean_nicely_and_exit(shell, EXIT_FAILURE);
	else if (!shell->parent)
	{
		run_child(shell, cmd);
		if (access(cmd->args[0], F_OK) != 0 && ft_ispath(cmd->args[0]))
			handle_cmd_err(shell, cmd, strerror(ENOENT));
		else if (access(cmd->args[0], F_OK) == 0 && ft_ispath(cmd->args[0]))
			handle_cmd_err(shell, cmd, strerror(errno));
		else
			handle_cmd_err(shell, cmd, "command not found");
		clean_nicely_and_exit(shell, EXIT_CMD_NOT_FOUND);
	}
	do_parent_duties(shell, cmd);
}

/**
 * @brief Executes the specified command in a child process.
 * 
 * Initializes signal handling, performs I/O redirection,
 * retrieves the path of the executable file, and sets up the environment
 * variables needed for the command's execution.
 * 
 * If the executable path cannot be determined or accessed, 
 * it handles this error by printing an error message in the calling 
 * function and exiting with an appropriate error code.
 * 
 * @param[in] shell Pointer to the shell structure.
 * @param[in] cmd Pointer to the command structure containing
 *                the command's arguments and metadata.
 * 
 * @return void
 * 
 * @note If an error occurs during execution setup, the function 
 *       exits the child process with the appropriate exit code.
 */
static void	run_child(t_shell *shell, t_cmd *cmd)
{
	char	*cmd_path;
	char	**env_array;

	cmd_path = NULL;
	init_signals(CHILD_NON_INTERACTIVE);
	handle_redirs_in_child(shell, cmd);
	if (cmd && cmd->args)
		cmd_path = get_cmd_path(shell, cmd->args[0]);
	if (!cmd_path && shell->exit_code != 0)
	{
		handle_cmd_err(shell, cmd, strerror(errno));
		clean_nicely_and_exit(shell, shell->exit_code);
	}
	env_array = create_env_array(shell->env_list);
	if (cmd_path)
		execve(cmd_path, cmd->args, env_array);
	if (ft_strncmp(cmd->args[0], cmd_path, ft_strlen(cmd_path)))
		ft_free((void **) &cmd_path);
	ft_free_2d((void ***) &env_array);
}

/**
 * @brief Replaces the standard input (STDIN) and standard output (STDOUT)
 * with the specified redirections if they are valid.
 * 
 * This function checks the file descriptors for the latest input
 * and output redirections. If they are valid and not equal to their 
 * respective standard file descriptors, it duplicates the file descriptors 
 * for input and output. If there is an error in opening the redirections 
 * or in duplicating the file descriptors, the function exits the shell 
 * with an appropriate failure code.
 * 
 * @param shell A pointer to the shell structure.
 * @param cmd A pointer to the command structure that contains 
 *            the latest input and output file descriptors.
 * 
 * @return void
 * 
 * @note If either `latest_in` or `latest_out` is set to ERROR, 
 *       the function will exit with `EXIT_FAILURE`.
 */
static void	handle_redirs_in_child(t_shell *shell, t_cmd *cmd)
{
	if (cmd->latest_in == ERROR || cmd->latest_in == ERROR)
		clean_nicely_and_exit(shell, EXIT_FAILURE);
	if (cmd->latest_in != STDIN_FILENO)
	{
		if (dup_and_close(cmd->latest_in, STDIN_FILENO))
			clean_nicely_and_exit(shell, EXIT_FAILURE);
	}
	if (cmd->latest_out != STDOUT_FILENO)
	{
		if (dup_and_close(cmd->latest_out, STDOUT_FILENO))
			clean_nicely_and_exit(shell, EXIT_FAILURE);
	}
}

/**
 * @brief Waits for the child process to finish and 
 *        stores the exit status while closing all open file descriptors.
 * 
 * This function waits for the child process from execution to terminate
 * and retrieves its exit status. Depending on how the child process exited
 * (normally or via a signal), it updates the shell's exit code accordingly.
 * Additionally, it ensures that all file descriptors associated with
 * the command are closed after the child's execution.
 * 
 * @param shell A pointer to the shell structure.
 * @param cmd A pointer to the command structure containing the 
 *            file descriptors and other command-related data.
 * 
 * @return void
 * 
 * @note The exit code is set based on whether the child process exited 
 *       normally or was terminated by a signal.
 */
static void	do_parent_duties(t_shell *shell, t_cmd *cmd)
{
	int	wstatus;

	wstatus = 0;
	waitpid(shell->parent, &wstatus, 0);
	if (WIFEXITED(wstatus) == true)
		shell->exit_code = WEXITSTATUS(wstatus);
	else if (WIFSIGNALED(wstatus) == true)
	{
		shell->exit_code = WTERMSIG(wstatus) + EXIT_SIGNAL_CODE;
		g_signalcode = WTERMSIG(wstatus);
	}
	close_all_fds_in_cmd(cmd);
}
