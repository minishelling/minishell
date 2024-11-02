/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   cd_utils.c                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: lprieri <lprieri@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/10/31 13:31:42 by lprieri       #+#    #+#                 */
/*   Updated: 2024/10/31 13:31:43 by lprieri       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

/**
 * @brief Updates the PWD and OLDPWD environment nodes with new values.
 * 
 * This function sets the OLDPWD environment variable to the current 
 * working directory (cwd) and updates the cwd to reflect the new PWD 
 * environment variable. If any update fails, it prints an error message 
 * and returns a failure code.
 * 
 * @param[in,out] env_list A double pointer to the environment list.
 * @param[in,out] cwd A string representing the current working directory 
 *                    (cwd). It initially holds the value for OLDPWD and 
 *                    is updated with the new PWD value.
 * 
 * @return Returns SUCCESS if both OLDPWD and PWD are successfully updated. 
 * Returns NULL_ERROR if the environment list is NULL, or FAILURE if it 
 * fails to update OLDPWD or PWD, while printing an error message in either 
 * case.
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
 * @brief Retrieves the path to the home directory from the HOME 
 * environment variable, reconstructing it from the initial segments.
 * 
 * The function attempts to split the HOME path by '/' and then constructs 
 * a new string using the first two components, resulting in a path with the 
 * format "/user/home". If memory allocation fails or HOME is not defined, 
 * it returns NULL.
 * 
 * @param void
 * 
 * @return A string containing the reconstructed path to home, or NULL if 
 * the HOME variable is not set or if a memory allocation error occurs.
 * 
 * @note The caller is responsible for freeing the returned string.
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
 * @brief Checks if a directory string has a valid prefix for CDPATH usage.
 * 
 * A valid prefix for CDPATH is a directory that does not begin with any of 
 * the following characters or sequences: '/', '.', '..', or '~'.
 * 
 * @param[in] directory The directory string to check.
 * 
 * @return True if the directory has a valid prefix for CDPATH.
 *         False if it starts with an invalid prefix.
 */
bool	has_cdpath_prefix(char *directory)
{
	if (!directory || directory[0] == '/' || directory[0] == '.'
		|| (directory[0] == '.' && directory[1] == '.') || directory[0] == '~')
		return (false);
	return (true);
}

/**
 * @brief Checks the accessibility of the specified path.
 * 
 * This function verifies if the given path exists and if it is executable.
 * It uses the `access` system call to check for the presence of the path 
 * (using `F_OK`) and to check if the path can be executed (using `X_OK`).
 * 
 * @param[in] curpath The current path to check for accessibility.
 * 
 * @return SUCCESS if the path exists and is executable.
 *         FAILURE if the path does not exist or is not executable, 
 *         with errno set accordingly to indicate the error.
 */
t_ecode	check_curpath_access(char *curpath)
{
	if (access(curpath, F_OK))
		return (FAILURE);
	if (access(curpath, X_OK))
		return (FAILURE);
	return (SUCCESS);
}
