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
	else if (is_dir_prefix_valid(directory))
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
		env_update_node();
		ft_free((void **) cwd);
		return (SUCCESS);
	}
}



t_ecode	update_pwd(t_env *pwd_node)
{
	if (!pwd_node)
	{
		pwd_node = env_new_populated_node("PWD", getcwd(NULL, PATH_MAX));
		if (!pwd_node)
			return (MALLOC_ERROR);
	}
	else
	{
		if (env_update_value(pwd_node, getcwd(NULL, PATH_MAX)))
			return (MALLOC_ERROR);
	}
	return (SUCCESS);
}

t_ecode	update_oldpwd(t_env	*oldpwd_node, char *cwd)
{
	if (!pwd_node)
	{
		pwd_node = env_new_populated_node("PWD", getcwd(NULL, PATH_MAX));
		if (!pwd_node)
			return (MALLOC_ERROR);
	}
	else
	{
		if (env_update_value(pwd_node, getcwd(NULL, PATH_MAX)))
			return (MALLOC_ERROR);
	}
	return (SUCCESS);
}

/**
* @brief Checks if the directory is valid for running the function chdir_cdpath.
*
* @param directory
* @return False if the directory begins with dot, double dot, or slash. True otherwise.
*/
bool	is_dir_prefix_valid(char *directory)
{
	if (!directory || directory[0] == '/' || directory[0] == '.'
		|| (directory[0] == '.' && directory[1] == '.'))
		return (false);
	return (true);
}
