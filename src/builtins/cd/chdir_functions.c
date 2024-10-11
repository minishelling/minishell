#include "../../../include/minishell.h"

//Chdir functions
t_ecode	chdir_home(t_env **env_list, char **cwd)
{
	t_env	*home_node;
	t_ecode	exit_status;

	if (!*env_list)
		return (NULL_ENV);
	home_node = find_env_node(*env_list, "HOME");
	if (!home_node || !home_node->value)
	{
		handle_builtin_err("cd", NULL, "HOME not set");
		ft_free((void **) cwd);
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
		handle_builtin_err("cd", NULL, "HOME not set");
		return (FAILURE);
	}
	status = chdir(home_path);
	ft_free((void **) &home_path);
	if (status)
	{
		handle_builtin_err("cd", NULL, strerror(errno));
		return (ft_free((void **) &cwd), status);
	}
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
		handle_builtin_err("cd", NULL, "OLDPWD not set");
		return (ft_free ((void **) &cwd), FAILURE);
	}
	status = chdir(oldpwd_node->value);
	if (status)
		return (ft_free((void **) &cwd), status);
	return (update_oldpwd_pwd(env_list, cwd));
}
