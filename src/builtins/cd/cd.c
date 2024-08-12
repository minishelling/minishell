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
		return (chdir_home(shell));
	if (is_cdpath_valid(directory))
	{
		status = chdir_cdpath(shell, directory);
		if (!status)
			return (SUCCESS);
	}
	curpath = cd_trim_curpath(directory);
	status = chdir_curpath(shell, curpath);
	
}


