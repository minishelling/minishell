#include "../../../include/minishell.h"

/**
 * @brief Attempts to change the current working directory using paths 
 *        specified in the CDPATH environment variable.
 * 
 * This function retrieves directories listed in the `CDPATH` 
 * environment variable and checks if appending the target directory 
 * to any of these paths results in a valid and accessible directory. 
 * If a valid path is found, it changes into that directory. 
 * 
 * @param[in] env_list A double pointer to the environment list 
 *                     containing environment variables, including 
 *                     `CDPATH`.
 * @param[in] directory The directory string to be appended to 
 *                      each `CDPATH` directory.
 * @param[in] cwd The current working directory to update the 
 *                `OLDPWD` variable upon a successful change.
 * 
 * @return Returns SUCCESS if the directory change is successful 
 *         and the `OLDPWD` and `PWD` variables are updated. 
 *         If memory allocation fails, it prints an error message 
 *         and returns MALLOC_ERROR. If no valid path is found 
 *         but the operation can proceed normally, it returns 
 *         PROCEED to signal the caller to continue with default 
 *         functionality.
 * 
 * @note If `CDPATH` is not set or if the provided directory does 
 *       not yield any valid paths, the function will return 
 *       PROCEED, allowing the caller to handle the change in 
 *       directory using standard methods.
 */
t_ecode	chdir_cdpath(t_env **env_list, char *directory, char *cwd)
{
	t_env	*cdpath_node;
	char	**cdpath_values;
	ssize_t	values_count;
	t_ecode	status;
	char	new_cwd[PATH_MAX];

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
	ft_putendl_fd(getcwd(new_cwd, PATH_MAX), STDOUT_FILENO);
	return (update_oldpwd_pwd(env_list, cwd));
}

/**
 * @brief Traverses the list of CDPATH values to attempt changing 
 *        the current directory by appending the target directory 
 *        to each CDPATH value.
 * 
 * This function iterates through each directory in the `CDPATH` 
 * environment variable, appending the specified target directory 
 * to form a complete path. It attempts to change the current 
 * working directory to the first valid and accessible path found.
 * 
 * @param[in] cdpath_values An array of strings representing all 
 *                          directory paths specified in the 
 *                          `CDPATH` environment variable.
 * @param[in] values_count The number of valid CDPATH entries in 
 *                         the `cdpath_values` array.
 * @param[in] directory The target directory string that will be 
 *                      appended to each CDPATH value for the 
 *                      change attempt.
 * 
 * @return Returns SUCCESS if a valid directory change occurs. 
 *         If a memory allocation failure occurs, it returns 
 *         MALLOC_ERROR. If no valid directory change can be made, 
 *         it returns PROCEED, indicating that the caller should 
 *         handle the situation using standard methods.
 * 
 * @note This function handles cases where the CDPATH value might 
 *       be NULL and attempts to change to the directory accordingly.
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
 * @brief Handles directory change attempts when the CDPATH is NULL. 
 * 
 * In the absence of a valid CDPATH, this function attempts to 
 * change the current directory by prepending "./" to the 
 * specified target directory.
 * 
 * @param[in] directory The target directory string to which 
 *                      "./" will be prepended for the change 
 *                      attempt.
 * @param[in,out] i A pointer to the counter for CDPATH values, 
 *                  incremented to indicate a new attempt.
 * @param[in,out] null_flag A pointer to a flag that indicates 
 *                          whether the null path has already 
 *                          been attempted. If set to 1, it 
 *                          indicates that the attempt has been made.
 * 
 * @return Returns SUCCESS if the directory change succeeds. 
 *         If the null_flag is already activated or if the 
 *         directory change fails, it returns PROCEED. On memory 
 *         allocation failure, it prints an error message and 
 *         returns MALLOC_ERROR.
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
 * @brief Attempts to change the current directory by appending 
 *        the target directory to the specified CDPATH value.
 * 
 * This function constructs a new path by combining the 
 * provided CDPATH value with the target directory, 
 * and then attempts to change into that new directory.
 * 
 * @param[in] cdpath_value The current CDPATH value being processed. 
 * @param[in] directory The target directory to be appended to 
 *                      the current CDPATH value.
 * @param[in,out] i A pointer to the counter for CDPATH values, 
 *                  incremented to reflect the current index.
 * 
 * @return Returns SUCCESS if the directory change is successful. 
 *         If a memory allocation fails at any point, it prints 
 *         an error message and returns MALLOC_ERROR. If the 
 *         directory change fails for other reasons, it returns 
 *         PROCEED to indicate that further attempts may be necessary.
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
