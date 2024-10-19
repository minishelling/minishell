#include "../../../include/minishell.h"

/**
 * @brief Change directory built-in.
 * It will attempt to change into the target directory.
 * If CDPATH is set that would take priority over the cwd.
 * It also supports absolute paths.
 * 
 * @param shell A pointer to the shell structure.
 * @param cmd_args An array of the command's arguments.
 * 
 * @return If the change of directories and updating
 * OLDPWD and PWD is successful it returns SUCCESS.
 * Otherwise it returns an error code, and if neccessary
 * prints an error message.
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
