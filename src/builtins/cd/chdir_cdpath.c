#include "../../../include/minishell.h"

/**
 * @brief Retrieves the directories in CDPATH if set,
 * and uses them as the current working directory.
 * If the concatenation of a cdpath dir and the directory
 * is a valid and accessible path, then it changes into it.
 * 
 * @param env_list A double pointer to the environment list.
 * @param directory The directory to change into.
 * @param cwd The current working directory.
 * 
 * @return If it manages to change directories and update OLDPWD
 * and PWD successfully it returns SUCCESS. On malloc failures
 * it prints an error message and returns an error code.
 * Otherwise it returns PROCEED, which indicates to the main cd function
 * to proceed with the default functionality.
 */
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
		return (handle_perror("chdir_cdpath"), MALLOC_ERROR);
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

/**
 * @brief Traverses the list of CDPATH values,
 * appending the directory to each value and attempting
 * to change into that directory.
 * 
 * @param cdpath_values An array containing all CDPATH values.
 * @param values_count The number of members in the array.
 * @param directory The target directory.
 * 
 * @return If there's a malloc failure it returns an error code.
 * In case of a successful directory change it returns SUCCESS.
 * Otherwise it returns the code PROCEED.
 */
t_ecode	traverse_and_chdir_cdpath(char **cdpath_values,
			ssize_t values_count, char *directory)
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

/**
 * @brief In case of a CDPATH being NULL, the default functionality is
 * to append "./" to the directory and attempt to go into there.
 * 
 * @param directory The target directory
 * @param i A pointer to the counter of CDPATH values.
 * @param null_flag A flag to indicate if we have already tried this path before.
 * 
 * @return If the null flag is already activated,
 * or if changing into the directory failed, it returns PROCEED.
 * If changing into the directory succeeded it returns SUCCESS.
 * On malloc failure it prints an error message and returns MALLOC_ERROR.
 */
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
		return (handle_perror("chdir_null_cdpath"), MALLOC_ERROR);
	status = chdir(curpath);
	ft_free((void **)curpath);
	if (status)
		return (PROCEED);
	return (SUCCESS);
}

/**
 * @brief Appends the directory to the current value in CDPATH,
 * and attempts to change into that directory.
 * 
 * @param cdpath_value The current value being traversed in CDPATH.
 * @param directory The target directory
 * @param i A pointer to the counter of CDPATH values.
 * 
 * @return On success it returns SUCCESS. On malloc failure it prints
 * an error code and returns MALLOC_ERROR. Otherwise it returns PROCEED.
 */
t_ecode	chdir_cdpath_value(char *cdpath_value, char *directory, ssize_t *i)
{
	char	*curpath;
	t_ecode	status;

	*i += 1;
	curpath = ft_strdup(cdpath_value);
	if (!curpath)
		return (handle_perror("chdir_cdpath_value"), MALLOC_ERROR);
	if (append_suffix(&curpath, "/", false))
		return (handle_perror("chdir_cdpath_value"), MALLOC_ERROR);
	curpath = ft_strjoin_fs1(&curpath, directory);
	if (!curpath)
		return (handle_perror("chdir_cdpath_value"), MALLOC_ERROR);
	status = chdir(curpath);
	ft_free((void **) &curpath);
	if (status)
		return (PROCEED);
	return (SUCCESS);
}
