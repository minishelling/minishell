#include "../../include/minishell.h"

static void	handle_non_builtin(t_shell *shell, t_cmd *cmd);
static void	handle_redirs_in_child(t_cmd *cmd);
static void	run_child(t_shell *shell, t_cmd *cmd);
static void	do_parent_duties(t_shell *shell, t_cmd *cmd);

/**
 * @brief Executes a passed command.
 * It checks whether the command is an arithmetic expansion,
 * a built-in or an executable file.
 * 
 * @param shell A pointer to the shell structure.
 * @param cmd A pointer to the command node.
 * 
 * @return If it is an arithmetic expansion it prints an error message
 * and returns FAILURE.
 * If it is a built-in it calls the built-in function handler.
 * For any other type of command it calls the non-builtin handler.
 * If the command was successfully executed it returns SUCCESS,
 * and the exit code is set appropriately.
 * Otherwise it returns the corresponding exit code.
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
		ft_putstr_fd("This is an arithmetic expansion. We don't do that here.\n", 2);
		shell->exit_code = FAILURE;
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
 * @brief Creates a fork and executes the command in the child process.
 * The parent process waits for the child process to finish,
 * stores the exit code of the child and cleans up the memory and fds.
 * @param shell A pointer to the shell structure.
 * @param cmd A pointer to the current's command structure.
 * @return void
 */
static void	handle_non_builtin(t_shell *shell, t_cmd *cmd)
{
	shell->parent = fork();
	if (shell->parent == -1)
		exit(EXIT_FAILURE); //Clean nicely
	else if (!shell->parent)
		run_child(shell, cmd);
	do_parent_duties(shell, cmd);
}

/**
 * @brief It executes the current command. In the process it
 * handles signals, redirections, gets the path to the executable file,
 * and creates the environment array for the command's execution.
 * @param shell A pointer to the shell structure.
 * @param cmd A pointer to the command's structure.
 * @return void
 * In case of failure it exits with an exit code,
 * and printing an error message if it corresponds.
 */
static void	run_child(t_shell *shell, t_cmd *cmd)
{
	char	*cmd_path;
	char	**env_array;

	init_signals(CHILD_NON_INTERACTIVE);
	handle_redirs_in_child(cmd);
	if (cmd && cmd->args)
		cmd_path = get_cmd_path(shell, cmd->args[0]);
	if (!cmd_path && shell->exit_code == 126)
	{
		handle_cmd_err(cmd, strerror(EACCES));
		exit(EXIT_CMD_NOT_EXECUTABLE);
	}
	env_array = create_env_array(shell->env_list);
	execve(cmd_path, cmd->args, env_array);
	handle_cmd_err(cmd, strerror(ENOENT));
	exit(EXIT_CMD_NOT_FOUND); //Clean nicely?
}

/**
 * @brief Replaces STDIN and STDOUT with redirections if valid.
 * If there was an error opening the redirections, or duping, it exits.
 * @param cmd A pointer to the command's structure.
 * @return void
 */
static void	handle_redirs_in_child(t_cmd *cmd)
{
	if (cmd->latest_in == ERROR || cmd->latest_in == ERROR)
		exit(EXIT_FAILURE); //Clean nicely?
	if (cmd->latest_in != STDIN_FILENO)
	{
		if (dup_and_close(cmd->latest_in, STDIN_FILENO))
			exit(EXIT_FAILURE); //Clean nicely?
	}
	if (cmd->latest_out != STDOUT_FILENO)
	{
		if (dup_and_close(cmd->latest_out, STDOUT_FILENO))
			exit(EXIT_FAILURE); //Clean nicely?
	}
}

/**
 * @brief Waits for the child process to finish
 * and stores the exit status.
 * It also closes all the open file descriptors. 
 * @param shell A pointer to the shell structure.
 * @param cmd A pointer to the command's structure.
 * @return void
 */
static void	do_parent_duties(t_shell *shell, t_cmd *cmd)
{
	int	wstatus;

	waitpid(shell->parent, &wstatus, 0);
	if (WIFEXITED(wstatus) == true)
		shell->exit_code = WEXITSTATUS(wstatus);
	else if (WIFSIGNALED(wstatus) == true)
		shell->exit_code = WTERMSIG(wstatus) + EXIT_SIGNAL_CODE;
	close_all_fds_in_cmd(cmd); //Clean nicely
}
