#include "../../../include/minishell.h"

t_ecode	cd_builtin(t_shell *shell, char **cmd_args)
{
	t_ecode	e_status;
	char	*directory;
	char	*cwd;

	if (!shell || !cmd_args)
		return (NULL_ERROR);
	directory = cmd_args[1];
	cwd = getcwd(NULL, PATH_MAX);
	e_status = INIT;
	if (!cwd)
		return (CWD_ERROR);
	if (!directory || !ft_strncmp(directory, "--", 3))
		e_status = chdir_home(&shell->env_list, &cwd);
	else if (!ft_strncmp(directory, "~", 2))
		e_status = chdir_tilde(&shell->env_list, &cwd);
	else if (!ft_strncmp(directory, "-", 2))
		e_status = chdir_dash(&shell->env_list, &cwd);
	else if (has_cdpath_prefix(directory))
		e_status = chdir_cdpath(&shell->env_list, directory, &cwd);
	if (e_status == INIT || e_status == PROCEED) //Maybe include if e_status != CDPATH_SUCCESS || e_status == PROCEED. Or maybe initialize e_status differently.
		e_status = chdir_default(&shell->env_list, directory, &cwd);
	if (e_status && e_status != PROCEED)
		return (e_status);
	return (SUCCESS); //Handle exit codes and errors and return success.
}

//Cd util functions
t_ecode	update_oldpwd_pwd(t_env **env_list, char **cwd)
{
	if (!*env_list)
		return (NULL_ERROR);
	update_env_node(env_list, "OLDPWD", *cwd, true);
	ft_free((void **) cwd);
	*cwd = getcwd(NULL, PATH_MAX);
	if (!*cwd)
		return (MALLOC_ERROR);
	update_env_node(env_list, "PWD", *cwd, true);
	ft_free((void **) cwd);
	return (SUCCESS);
}

t_ecode	check_curpath_access(char *curpath)
{
	t_ecode	status;

	if (!curpath)
		return (NULL_STRING);
	status = access(curpath, F_OK);
	if (status)
		return (INVALID_FILE);
	status = access(curpath, X_OK);
	if (status)
		return (ACCESS_ERROR);
	return (SUCCESS);
}
