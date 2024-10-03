#include "../../../include/minishell.h"

char	*get_home(void)
{
	char	*home_path;
	char	**home_breakdown;

	home_breakdown = NULL;
	home_path = getenv("HOME");
	if (home_path)
	{
		home_breakdown = ft_split(home_path, '/');
		if (!home_breakdown)
			return (NULL);
	}
	home_path = ft_strjoin("/", home_breakdown[0]);
	if (!home_path)
		return (ft_free_2d((void ***) &home_breakdown), NULL);
	home_path = ft_strjoin_fs1(&home_path, "/");
	if (!home_path)
		return (ft_free_2d((void ***) &home_breakdown), NULL);
	home_path = ft_strjoin_fs1(&home_path, home_breakdown[1]);
	if (!home_path)
		return (ft_free_2d((void ***) &home_breakdown), NULL);
	ft_free_2d((void ***) &home_breakdown);
	return (home_path);
}

bool	has_cdpath_prefix(char *directory)
{
	if (!directory || directory[0] == '/' || directory[0] == '.'
		|| (directory[0] == '.' && directory[1] == '.') || directory[0] == '~')
		return (false);
	return (true);
}

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
