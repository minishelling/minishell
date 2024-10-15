#include "../../../include/minishell.h"

/**
 * @brief Updates the PWD and OLDPWD environment nodes
 * with the new values.
 * @param env_list A double pointer to the environment list.
 * @param cwd A double pointer to cwd.
 */
t_ecode	update_oldpwd_pwd(t_env **env_list, char **cwd)
{
	if (!*env_list)
		return (NULL_ERROR);
	update_env_node(env_list, "OLDPWD", *cwd, true);
	ft_free((void **) cwd);
	*cwd = getcwd(NULL, PATH_MAX);
	if (!*cwd)
		return (MALLOC_ERROR);
	update_env_node(env_list, "PWD", *cwd, true);
	ft_free((void **) cwd);
	return (SUCCESS);
}

/**
 * @brief Returns the path to home, retrieved from the original
 * HOME environment variable.
 * @param void
 * @return A string containing the path to home,
 * or NULL if it doesn't exist or if there was a malloc failure.
 */
char	*get_home(void)
{
	char *home_path;
	char **home_breakdown;
	size_t home_path_len;

	home_breakdown = NULL;
	home_path = getenv("HOME");
	if (home_path)
	{
		home_breakdown = ft_split(home_path, '/');
		if (!home_breakdown)
			return (handle_perror("get_home"), NULL);
		home_path_len = ft_strlen("/") * 2 + ft_strlen(home_breakdown[0]) +
						ft_strlen(home_breakdown[1]);
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
 * @param directory The directory string.
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
 * @brief 
 */
t_ecode	check_for_special_cd_cases(t_env *env, char *directory, char **curpath)
{
	t_env	*env_node;

	env_node = NULL;
	if (!directory)
		return (SUCCESS);
	if (!ft_strncmp(directory, "~", 1))
	{
		printf("~ is home\n");
		env_node = find_env_node(env, "HOME");
		if (!env_node)
			return (ENV_ERROR);
		*curpath = ft_strdup(env_node->value);
		if (!curpath)
			return (MALLOC_ERROR);
	}
	else if (!ft_strncmp(*curpath, "-", 1))
	{
		printf("~ is home\n");
		env_node = find_env_node(env, "OLDPWD");
		if (!env_node)
			return (ENV_ERROR);
		*curpath = ft_strdup(env_node->value);
		if (!curpath)
			return (MALLOC_ERROR);
	}
	else
	{
		*curpath = ft_strdup(directory);
		if (!curpath)
			return (MALLOC_ERROR);
	}
	return (SUCCESS);
}
