#include "../../../include/minishell.h"

/**
 * @brief Changes directory to home using the HOME env variable.
 * 
 * @param env_list A double pointer to the environment list head.
 * @param cwd The current working directory.
 * 
 * @return It returns SUCCESS if it manages to change directory
 * and update OLDPWD and PWD. In case of failure
 * it prints an error message and returns FAILURE.
 */
t_ecode	chdir_home(t_env **env_list, char *cwd)
{
	t_env	*home_node;
	t_ecode	exit_status;

	if (!*env_list)
		return (NULL_ENV);
	home_node = find_env_node(*env_list, "HOME");
	if (!home_node || !home_node->value)
	{
		handle_builtin_err("cd", NULL, "HOME not set");
		return (FAILURE);
	}
	else
	{
		exit_status = chdir(home_node->value);
		if (exit_status)
		{
			handle_builtin_err("cd", home_node->value, strerror(errno));
			return (FAILURE);
		}
		return (update_oldpwd_pwd(env_list, cwd));
	}
}

/**
 * @brief Changes directory to home.
 * If HOME is unset, it retrieves HOME's original value,
 * and extracts the first two directories.
 * 
 * @param env_list A double pointer to the environment list head.
 * @param cwd The current working directory.
 * 
 * @return It returns SUCCESS if it manages to change directory
 * and update OLDPWD and PWD. In case of failure
 * it prints an error message and returns FAILURE.
 */
t_ecode	chdir_tilde(t_env **env_list, char *cwd)
{
	t_env	*home_node;
	char	*home_path;
	int		status;

	if (!*env_list)
		return (NULL_ENV);
	home_node = find_env_node(*env_list, "HOME");
	if (home_node)
		return (chdir_home(env_list, cwd));
	home_path = get_home();
	if (!home_path)
	{
		handle_builtin_err("cd", NULL, "HOME not set");
		return (FAILURE);
	}
	status = chdir(home_path);
	ft_free((void **) &home_path);
	if (status)
	{
		handle_builtin_err("cd", NULL, strerror(errno));
		return (FAILURE);
	}
	return (update_oldpwd_pwd(env_list, cwd));
}

/**
 * @brief Changes directory to OLDPWD.
 * 
 * @param env_list A double pointer to the environment list.
 * @param cwd The current working directory.
 * 
 * @return If it successfully changes directory, and updates
 * OLDPWD and PWD it returns SUCCESS.
 * If OLDPWD was not found it returns FAILURE and prints an error message.
 * If OLDPWD was found but it fails to change directory, it also
 * prints an error message and returns FAILURE.
 */
t_ecode	chdir_dash(t_env **env_list, char *cwd)
{
	t_env	*oldpwd_node;

	if (!*env_list)
		return (NULL_ENV);
	oldpwd_node = find_env_node(*env_list, "OLDPWD");
	if (!oldpwd_node)
	{
		handle_builtin_err("cd", NULL, "OLDPWD not set");
		return (FAILURE);
	}
	if (chdir(oldpwd_node->value))
	{
		handle_builtin_err("cd", NULL, strerror(errno));
		return (FAILURE);
	}
	if (update_oldpwd_pwd(env_list, cwd))
		return (FAILURE);
	return (SUCCESS);
}
