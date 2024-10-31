#include "../../../include/minishell.h"

/**
 * @brief Change directory built-in command.
 * 
 * This function attempts to change the current working directory to 
 * the target directory specified in the command arguments. If the 
 * `CDPATH` environment variable is set, it will take priority over 
 * the current working directory (cwd). The function also supports 
 * changing to home (`~`) and previous (`-`) directories, as well as 
 * absolute paths.
 * 
 * @param[in] shell A pointer to the shell structure.
 * @param[in] cmd_args An array of command arguments, where the 
 *                     first argument is expected to be the command 
 *                     itself and the second is the target directory.
 * 
 * @return On success, returns SUCCESS after updating `OLDPWD` and 
 *         `PWD` environment variables. On failure, returns an error 
 *         code and, if necessary, prints an appropriate error message.
 * 
 * @note The function handles cases such as:
 *       - No arguments provided
 *       - Too many arguments
 *       - Changing to home or previous directories
 *       - Resolving paths via `CDPATH`.
 */
t_ecode	cd_builtin(t_shell *shell, char **cmd_args)
{
	t_ecode	e_status;
	char	*directory;
	char	cwd[PATH_MAX];

	if (!shell || !cmd_args)
		return (NULL_ERROR);
	directory = cmd_args[1];
	getcwd(cwd, PATH_MAX);
	e_status = INIT;
	if (ft_str_count(cmd_args) > 2)
		return (handle_builtin_err("cd", NULL, "too many arguments"), FAILURE);
	if (!directory || !ft_strncmp(directory, "--", 3))
		e_status = chdir_home(&shell->env_list, cwd);
	else if (!ft_strncmp(directory, "~", 2))
		e_status = chdir_tilde(&shell->env_list, cwd);
	else if (!ft_strncmp(directory, "-", 2))
		e_status = chdir_dash(&shell->env_list, cwd);
	else if (has_cdpath_prefix(directory))
		e_status = chdir_cdpath(&shell->env_list, directory, cwd);
	if (e_status == INIT || e_status == PROCEED)
		e_status = chdir_default(&shell->env_list, directory, cwd);
	if (e_status)
		return (e_status);
	return (SUCCESS);
}
