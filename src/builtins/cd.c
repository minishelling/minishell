#include "../../include/minishell.h"

t_ecode	cd_builtin(t_shell *shell, char **cmd_args)
{
	t_ecode	e_status;
	char	*directory;
	char	*cwd;

	if (!shell || !cmd_args)
		return (NULL_ERROR);
	directory = cmd_args[1];
	cwd = getcwd(NULL, PATH_MAX);
	e_status = SUCCESS;
	if (!cwd)
		return (CWD_ERROR);
	if (!directory || !ft_strncmp(directory, "--", ft_strlen(directory)))
		e_status = chdir_home(&shell->env_list, &cwd);
	else if (!ft_strncmp(directory, "~", ft_strlen(directory)))
		e_status = chdir_tilde(&shell->env_list, &cwd);
	else if (!ft_strncmp(directory, "-", ft_strlen(directory)))
		e_status = chdir_dash(&shell->env_list, &cwd);
	else if (has_cdpath_prefix(directory))
		e_status = chdir_cdpath(&shell->env_list, directory, &cwd);
	if (e_status == PROCEED)
		e_status = chdir_default(&shell->env_list, directory, &cwd);
	//Handle exit codes and errors.
	return (SUCCESS);
}

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

//Cd util functions
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

t_ecode	chdir_cdpath(t_env **env_list, char *directory, char **cwd)
{
	t_env	*cdpath_node;
	char	**cdpath_values;
	ssize_t	values_count;
	t_ecode	status;

	if (!*env_list)
		return (NULL_ENV);
	cdpath_node = find_env_node(*env_list, "CDPATH");
	if (!cdpath_node || !cdpath_node->value)
		return (PROCEED);
	cdpath_values = ft_split(cdpath_node->value, ':');
	if (!cdpath_values)
		return (MALLOC_ERROR);
	values_count = count_values_from_env_node(*env_list, "CDPATH");
	if (values_count == -1)
		return (PROCEED);
	status = traverse_and_chdir_cdpath(cdpath_values, values_count, directory);
	ft_free_2d((void ***) &cdpath_values);
	if (status == MALLOC_ERROR)
		return (ft_free((void **) cwd), MALLOC_ERROR);
	else if (status)
		return (PROCEED);
	return (update_oldpwd_pwd(env_list, cwd));
}

t_ecode	traverse_and_chdir_cdpath(char **cdpath_values, ssize_t values_count, char *directory)
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
		status = chdir_default_cdpath(cdpath_values[i], directory, &i);
		if (status == MALLOC_ERROR)
			return (MALLOC_ERROR);
		else if (status)
			continue ;
		return (SUCCESS);
	}
	return (PROCEED);
}

t_ecode	check_curpath_access(char *curpath)
{
	t_ecode	status;

	if (!curpath)
		return (NULL_STRING);
	status = access(curpath, F_OK);
	if (status)
		return (INVALID_FILE);
	status = access(curpath, X_OK);
	if (status)
		return (ACCESS_ERROR);
	return (SUCCESS);
}

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
		return (MALLOC_ERROR);
	// status = check_curpath_access(curpath);
	// if (status)
	// {
	// 	ft_free((void **)curpath);
	// 	return (status);
	// }
	status = chdir(curpath);
	ft_free((void **)curpath);
	if (status)
		return (CHDIR_ERROR);
	return (SUCCESS);
}

t_ecode chdir_default_cdpath(char *cdpath_value, char *directory, ssize_t *i)
{
	char	*curpath;
	t_ecode	status;

	*i += 1;
	curpath = ft_strdup(cdpath_value);
	if (!curpath)
		return (MALLOC_ERROR);
	if (append_suffix(&curpath, "/", false))
		return (MALLOC_ERROR);
	curpath = ft_strjoin_fs1(&curpath, directory);
		// status = check_curpath_access(curpath);
		// if (status)
		// {
		// 	ft_free((void **) curpath);
		// 	continue ;
		// }
	status = chdir(curpath);
	ft_free((void **) curpath);
	if (status)
		return (PROCEED);
	return (SUCCESS);
}

t_ecode	chdir_default(t_env **env_list, char *directory, char **cwd)
{
	char	*curpath;
	t_ecode	status;

	curpath = ft_strdup(*cwd);
	if (!curpath)
		return (MALLOC_ERROR);
	status = (t_ecode) append_suffix(&curpath, "/", false);
	if (status)
		return (MALLOC_ERROR);
	curpath = ft_strjoin_fs1(&curpath, directory);
	if (!curpath)
		return (MALLOC_ERROR);
	status = check_curpath_access(curpath);
	if (status)
		return (ft_free((void **) &curpath), status);
	status = chdir(curpath);
	ft_free((void **) &curpath);
	if (status)
		return (status);
	return (update_oldpwd_pwd(env_list, cwd));
}
