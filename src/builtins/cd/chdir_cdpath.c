#include "../../../include/minishell.h"

t_ecode	chdir_cdpath(t_env **env_list, char *directory, char *cwd)
{
	t_env	*cdpath_node;
	char	**cdpath_values;
	ssize_t	values_count;
	t_ecode	status;

	if (!*env_list)
		return (NULL_ENV);
	cdpath_node = find_env_node(*env_list, "CDPATH");
	if (!cdpath_node || !cdpath_node->value)
		return (PROCEED);
	cdpath_values = ft_split(cdpath_node->value, ':');
	if (!cdpath_values)
		return (MALLOC_ERROR);
	values_count = count_values_from_env_node(*env_list, "CDPATH");
	if (values_count == -1)
		return (PROCEED);
	status = traverse_and_chdir_cdpath(cdpath_values, values_count, directory);
	ft_free_2d((void ***) &cdpath_values);
	if (status == MALLOC_ERROR)
		return (MALLOC_ERROR);
	else if (status)
		return (PROCEED);
	return (update_oldpwd_pwd(env_list, cwd));
}

t_ecode	traverse_and_chdir_cdpath(char **cdpath_values, ssize_t values_count, char *directory)
{
	ssize_t	i;
	int8_t	null_flag;
	t_ecode	status;

	i = 0;
	null_flag = 0;
	while (i < values_count)
	{
		if (!cdpath_values[i])
		{
			status = chdir_null_cdpath(directory, &i, &null_flag);
			if (status == MALLOC_ERROR)
				return (MALLOC_ERROR);
			else if (status)
				continue ;
			return (SUCCESS);
		}
		status = chdir_cdpath_value(cdpath_values[i], directory, &i);
		if (status == MALLOC_ERROR)
			return (MALLOC_ERROR);
		else if (status)
			continue ;
		return (SUCCESS);
	}
	return (PROCEED);
}

t_ecode	chdir_null_cdpath(char *directory, ssize_t *i, int8_t *null_flag)
{
	char	*curpath;
	t_ecode	status;

	*i += 1;
	if (*null_flag == 1)
		return (PROCEED);
	*null_flag = 1;
	curpath = ft_strjoin("./", directory);
	if (!curpath)
		return (MALLOC_ERROR);
	status = chdir(curpath);
	ft_free((void **)curpath);
	if (status)
		return (CHDIR_ERROR);
	return (SUCCESS);
}

t_ecode chdir_cdpath_value(char *cdpath_value, char *directory, ssize_t *i)
{
	char	*curpath;
	t_ecode	status;

	*i += 1;
	curpath = ft_strdup(cdpath_value);
	if (!curpath)
		return (MALLOC_ERROR);
	if (append_suffix(&curpath, "/", false))
		return (MALLOC_ERROR);
	curpath = ft_strjoin_fs1(&curpath, directory);
	if (!curpath)
		return (MALLOC_ERROR);
	status = chdir(curpath);
	ft_free((void **) &curpath);
	if (status)
		return (PROCEED);
	return (SUCCESS);
}
