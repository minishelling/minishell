#include "../../include/minishell.h"

static char	*ft_strjoin_cmd_path(char *path, char *cmd_name)
{
	char	*cmd_path;
	size_t	path_len;

	if (!path || !cmd_name)
		return (NULL);
	path_len = ft_strlen(path);
	if (path[path_len - 1] == '/')
	{
		// printf("Ending in slash.\n");
		return (ft_strjoin(path, cmd_name));
	}
	else
	{
		// printf("Not ending in slash.\n");
		cmd_path = ft_strjoin(path, "/");
		return (ft_strjoin_fs1(&cmd_path, cmd_name));
	}
}

char	*get_cmd_path(t_shell *shell, char *cmd_name)
{
	t_env	*path_node;
	char	*cmd_path;
	char	**paths;
	int		i;
	int		status;

	if (!cmd_name)
		return (NULL);
	paths = NULL;
	path_node = find_env_node(shell->env_list, "PATH");
	if (path_node)
		paths = ft_split(path_node->value, ':');
	if (!paths)
	{ //Checks if the cmd_name is already executable.
		status = access(cmd_name, X_OK);
		if (!status)
			return (cmd_name);
		cmd_path = ft_strjoin("./", cmd_name);
		if (!cmd_path)
			return (NULL);
		fprintf(stderr, "Cmd_path for rel path is: %s\n", cmd_path);
		status = access(cmd_path, X_OK);
		if (!status)
			return (cmd_path);
		return (ft_free((void **) &cmd_path), NULL);
	}
	i = 0;
	while (paths[i])
	{
		cmd_path = ft_strjoin_cmd_path(paths[i], cmd_name);
		if (!cmd_path)
		{
			i++;
			continue ;
		}
		status = access(cmd_path, X_OK);
		if (!status)
		{
			ft_free_2d((void ***) &paths);
			return (cmd_path);
		}
		ft_free((void **) &cmd_path);
		i++;
	}
	status = access(cmd_name, X_OK);
	if (!status)
		return (cmd_name);
	cmd_path = ft_strjoin("./", cmd_name);
	if (!cmd_path)
		return (NULL);
	fprintf(stderr, "Cmd_path for rel path is: %s\n", cmd_path);
	status = access(cmd_path, X_OK);
	if (!status)
		return (cmd_path);
	//Set FT_ERRNO to file not found and return NULL.
	return (ft_free((void **) &cmd_path), NULL);
}
