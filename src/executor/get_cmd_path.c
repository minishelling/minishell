#include "../../include/minishell.h"

static char	*ft_strjoin_cmd_path(char *path, char *cmd_name)
{
	char	*cmd_path;

	if (!path || !cmd_name)
		return (NULL);
	cmd_path = ft_strdup(path);
	if (!cmd_path)
		return (NULL);
	if (append_suffix(&cmd_path, "/", false))
		return (NULL);
	cmd_path = ft_strjoin_fs1(&cmd_path, cmd_name);
	if (!cmd_path)
		return (NULL);
	return (cmd_path);
}

static char	*check_name_as_relative_path(char *cmd_name)
{
	if (access(cmd_name, X_OK))
		return (NULL);
	return (cmd_name);
}

static char	*check_path_access(char *current_path, char *cmd_name)
{
	char	*cmd_path;

	cmd_path = ft_strjoin_cmd_path(current_path, cmd_name);
	if (!cmd_path)
		return (NULL);
	if (!access(cmd_path, X_OK))
		return (cmd_path);
	ft_free((void **) &cmd_path);  // g_exitcode = 126 ?
	return (NULL);
}

char	*get_cmd_path(t_shell *shell, char *cmd_name)
{
	t_env	*path_node;
	char	*cmd_path;
	char	**paths;
	int		i;

	if (!cmd_name)
		return (NULL);
	paths = NULL;
	path_node = find_env_node(shell->env_list, "PATH");
	if (path_node)
		paths = ft_split(path_node->value, ':');
	if (!paths)
		return (check_name_as_relative_path(cmd_name));
	i = 0;
	while (paths[i])
	{
		cmd_path = check_path_access(paths[i], cmd_name);
		if (cmd_path)
			return(cmd_path);
		i++;
	}
	ft_free_2d((void ***) &paths);
	return (check_name_as_relative_path(cmd_name));
}
