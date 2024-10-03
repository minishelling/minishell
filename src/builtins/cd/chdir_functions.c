#include "../../../include/minishell.h"

//Chdir functions
t_ecode	chdir_home(t_env **env_list, char **cwd)
{
	t_env	*home_node;
	t_ecode	exit_status;
	char	*error_message;

	fprintf(stderr, "We're in chdir_home\n");
	if (!*env_list)
		return (NULL_ENV);
	home_node = find_env_node(*env_list, "HOME");
	error_message = NULL;
	if (!home_node || !home_node->value)
	{
		write(2, "mini_shared: cd: HOME not set\n", 30);
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

	fprintf(stderr, "We're in chdir_tilde\n");
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

	fprintf(stderr, "We're in chdir_dash\n");
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

char	*get_tilde_absolute_path(t_env *env_list, char *directory)
{
	char	*curpath;
	t_env	*home_node;
	char	*home_path;

	if (!env_list || !directory || directory[1] != '/')
		return (NULL);
	home_node = find_env_node(env_list, "HOME");
	if (!home_node)
		home_path = get_home();
	if (!home_node && !home_path)
		return (NULL);
	if (home_node)
		curpath = ft_strdup(home_node->value);
	else
	{
		curpath = ft_strdup(home_path);
		ft_free((void **)&home_path);
	}
	if (!curpath)
		return (NULL);
	curpath = ft_strjoin_fs1(&curpath, &directory[1]);
	if (!curpath)
		return (NULL);
	return (curpath);
}

t_ecode	chdir_default(t_env **env_list, char *directory, char **cwd)
{ //Should it free cwd in here or in the main?
	char	*curpath;
	t_ecode	status = 0;

	fprintf(stderr, "We're in chdir_default\n");
	if (directory[0] == '~') //Split into handle_tilde_absolute_path
	{
		fprintf(stderr, "We're in handle_tilde_abs_path\n");
		curpath = get_tilde_absolute_path(*env_list, directory);
		if (!curpath)
			return (status);
		status = check_curpath_access(curpath);
		if (status)
			return (status);
		status = chdir(curpath);
	}
	else if (directory[0] == '/') //Split into handle_absolute_path
	{
		fprintf(stderr, "Checking if we go into absolute path\n");
		status = check_curpath_access(directory);
		if (status)
		{
			return (status);
		}
		status = chdir(directory);
	}
	else //Split into handle_relative_path
	{
		fprintf(stderr, "Checking if we go into relative path\n");
		curpath = ft_strdup(*cwd);
		if (!curpath)
			return (MALLOC_ERROR);
		status = (t_ecode)append_suffix(&curpath, "/", false);
		if (status)
			return (MALLOC_ERROR);
		curpath = ft_strjoin_fs1(&curpath, directory);
		if (!curpath)
			return (MALLOC_ERROR);
		status = check_curpath_access(curpath);
		if (status)
			return (ft_free((void **)&curpath), status);
		status = chdir(curpath);
		ft_free((void **)&curpath);
	}
	if (status)
		return (status);
	return (update_oldpwd_pwd(env_list, cwd));
}
