#include "../../include/minishell.h"

static char	*ft_strjoin_cmd_path(char *path, char *cmd_name)
{
	char	*cmd_path;
	size_t	path_len;

	// fprintf(stderr, "Checkpoint 1 in ft_strjoin_cmd_path\n");
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


	// fprintf(stderr, "Checkpoint 2 in ft_strjoin_cmd_path\n");
	// cmd_path = ft_strdup(path);
	// fprintf(stderr, "Checkpoint 3 in ft_strjoin_cmd_path\n");
	// if (!cmd_path)
	// 	return (NULL);
	// fprintf(stderr, "Checkpoint 4 in ft_strjoin_cmd_path\n");
	// if (append_suffix(&cmd_path, "/", false))
	// 	return (NULL);
	// fprintf(stderr, "Checkpoint 5 in ft_strjoin_cmd_path\n");
	// cmd_path = ft_strjoin_fs1(&cmd_path, cmd_name);
	// if (!cmd_path)
	// 	return (NULL);
	// fprintf(stderr, "Checkpoint 6 in ft_strjoin_cmd_path\n");
	// return (cmd_path);
}

static char	*check_name_as_relative_path(char *cmd_name)
{
	if (access(cmd_name, X_OK) == 0)
		return (cmd_name);
	return (NULL);
}

static char	*check_path_access(char *current_path, char *cmd_name)
{
	char	*cmd_path;

	cmd_path = ft_strjoin_cmd_path(current_path, cmd_name);
	// fprintf(stderr, "Checkpoint in check_path_access\n");
	if (!cmd_path)
		return (NULL);
	if (access(cmd_path, X_OK) == 0)
		return (cmd_path);
	ft_free((void **) &cmd_path);
	return (NULL);
}

char	*get_cmd_path(t_shell *shell, char *cmd_name)
{
	t_env	*path_node;
	char	*cmd_path;
	char	**paths;
	int		i;

	if (!cmd_name)
	{
		// fprintf(stderr, "No cmd_name in path\n");
		return (NULL);
	}
	paths = NULL;
	path_node = find_env_node(shell->env_list, "PATH");
	// fprintf(stderr, "Checkpoint 1 in path\n");
	if (path_node)
		paths = ft_split(path_node->value, ':');
	// fprintf(stderr, "Checkpoint 2 in path\n");
	if (!paths)
	{
		// fprintf(stderr, "Check name as rel path 1 in path\n");
		return (check_name_as_relative_path(cmd_name));
	}
	i = 0;
	// fprintf(stderr, "Checkpoint 3 in path\n");
	while (paths[i])
	{
		cmd_path = check_path_access(paths[i], cmd_name);
		// fprintf(stderr, "Checkpoint i: %d in path\n", i);
		if (cmd_path)
		{
			// fprintf(stderr, "cmd_path found: %s in path\n", cmd_path);
			return(cmd_path);
		}
		// fprintf(stderr, "Checkpoint i: %d in path\n", i);
		i++;
	}
	// fprintf(stderr, "Checkpoint 4 in path\n");
	ft_free_2d((void ***) &paths);
	// fprintf(stderr, "Check name as rel path 2 in path\n");
	return (check_name_as_relative_path(cmd_name));
}
