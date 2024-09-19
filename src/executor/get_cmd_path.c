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

	path_node = find_env_node(shell->env_list, "PATH");
	if (!path_node || !path_node->value || !cmd_name)
		return (NULL); //Idea to set ft_errno.
	paths = ft_split(path_node->value, ':');
	if (!paths)
		return (NULL);
	i = 0;
	while (paths[i])
	{
		cmd_path = ft_strjoin_cmd_path(paths[i], cmd_name);
		if (!cmd_path)
		{
			i++;
			continue ;
		}
		// printf("CMD_PATH in get_cmd_path: %s\n", cmd_path);

		//NOT NECCESSARY.
		// status = access(cmd_path, F_OK);
		// if (status)
		// {
		// 	i++;
		// 	continue ; //Set ft_errno to file not found.
		// }
		
		//In this case ERRNO should be set to "EACCES | 13" but it doesn't stop finding a possible path.
		status = access(cmd_path, X_OK);
		if (!status)
		{
			ft_free_2d((void ***) &paths);
			return (cmd_path);
		}
		ft_free((void **) &cmd_path);
		i++;
	}
	//Set FT_ERRNO to file not found and return NULL.
	return (NULL);
}