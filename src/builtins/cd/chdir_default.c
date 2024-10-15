#include "../../../include/minishell.h"

static t_ecode	handle_tilde_absolute_path(t_env **env_list, char *directory);
static t_ecode	handle_absolute_path(char *directory);
static t_ecode	handle_relative_path(char *directory, char *cwd);

t_ecode	chdir_default(t_env **env_list, char *directory, char *cwd)
{
	t_ecode	status;

	if (directory[0] == '~')
		status = handle_tilde_absolute_path(env_list, directory);
	else if (directory[0] == '/')
		status = handle_absolute_path(directory);
	else
		status = handle_relative_path(directory, cwd);
	if (status)
		return (status);
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
	if (home_node)
		curpath = ft_strdup(home_node->value);
	else
	{
		home_path = get_home();
		if (!home_path)
			return (handle_perror("get_tilde_absolute_path"), NULL);
		curpath = ft_strdup(home_path);
		ft_free((void **)&home_path);
	}
	if (!curpath)
		return (handle_perror("get_tilde_absolute_path"), NULL);
	curpath = ft_strjoin_fs1(&curpath, &directory[1]);
	if (!curpath)
		return (handle_perror("get_tilde_absolute_path"), NULL);
	return (curpath);
}

static t_ecode	handle_tilde_absolute_path(t_env **env_list, char *directory)
{
	char	*curpath;
	t_ecode	status;

	status = SUCCESS;
	curpath = get_tilde_absolute_path(*env_list, directory);
	if (!curpath)
		return (status);
	status = check_curpath_access(curpath);
	if (status)
	{
		handle_builtin_err("cd", NULL, strerror(errno));
		return (status);
	}
	status = chdir(curpath);
	return (status);
}

static t_ecode	handle_absolute_path(char *directory)
{
	t_ecode	status;

	status = check_curpath_access(directory);
	if (status)
	{
		handle_builtin_err("cd", NULL, strerror(errno));
		return (status);
	}
	status = chdir(directory);
	return (status);
}

static t_ecode	handle_relative_path(char *directory, char *cwd)
{
	char	*curpath;
	t_ecode	status;

	curpath = ft_strdup(cwd);
	if (!curpath)
		return (handle_perror("handle_relative_path"), MALLOC_ERROR);
	status = (t_ecode)append_suffix(&curpath, "/", false);
	if (status)
		return (handle_perror("handle_relative_path"), MALLOC_ERROR);
	curpath = ft_strjoin_fs1(&curpath, directory);
	if (!curpath)
		return (handle_perror("handle_relative_path"), MALLOC_ERROR);
	status = check_curpath_access(curpath);
	if (status)
	{
		handle_builtin_err("cd", NULL, strerror(errno));
		return (ft_free((void **)&curpath), FAILURE);
	}
	if (chdir(curpath))
	{
		handle_builtin_err("cd", NULL, strerror(errno));
		return (ft_free((void **)&curpath), FAILURE);
	}
	ft_free((void **)&curpath);
	return (SUCCESS);
}
