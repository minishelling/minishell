/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   cd.c                                               :+:    :+:            */
/*                                                     +:+                    */
/*   By: lprieri <lprieri@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/07/31 12:47:36 by lprieri       #+#    #+#                 */
/*   Updated: 2024/07/31 12:47:36 by lprieri       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/builtins.h"
#include "../../../include/minishell.h"

t_ecode	builtin_cd(t_shell **shell, char *directory)
{
	char		*curpath;
	char		*cwd;
	t_cd_ecode	status;

	cwd = getcwd(NULL, PATH_MAX);
	if (!cwd)
		return (CWD_ERROR);
	if (!directory)
		return (chdir_home((*shell)->env, &cwd));
	else if (is_cdpath_set(directory))
	{
		status = chdir_cdpath(shell, directory);
		if (!status)
			return (SUCCESS);
	}
	curpath = cd_trim_curpath(directory);
	return (chdir_curpath(shell, curpath));
}

t_ecode	chdir_home(t_env *env_head, char **cwd)
{
	t_env	*home_node;
	t_ecode	exit_status;

	home_node = env_find_node(env_head, "HOME");
	if (!home_node || !home_node->value)
	{
		// PRINT ERROR
		ft_free((void **) cwd);
		return (HOME_ERROR);
	}
	else
	{
		exit_status = chdir(home_node->value);
		if (exit_status)
			return (CHDIR_ERROR);
		update_wd(env_head, *cwd);
		return (SUCCESS);
	}
}

t_ecode	update_wd(t_env	*env_head, char *cwd)
{
	t_env	*pwd_node;
	t_env	*oldpwd_node;

	pwd_node = env_find_node(env_head, "PWD");
	if (!pwd_node)
		pwd_node = env_new_populated_node("PWD", cwd);
	oldpwd_node = env_find_node(env_head, "OLDPWD");
	if (!oldpwd_node)
		oldpwd_node = env_new_populated_node("OLDPWD", getcwd(NULL, PATH_MAX));
}

bool	is_cdpath_set(directory)
{
	
}
