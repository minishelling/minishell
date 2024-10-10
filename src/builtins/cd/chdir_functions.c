#include "../../../include/minishell.h"

//Chdir functions
t_ecode	chdir_home(t_env **env_list, char **cwd)
{
	t_env	*home_node;
	t_ecode	exit_status;
	char	*error_message;

	if (!*env_list)
		return (NULL_ENV);
	home_node = find_env_node(*env_list, "HOME");
	error_message = NULL;
	if (!home_node || !home_node->value)
	{
		handle_builtin_err("cd", "HOME not set");
		// write(2, "mini_shared: cd: HOME not set\n", 30);
		ft_free((void **) cwd);
		return (HOME_ERROR);
	}
	else
	{
		exit_status = chdir(home_node->value);
		if (exit_status)
		{
			error_message = ft_strjoin("mini_shared: cd: ", home_node->value);
			if (!error_message)
				return (MALLOC_ERROR);
			return (perror("error_message"), ft_free((void **) &error_message), HOME_ERROR); //Returning HOME_ERROR code instead of FAILURE in the case I handle all error codes & messages outside the function.
		}
		return (update_oldpwd_pwd(env_list, cwd));
	}
}

//Chdir functions
t_ecode	chdir_tilde(t_env **env_list, char **cwd)
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
		//PRINT mini_shared: cd: HOME not set.
		return (FAILURE);
	}
	status = chdir(home_path);
	ft_free((void **) &home_path);
	if (status)
		return (ft_free((void **) &cwd), status);
	return (update_oldpwd_pwd(env_list, cwd));
}

//Chdir functions
t_ecode	chdir_dash(t_env **env_list, char **cwd)
{
	t_env	*oldpwd_node;
	int		status;

	if (!*env_list)
		return (NULL_ENV);
	oldpwd_node = find_env_node(*env_list, "OLDPWD");
	if (!oldpwd_node)
	{
		//PRINT mini_shared: cd: OLDPWD not set.
		return (ft_free ((void **) &cwd), FAILURE);
	}
	status = chdir(oldpwd_node->value);
	if (status)
		return (ft_free((void **) &cwd), status);
	return (update_oldpwd_pwd(env_list, cwd));
}
