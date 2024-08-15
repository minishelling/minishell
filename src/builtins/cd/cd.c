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
		else if (status == MALLOC_ERROR)
			return (FAILURE); //And free everything... Rather 'exit' than return.
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
		env_update_node(env_head, "PWD", getcwd(NULL, PATH_MAX), true);
		env_update_node(env_head, "OLDPWD", *cwd, true);
		ft_free((void **) cwd);
		return (SUCCESS);
	}
}

t_ecode	chdir_cdpath(t_shell **shell, char *directory)
{
	t_env	*cdpath_node;
	char	*curpath;
	char	**values;
	int		i;
	int		null_flag;

	cdpath_node = env_find_node((*shell)->env, "CDPATH");
	if (!cdpath_node || !cdpath_node->value)
		return (CDPATH_NULL);
	values = ft_split(cdpath_node->value, ':');
	if (!values)
		return (MALLOC_ERROR);
	i = 0;
	null_flag = 0;
	while (values[i])
	{
		curpath = ft_strdup(values[i]);
		if (!curpath && !null_flag)
		{
			null_flag = 1;
			curpath = ft_strjoin("./", directory);
			if (curpath_check_access(curpath))
			{
				ft_free((void **) &curpath);
				ft_free_2d((void ***) &values);
				i++;
				continue ;
			}
			if (chdir(curpath))
			{
				ft_free((void **) &curpath);
				i++;
				continue ;
			}
			else
			{
				ft_free((void **) &curpath);
				ft_free_2d((void ***) &values);
				return (SUCCESS);
			}
		}
		else if (!curpath && null_flag)
		{
			i++;
			continue ;
		}
		if (append_suffix(&curpath, "/", false) == MALLOC_ERROR)
			return (MALLOC_ERROR);
		curpath = ft_strjoin_fs1(&curpath, directory);
		if (!curpath)
			return (MALLOC_ERROR);
		if (curpath_check_access(curpath))
		{
			ft_free((void **) &curpath);
			i++;
			continue ;
		}
		if (chdir(curpath))
		{
			ft_free((void **) &curpath);
			i++;
			continue ;
		}
		else
		{
			ft_free((void **) &curpath);
			ft_free((void ***) &values);
			return (SUCCESS);
		}
	}
	return (PROCEED);
}

// TEST This function. Idea: make test directory equipped with libft.
t_ecode	append_suffix(char **str, char *suffix, bool duplicate)
{
	size_t	suffix_len;

	if (!*str || !suffix)
		return (NULL_STRING);
	suffix_len = ft_strlen(suffix);
	if (ft_strncmp(str[suffix_len], suffix, suffix_len) && !duplicate);
		return (SUCCESS);
	else
	{
		*str = ft_strjoin_fs1(str, suffix);
		if (!*str)
			return (MALLOC_ERROR);
		else return (SUCCESS);
	}
}


