#include "../../include/minishell.h"

static char	*check_name_as_path(t_shell *shell, char *cmd_name);
static char	*check_path_access(t_shell *shell, char *current_path, char *cmd_name);
static char	*ft_strjoin_cmd_path(char *path, char *cmd_name);

/**
 * @brief Returns the full path of the command by looking
 * at the PATH variable in the environment.
 * If there is no path node, or none of the path values are valid,
 * it checks if the command name itself is a path.
 * 
 * @param shell A pointer to the shell structure.
 * @param cmd_name The command name.
 * 
 * @return On success it returns the full path
 * to the commands executable file. On failure it returns NULL.
 */
char	*get_cmd_path(t_shell *shell, char *cmd_name)
{
	t_env	*path_node;
	char	*cmd_path;
	char	**paths;
	int		i;

	if (!cmd_name)
		return (NULL);
	if (!ft_strncmp(cmd_name, "./", 2) || cmd_name[0] == '/')
		return (check_name_as_path(shell, cmd_name));
	paths = NULL;
	path_node = find_env_node(shell->env_list, "PATH");
	if (path_node)
		paths = ft_split(path_node->value, ':');
	if (!paths)
		return (check_name_as_path(shell, cmd_name));
	i = 0;
	while (paths[i])
	{
		cmd_path = check_path_access(shell, paths[i], cmd_name);
		if (cmd_path)
			return(cmd_path);
		i++;
	}
	ft_free_2d((void ***) &paths);
	return (NULL);
}

/**
 * @brief It checks whether the command name is a valid path.
 * 
 * @param shell A pointer to the shell structure.
 * @param cmd_name The command's name.
 * 
 * @return If the file is found and is executable,
 * it returns the name of the command that will act as path.
 * If the file is a directory it sets errno to EISDIR,
 * and for files (and directories) that are found but cannot be executed
 * it sets the exit code to 126 and returns NULL.
 * Else the exit_code is set to 127 and returns NULL as well.
 */
static char	*check_name_as_path(t_shell *shell, char *cmd_name)
{
	struct stat	stat_buffer;

	if (stat(cmd_name, &stat_buffer) != 0)
	{
		shell->exit_code = 0;
		return (NULL);
	}
	if (access(cmd_name, F_OK) == 0)
	{
		if (S_ISDIR(stat_buffer.st_mode))
			errno = EISDIR;
		else if (access(cmd_name, X_OK) == 0)
			return (cmd_name);
		shell->exit_code = 126;
		return (NULL);
	}
	shell->exit_code = 127;
	return (NULL);
}

/**
 * @brief Concatenates the command name to the current path,
 * and tests if the full path is executable.
 * 
 * @param shell A pointer to the shell structure.
 * @param current_path The current path value.
 * @param cmd_name The command name.
 * 
 * @return If the full path is executable it returns the path.
 * If the path leads to a valid file but is not executable,
 * it returns NULL and sets the error code to 126.
 * Else it returns NULL.
 */
static char	*check_path_access(t_shell *shell, char *current_path, char *cmd_name)
{
	char	*cmd_path;

	cmd_path = ft_strjoin_cmd_path(current_path, cmd_name);
	if (!cmd_path)
		return (NULL);
	if (check_name_as_path(shell, cmd_path) == NULL)
	{
		ft_free((void **) &cmd_path);
		return (NULL);
	}
	return (cmd_path);
}

/**
 * @brief Returns a string with the cmd name concatenated
 * to the current path value.
 * 
 * @param path The current path value.
 * @param cmd_name The command name.
 * 
 * @return If there's a malloc failure it prints
 * an error message and returns NULL.
 * Otherwise it returns the full command's path.
 */
static char	*ft_strjoin_cmd_path(char *path, char *cmd_name)
{
	char	*cmd_path;

	if (!path || !cmd_name)
		return (NULL);
	cmd_path = ft_strdup(path);
	if (!cmd_path)
		return (handle_perror("ft_strjoin_cmd_path"), NULL);
	if (append_suffix(&cmd_path, "/", false))
		return (handle_perror("ft_strjoin_cmd_path"), NULL);
	cmd_path = ft_strjoin_fs1(&cmd_path, cmd_name);
	if (!cmd_path)
		return (handle_perror("ft_strjoin_cmd_path"), NULL);
	return (cmd_path);
}
