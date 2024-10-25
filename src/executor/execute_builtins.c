#include "../../include/minishell.h"

static t_ecode	execute_builtin(t_shell *shell, char **cmd_args);

/**
 * @brief Handles redirections and executes a builtin function.
 * 
 * It creates a backup of STDIN and STDOUT, and then duplicates
 * the redirection file descriptors into the corresponding STD fd,
 * if there are any present.
 * Then it executes the builtin and stores the exit code in the shell struct.
 * After the execution is completed it restores the STDIN and STDOUT.
 * 
 * @param shell A pointer to the shell structure.
 * @param cmd A pointer to the command's structure.
 * @return void 
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
 * @brief Executes the builtin function.
 * 
 * Given the command's name, it checks if it is a valid builtin.
 * If it is then it calls the builtin function from a jumptable.
 * 
 * @param shell A pointer to the shell structure.
 * @param cmd_args An array with the command's arguments.
 * @return If the shell or cmd's array are null it returns a NULL ERROR.
 * If the command is not a builtin it returns PROCEED, which indicates
 * to the parent function to look and execute the command's binary file.
 * On error it returns an error code and prints an error message
 * if it is neccessary.
 */
static t_ecode	execute_builtin(t_shell *shell, char **cmd_args)
{
	t_builtin	builtin_code;

	t_ecode	(*builtins_jumptable[])(t_shell *, char **) = {echo_builtin,
		cd_builtin,	pwd_builtin, export_builtin, declare_builtin,
		unset_builtin, env_builtin, exit_builtin};
	if (!shell || !cmd_args)
		return (NULL_ERROR); //This would come from a bug, for instance freeing wrong. But it's important to have it anyways I guess.
	builtin_code = check_builtin(cmd_args[0]);
	if (builtin_code == NON_BUILTIN)
		return (PROCEED);
	else
		return (builtins_jumptable[builtin_code](shell, cmd_args));
}
