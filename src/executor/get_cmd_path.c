#include "../../include/minishell.h"

static char	*check_name_as_path(t_shell *shell, char *cmd_name);
static char	*check_path_access(t_shell *shell, \
				char *current_path, char *cmd_name);
static char	*ft_strjoin_cmd_path(char *path, char *cmd_name);

/**
 * @brief Retrieves the full path of a command by examining the PATH 
 *        variable in the environment. If no valid path is found, it 
 *        checks if the command name itself is a valid path.
 * 
 * This function first checks if the command name is provided and 
 * whether it refers to a relative or absolute path. If it does, it 
 * uses the `check_name_as_path` function to validate it. If the 
 * command name is not a direct path, it retrieves the PATH variable 
 * from the environment, splits it into individual paths, and checks 
 * each one to see if it contains the executable. If a valid path 
 * is found, it is returned; however if a PATH env variable has not
 * been found, the command name is checked as a potential path.
 * 
 * @param shell A pointer to the shell structure that holds the 
 *              environment variables and state.
 * @param cmd_name The name of the command for which to find the 
 *                 full executable path.
 * 
 * @return Returns the full path to the command's executable file 
 *         on success. If no valid path is found, it returns NULL.
 */
char	*get_cmd_path(t_shell *shell, char *cmd_name)
{
	t_env	*path_node;
	char	*cmd_path;
	char	**paths;
	int		i;

	if (!cmd_name || !cmd_name[0])
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
			return (ft_free_2d((void ***) &paths), cmd_path);
		i++;
	}
	ft_free_2d((void ***) &paths);
	return (NULL);
}

/**
 * @brief Checks whether the command name corresponds to a valid path.
 * 
 * This function examines the specified command name to determine if it 
 * points to an executable file. It uses the `stat` system call to gather 
 * information about the file. The function handles various scenarios, 
 * including cases where the file is a directory or cannot be executed.
 * 
 * @param shell A pointer to the shell structure used for managing 
 *              exit codes and error states.
 * @param cmd_name The name of the command to check as a path.
 * 
 * @return Returns the command name if it is found and executable. If 
 *         `stat` fails, it sets the exit code to 0 and returns NULL. If 
 *         the file is a directory, it sets `errno` to EISDIR. For files 
 *         that are found but cannot be executed, it sets the exit code 
 *         to 126 and returns NULL. If the file is not found, it sets 
 *         the exit code to 127 and returns NULL.
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
 * @brief Concatenates the command name to the current path and checks if the 
 *        resulting path is executable.
 * 
 * This function constructs the full path by concatenating the command name 
 * to the specified current path. It then verifies whether this path points 
 * to an executable file. 
 * 
 * @param shell A pointer to the shell structure.
 * @param current_path The current path to which the cmd name will be appended.
 * @param cmd_name The command name to check for executability.
 * 
 * @return Returns the full path if it is executable. If the path leads to a 
 *         valid file but is not executable, it returns NULL and sets the error 
 *         code to 126. If the path is invalid or an error occurs, it also 
 *         returns NULL.
 */
static char	*check_path_access(t_shell *shell, \
				char *current_path, char *cmd_name)
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
 * @brief Concatenates a command name to the specified path.
 * 
 * This function creates a new string that consists of the given path 
 * followed by the command name, ensuring proper formatting. 
 * If either the path or the command name is NULL or empty, it returns NULL.
 * 
 * @param path The current path value to which the command name will be appended.
 * @param cmd_name The command name to concatenate to the path.
 * 
 * @return Returns a pointer to the newly allocated string containing 
 *         the full command path if successful. If there is a memory 
 *         allocation failure, it prints an error message and returns NULL.
 */
static char	*ft_strjoin_cmd_path(char *path, char *cmd_name)
{
	char	*cmd_path;

	if (!path || !cmd_name || !cmd_name[0])
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
