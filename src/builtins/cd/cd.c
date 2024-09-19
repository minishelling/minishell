#include "../../../include/minishell.h"

// typedef	struct s_cd_struct
// {

// }	t_cd_struct;

t_ecode	chdir_curpath(t_shell **shell, char **curpath, char **cwd)
{
	int	status;

	(void) shell;
	status = chdir(*curpath);
	ft_free((void **) curpath);
	ft_free((void **) cwd);
	printf("chdir status: %d\n", status);
	return (0);
}

t_ecode	chdir_tilde(t_env *env_list, char **cwd)
{
	t_env	*home_node;
	char	*home_path;
	int		status;

	home_node = find_env_node(env_list, "HOME");
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
	//CALL FUNCTION TO UPDATE PWD, OLDPWD and free cwd.
	return (status);
}

t_ecode	chdir_dash(t_env *env_list, char **cwd)
{
	t_env	*oldpwd_node;
	int		status;

	oldpwd_node = find_env_node(env_list, "OLDPWD");
	if (!oldpwd_node)
	{
		//PRINT mini_shared: cd: OLDPWD not set.
		return (FAILURE);
	}
	status = chdir(oldpwd_node->value);
	(void) cwd;
	//CALL FUNCTION TO UPDATE PWD, OLDPWD and free cwd.
	return (status);
}

//For ~ use getenv("HOME");
t_ecode	builtin_cd(t_shell **shell, char *directory)
{
	char		*curpath;
	char		*cwd;
	t_ecode		status;

	curpath = NULL;
	cwd = getcwd(NULL, PATH_MAX);
	if (!cwd)
		return (CWD_ERROR);
	if (!directory || !ft_strncmp(directory, "--", ft_strlen(directory)))
		return (chdir_home((*shell)->env_list, &cwd));
	else if (!ft_strncmp(directory, "~", ft_strlen(directory)))
	{
		status = chdir_tilde((*shell)->env_list, &cwd);
		return (status);
	}
	else if (!ft_strncmp(directory, "-", ft_strlen(directory)))
	{
		status = chdir_dash((*shell)->env_list, &cwd);
		return (status);
	}
	else if (is_dir_prefix_valid_for_cdpath(directory))
	{
		status = chdir_cdpath(shell, directory);
		if (!status)
			return (SUCCESS);
		else if (status == MALLOC_ERROR)
			return (FAILURE); //And free everything... Rather 'exit' than return.
	}
	// curpath = ft_strdup(directory);
	// if (!curpath)
	// 	return (MALLOC_ERROR);
	// if (ft_strncmp(curpath, "/", ft_strlen(curpath)))
	// {

	// }
	status = curpath_prepare(&curpath, directory, cwd);
	if (status)
		return (status);
	status = curpath_trim(&curpath); // I think I have to append cwd to curpath, otherwise .. doesn't work.
	if (status)
		return (status); //Free everything.
	printf("Curpath in builtin_cd: %s\n", curpath);
	return (chdir_curpath(shell, &curpath, &cwd));
}

t_ecode	chdir_home(t_env *env_head, char **cwd)
{
	t_env	*home_node;
	t_ecode	exit_status;

	home_node = find_env_node(env_head, "HOME");
	if (!home_node || !home_node->value)
	{
		// PRINT ERROR
		ft_free((void **) cwd);
		return (HOME_ERROR);
	}
	else
	{
		exit_status = chdir(home_node->value);
		if (exit_status)
			return (CHDIR_ERROR);
		update_env_node(env_head, "PWD", getcwd(NULL, PATH_MAX), true);
		update_env_node(env_head, "OLDPWD", *cwd, true);
		ft_free((void **) cwd);
		return (SUCCESS);
	}
}

t_ecode	chdir_cdpath(t_shell **shell, char *directory)
{
	t_env	*cdpath_node;
	char	**values;
	t_ecode	status;

	cdpath_node = find_env_node((*shell)->env_list, "CDPATH");
	if (!cdpath_node || !cdpath_node->value)
		return (CDPATH_NULL);
	values = ft_split(cdpath_node->value, ':');
	if (!values)
		return (MALLOC_ERROR);
	status = loop_cdpath_values(&values, directory);
	return (status);
}

// TEST This function. Idea: make test directory equipped with libft.
t_ecode	append_suffix(char **str, char *suffix, bool duplicate)
{
	size_t	suffix_len;

	if (!*str || !suffix)
		return (NULL_STRING);
	suffix_len = ft_strlen(suffix);
	// printf("In append_suffix: str[suffix_len]: %s\n", str[suffix_len]);
	if (!ft_strncmp(str[suffix_len], suffix, suffix_len) && !duplicate)
	{
		// printf("in append_suffix, returning success\n");
		return (SUCCESS);
	}
	*str = ft_strjoin_fs1(str, suffix);
	if (!*str)
		return (MALLOC_ERROR);
	else return (SUCCESS);
}
