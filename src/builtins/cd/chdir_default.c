#include "../../../include/minishell.h"

static t_ecode	handle_tilde_absolute_path(t_env **env_list, char *directory);
static char		*get_tilde_absolute_path(t_env *env_list, char *directory);
static t_ecode	handle_absolute_path(char *directory);
static t_ecode	handle_relative_path(char *directory, char *cwd);

/**
 * @brief Attempts to change the current directory based on the provided directory path.
 * 
 * This function handles three types of paths: 
 * - Absolute paths (starting with '/')
 * - Absolute paths beginning with a tilde ('~')
 * - Relative paths, which are processed if CDPATH is not valid.
 * 
 * @param env_list A double pointer to the head of the environment list, 
 *                 allowing for updates to environment variables.
 * @param directory The target directory to change into.
 * @param cwd The current working directory, used for resolving relative paths.
 * 
 * @return Returns SUCCESS if the directory change was successful 
 *         and updates the OLDPWD and PWD environment variables. 
 *         If the change fails for any reason, it returns an appropriate 
 *         error code and prints an error message.
 */
t_ecode	chdir_default(t_env **env_list, char *directory, char *cwd)
{
	t_ecode	status;

	if (directory[0] == '~')
		status = handle_tilde_absolute_path(env_list, directory);
	else if (directory[0] == '/')
		status = handle_absolute_path(directory);
	else
		status = handle_relative_path(directory, cwd);
	if (status)
		return (status);
	return (update_oldpwd_pwd(env_list, cwd));
}

/**
 * @brief Replaces the tilde ('~') in the given directory path 
 *        with the user's home directory and attempts to change to that path.
 * 
 * This function retrieves the absolute path corresponding to the tilde, 
 * checks if the modified path is accessible, and then performs the directory 
 * change if it is valid.
 * 
 * @param[in] env_list A double pointer to the environment list, 
 *                 which is used to retrieve the home directory.
 * @param[in] directory The absolute path that begins with a tilde ('~'), 
 *               indicating that it should be replaced with the home directory.
 * 
 * @return Returns SUCCESS if the directory change is successful. 
 *         If the path is not accessible or if changing the directory fails, 
 *         an error message is printed, and it returns FAILURE.
 */
static t_ecode	handle_tilde_absolute_path(t_env **env_list, char *directory)
{
	char	*curpath;

	curpath = get_tilde_absolute_path(*env_list, directory);
	if (!curpath)
		return (FAILURE);
	if (check_curpath_access(curpath))
	{
		handle_builtin_err("cd", NULL, strerror(errno));
		return (FAILURE);
	}
	if (chdir(curpath))
		return (FAILURE);
	return (SUCCESS);
}

/**
 * @brief Replaces the tilde ('~') in the specified directory path 
 *        with the user's home directory and constructs the absolute path.
 * 
 * This function checks if the given directory starts with a tilde. If so, 
 * it retrieves the home directory path from the environment variables or 
 * falls back to the HOME environment variable. It then appends the rest 
 * of the directory path after the tilde.
 * 
 * @param[in] env_list A pointer to the environment list, which is used 
 *                  to find the HOME directory.
 * @param[in] directory The target directory string that may begin with a tilde
 * 
 * @return Returns a newly allocated string containing the absolute path 
 *         with the tilde replaced by the home directory on success. 
 *         Returns NULL and prints an error message if there are issues 
 *         with memory allocation or if the directory does not start 
 *         with a valid format.
 */
static char	*get_tilde_absolute_path(t_env *env_list, char *directory)
{
	char	*curpath;
	t_env	*home_node;
	char	*home_path;

	if (!env_list || !directory || directory[1] != '/')
		return (NULL);
	home_node = find_env_node(env_list, "HOME");
	if (home_node)
		curpath = ft_strdup(home_node->value);
	else
	{
		home_path = get_home();
		if (!home_path)
			return (handle_perror("get_tilde_absolute_path"), NULL);
		curpath = ft_strdup(home_path);
		ft_free((void **)&home_path);
	}
	if (!curpath)
		return (handle_perror("get_tilde_absolute_path"), NULL);
	curpath = ft_strjoin_fs1(&curpath, &directory[1]);
	if (!curpath)
		return (handle_perror("get_tilde_absolute_path"), NULL);
	return (curpath);
}

/**
 * @brief Checks the accessibility of a given absolute directory path 
 *        and attempts to change the current working directory to it.
 * 
 * This function first verifies if the specified absolute directory 
 * is accessible. If access is granted, it then attempts to change 
 * the current working directory to that directory. 
 * 
 * @param[in] directory The absolute path of the directory to change into.
 * 
 * @return Returns FAILURE if the directory is inaccessible or if 
 *         the change directory operation fails, printing an error 
 *         message in both cases. Returns SUCCESS if the directory 
 *         change is successful.
 */
static t_ecode	handle_absolute_path(char *directory)
{
	if (check_curpath_access(directory))
	{
		handle_builtin_err("cd", NULL, strerror(errno));
		return (FAILURE);
	}
	if (chdir(directory))
		return (FAILURE);
	return (SUCCESS);
}

/**
 * @brief Constructs a full path by concatenating the current working 
 *        directory with a relative directory path, and attempts to 
 *        change into the resulting directory.
 * 
 * This function takes a relative directory path and the current 
 * working directory, concatenates them to form an absolute path, 
 * and then attempts to change the current working directory to that 
 * absolute path.
 * 
 * @param[in] directory The relative path of the directory to change into.
 * @param[in] cwd The current working directory, used as the base for 
 *            constructing the new path.
 * 
 * @return Returns FAILURE if there are any memory allocation errors 
 *         or if the directory change fails, printing an appropriate 
 *         error message in both cases. Returns SUCCESS if the 
 *         directory change is successful.
 */
static t_ecode	handle_relative_path(char *directory, char *cwd)
{
	char	*curpath;
	t_ecode	status;

	curpath = ft_strdup(cwd);
	if (!curpath)
		return (handle_perror("handle_relative_path"), MALLOC_ERROR);
	status = (t_ecode)append_suffix(&curpath, "/", false);
	if (status)
		return (handle_perror("handle_relative_path"), MALLOC_ERROR);
	curpath = ft_strjoin_fs1(&curpath, directory);
	if (!curpath)
		return (handle_perror("handle_relative_path"), MALLOC_ERROR);
	status = check_curpath_access(curpath);
	if (status)
	{
		handle_builtin_err("cd", NULL, strerror(errno));
		return (ft_free((void **)&curpath), FAILURE);
	}
	if (chdir(curpath))
	{
		handle_builtin_err("cd", NULL, strerror(errno));
		return (ft_free((void **)&curpath), FAILURE);
	}
	ft_free((void **)&curpath);
	return (SUCCESS);
}
