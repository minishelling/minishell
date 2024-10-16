#include "../../../include/minishell.h"

static t_ecode	handle_tilde_absolute_path(t_env **env_list, char *directory);
static t_ecode	handle_absolute_path(char *directory);
static t_ecode	handle_relative_path(char *directory, char *cwd);

/**
 * @brief It will attempt to change directory if the directory is
 * an absolute path, an absolute path beginning with tilde,
 * or a relative path and CDPATH was not valid.
 * @param env_list A double pointer to the head of the environment list.
 * @param directory The directory to change into.
 * @param cwd The current working directory.
 * @return If it successfully changed directories,
 * and updates the OLDPWD and PWD it returns SUCCESS.
 * On failure it returns an error code and prints an error message.
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
 * @brief It replaces tilde with the home directory,
 * and attempts to change into that modified directory.
 * @param env_list A double pointer to the environment list.
 * @param directory The absolute path directory beginning with tilde.
 * @return On success it returns SUCCESS. On failure an error message
 * is printed and it returns FAILURE.
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
 * @brief Replaces tilde with the home directory
 * and returns the absolute path of the directory.
 * @param env_list The environment list to use for the HOME directory.
 * @param directory The target directory.
 * @return On success it returns a string that has the tilde replaced
 * with the home directory.
 * On failure it returns NULL and prints an error message.
 */
char	*get_tilde_absolute_path(t_env *env_list, char *directory)
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
 * @brief Checks access of the directory path,
 * and if the directory has access it attempts to change into it.
 * @param directory The directory to change into.
 * @return If the directory has no access it prints an error message
 * and returns FAILURE. If it fails to change directories it also returns
 * FAILURE. Otherwise it returns SUCCESS.
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
 * @brief Concatenates the directory to the current working directory,
 * and tries to change into it.
 * @param directory The relative path to the directory.
 * @param cwd The current working directory.
 * @return It returns FAILURE on malloc errors or if it unsuccessfully
 * changed directories, printing an error message as well.
 * Otherwise it returns SUCCESS.
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
