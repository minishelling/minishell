#include "../../../include/minishell.h"

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

t_ecode	builtin_cd(t_shell **shell, char *directory)
{
	char		*curpath;
	char		*cwd;
	t_ecode		status;

	cwd = getcwd(NULL, PATH_MAX);
	if (!cwd)
		return (CWD_ERROR);
	if (!directory)
		return (chdir_home((*shell)->env_list, &cwd));
	else if (is_dir_prefix_valid(directory))
	{
		status = chdir_cdpath(shell, directory);
		if (!status)
			return (SUCCESS);
		else if (status == MALLOC_ERROR)
			return (FAILURE); //And free everything... Rather 'exit' than return.
	}
	status = curpath_trim(directory, &curpath);
	if (status)
		return (status); //Free everything.
	return (chdir_curpath(shell, &curpath, &cwd));
}

t_ecode	chdir_home(t_env *env_head, char **cwd)
{
	t_env	*home_node;
	t_ecode	exit_status;

	home_node = env_find_node(env_head, "HOME");
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
		env_update_node(env_head, "PWD", getcwd(NULL, PATH_MAX), true);
		env_update_node(env_head, "OLDPWD", *cwd, true);
		ft_free((void **) cwd);
		return (SUCCESS);
	}
}

t_ecode	chdir_cdpath(t_shell **shell, char *directory)
{
	t_env	*cdpath_node;
	char	**values;
	t_ecode	status;

	cdpath_node = env_find_node((*shell)->env_list, "CDPATH");
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
	if (ft_strncmp(str[suffix_len], suffix, suffix_len) && !duplicate)
		return (SUCCESS);
	*str = ft_strjoin_fs1(str, suffix);
	if (!*str)
		return (MALLOC_ERROR);
	else return (SUCCESS);
}
