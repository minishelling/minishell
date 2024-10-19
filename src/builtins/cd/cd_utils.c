#include "../../../include/minishell.h"

/**
 * @brief Updates the PWD and OLDPWD environment nodes
 * with the new values.
 * 
 * @param env_list A double pointer to the environment list.
 * @param cwd The cwd string that holds the value of OLDPWD,
 * and that's going to be updated with the value of PWD.
 * 
 * @return It returns an error code in the case of a NULL env list,
 * or in the case that it fails to update OLDPWD or PWD,
 * while also printing an error message.
 * Returns SUCCESS if it succeeds in updating both OLDPWD and PWD.
 */
t_ecode	update_oldpwd_pwd(t_env **env_list, char *cwd)
{
	if (!*env_list)
		return (NULL_ERROR);
	if (update_env_node(env_list, "OLDPWD", cwd, true))
	{
		handle_builtin_err("cd", NULL, "failed to update OLDPWD.");
		return (FAILURE);
	}
	getcwd(cwd, PATH_MAX);
	if (update_env_node(env_list, "PWD", cwd, true))
	{
		handle_builtin_err("cd", NULL, "failed to update PWD.");
		return (FAILURE);
	}
	return (SUCCESS);
}

/**
 * @brief Returns the path to home,
 * retrieved from the original HOME environment variable.
 * 
 * @param void
 * 
 * @return A string containing the path to home,
 * or NULL if it doesn't exist or if there was a malloc failure.
 */
char	*get_home(void)
{
	char	*home_path;
	char	**home_breakdown;
	size_t	home_path_len;

	home_breakdown = NULL;
	home_path = getenv("HOME");
	if (home_path)
	{
		home_breakdown = ft_split(home_path, '/');
		if (!home_breakdown)
			return (handle_perror("get_home"), NULL);
		home_path_len = ft_strlen("/") * 2 + ft_strlen(home_breakdown[0])
			+ ft_strlen(home_breakdown[1]);
		home_path = ft_calloc(home_path_len + 1, sizeof(char));
		if (!home_path)
			return (ft_free_2d((void ***)&home_breakdown),
				handle_perror("get_home"), NULL);
		ft_strlcpy(home_path, "/", 2);
		ft_strlcat(home_path, home_breakdown[0], home_path_len + 1);
		ft_strlcat(home_path, "/", home_path_len + 1);
		ft_strlcat(home_path, home_breakdown[1], home_path_len + 1);
		ft_free_2d((void ***)&home_breakdown);
		return (home_path);
	}
	return (NULL);
}

/**
 * @brief Checks whether a directory string
 * has a valid prefix to go into cdpath.
 * A valid prefix is a directory that does not start with:
 * '/' '.' '..' or '~'.
 * 
 * @param directory The directory string.
 * 
 * @return True if the directory doesn't start with any of these characters.
 * False if it does.
 */
bool	has_cdpath_prefix(char *directory)
{
	if (!directory || directory[0] == '/' || directory[0] == '.'
		|| (directory[0] == '.' && directory[1] == '.') || directory[0] == '~')
		return (false);
	return (true);
}

/**
 * @brief Checks access of curpath with F_OK and X_OK.
 * 
 * @param curpath The current path variable.
 * @return If the directory was not found it returns FAILURE
 * and errno is set accordingly. If the directory was found
 * but is not executable/accessible, it also returns FAILURE
 * and errno is set accordingly. Otherwise it returns SUCCESS. 
 */
t_ecode	check_curpath_access(char *curpath)
{
	if (access(curpath, F_OK))
		return (FAILURE);
	if (access(curpath, X_OK))
		return (FAILURE);
	return (SUCCESS);
}
