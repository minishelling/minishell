#include "../../include/minishell.h"

static t_ecode	execute_builtin(t_shell *shell, char **cmd_args);

/**
 * @brief Handles I/O redirections and executes a builtin command.
 * 
 * This function manages temporary redirections for standard input (STDIN) and 
 * output (STDOUT) to execute a builtin command with specified I/O redirections.
 * The function performs the following steps:
 * - Backs up the current STDIN and STDOUT file descriptors.
 * - Redirects STDIN and/or STDOUT based on redirection specifications in the 
 *   cmd structure.
 * - Executes the builtin command via `execute_builtin` and stores the exit 
 *   code in the shell structure.
 * - Restores the original STDIN and STDOUT file descriptors to their previous 
 *   states.
 * 
 * @param[in,out] shell A pointer to the shell structure, where the function 
 *                      will store the exit code after executing the builtin.
 * @param[in] cmd A pointer to the command structure containing arguments and 
 *                redirection file descriptors for STDIN and STDOUT.
 * 
 * @return void This function does not return a value, but it may terminate 
 *              the program if critical errors occur.
 *
 * @note This function assumes that `cmd->latest_in` and `cmd->latest_out` 
 *       specify valid redirection targets, if any are present. In case of 
 *       failure during redirection or restoration, the function will clean up 
 *       and exit the program.
 */
void	handle_builtin(t_shell *shell, t_cmd *cmd)
{
	int	std_backup[2];

	if (create_std_backup(std_backup))
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
	shell->exit_code = execute_builtin(shell, cmd->args);
	if (dup_and_close(std_backup[STDIN_FILENO], STDIN_FILENO))
		clean_nicely_and_exit(shell, EXIT_FAILURE);
	if (dup_and_close(std_backup[STDOUT_FILENO], STDOUT_FILENO))
		clean_nicely_and_exit(shell, EXIT_FAILURE);
}

/**
 * @brief Executes a shell builtin command if recognized.
 *
 * Given a command name, this function checks if the command is a valid 
 * builtin using a lookup function. If it is a recognized builtin, the 
 * function executes it via a jumptable of function pointers. If not, 
 * the function signals the caller to search for and execute an external 
 * command.
 * 
 * @param[in] shell A pointer to the shell structure.
 * @param[in] cmd_args An array of strings representing the command name 
 *                     and its arguments.
 * @return t_ecode The function returns one of the following:
 * - `NULL_ERROR` if either `shell` or `cmd_args` is NULL.
 * - `PROCEED` if the command is not a recognized builtin, indicating 
 *   that the caller should attempt to execute it as an external binary.
 * - An error code if there was an error executing the builtin, and may 
 *   print an error message if appropriate.
 */
static t_ecode	execute_builtin(t_shell *shell, char **cmd_args)
{
	t_builtin	builtin_code;

	t_ecode (*builtins_jumptable[])(t_shell *, char **) = {echo_builtin,
	cd_builtin, pwd_builtin, export_builtin, declare_builtin,
	unset_builtin, env_builtin, exit_builtin};
	if (!shell || !cmd_args)
		return (NULL_ERROR);
	builtin_code = check_builtin(cmd_args[0]);
	if (builtin_code == NON_BUILTIN)
		return (PROCEED);
	else
		return (builtins_jumptable[builtin_code](shell, cmd_args));
}
