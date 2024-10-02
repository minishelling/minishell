#include "../include/minishell.h"

typedef struct s_pipex
{
	pid_t	pid1;
	pid_t	pid2;
	int		fd[2];
	int		infile_fd;
	int		outfile_fd;
	char	**cmd1;
	char	**cmd2;
	char	**cmd3;
	char	**possible_paths;
	char	*path;
}	t_pipex;

static void	close_fds(int fd1, int fd2)
{
	if (fd1 != -1)
		if (close(fd1) == -1)
			perror("close fd1");
	if (fd2 != -1)
		if (close(fd2) == -1)
			perror("close fd2");
}

static int	handle_pipe_left_node(t_shell *shell, t_tree *tree_node, int fd[2])
{
	int	status;
	if (dup2(fd[WRITE_END], STDOUT_FILENO) == -1)
	{
		perror("dup2");                                                                                         
		exit(EXIT_FAILURE);
	}
	close_fds(fd[READ_END], fd[WRITE_END]);
	status = pre_execute(shell, tree_node->left, tree_node, 0);
	exit(status); //Should probably take another variable as arg, and pass the return of pre_execute there.
}

static int	handle_pipe_right_node(t_shell *shell, t_tree *tree_node, int fd[2])
{
	if (dup2(fd[READ_END], STDIN_FILENO) == -1)
	{
		perror("dup2");
		exit(EXIT_FAILURE);
	}
	close_fds(fd[WRITE_END], fd[READ_END]);
	exit(pre_execute(shell, tree_node->right, tree_node, 0));
}

int	handle_pipe_subtree(t_shell *shell, t_tree *tree_node)
{
	int		fd[2];
	int		status;
	pid_t	left_node_pid;
	pid_t	right_node_pid;

	if (pipe(fd) == -1)
		exit(EXIT_FAILURE);
	left_node_pid = fork();
	if (left_node_pid == -1)
		exit(EXIT_FAILURE);
	else if (left_node_pid == 0)
		handle_pipe_left_node(shell, tree_node, fd);
	waitpid(left_node_pid, &status, 0);
	right_node_pid = fork();
	if (right_node_pid == -1)
		exit(EXIT_FAILURE);
	else if (right_node_pid == 0)
		handle_pipe_right_node(shell, tree_node, fd);
	waitpid(left_node_pid, &status, 0);
	close_fds(fd[READ_END], fd[WRITE_END]);
	while (wait(NULL) != -1)
		;
	return (status);
}

// void	free_array_pipex(char **arr)
// {
// 	int	i;

// 	i = 0;
// 	while (arr && arr[i])
// 	{
// 		free(arr[i]);
// 		i++;
// 	}
// 	free(arr);
// }

// void	clean_nicely(t_pipex *pipex)
// {
// 	if (pipex->cmd1)
// 		free_array_pipex(pipex->cmd1);
// 	if (pipex->cmd2)
// 		free_array_pipex(pipex->cmd2);
// 	free_array_pipex(pipex->possible_paths);
// }

// char	*ft_strjoin_fs1pipex(char *s1, char const *s2)
// {
// 	char	*dst;
// 	size_t	ls1;
// 	size_t	ls2;

// 	if (!s1)
// 		return (NULL);
// 	if (!s2)
// 		return (s1);
// 	ls1 = ft_strlen(s1);
// 	ls2 = ft_strlen(s2);
// 	dst = malloc(sizeof(char) * (ls1 + ls2 + 1));
// 	if (!dst)
// 		return (free(s1), NULL);
// 	ft_strlcpy(dst, s1, (ls1 + 1));
// 	ft_strlcat(dst, s2, (ls1 + ls2 + 1));
// 	free(s1);
// 	return (dst);
// }

// int find_correct_path(t_pipex *pipex, char **cmd)
// {
// 	char *temp;
// 	int i = 0;

// 	if (!pipex->possible_paths || *cmd == NULL || cmd == NULL)
// 		return (0);

// 	while (pipex->possible_paths[i])
// 	{
// 		//fprintf(stderr, "Checking path: %s/%s\n", pipex->possible_paths[i], *cmd);
		
// 		// Construct full path
// 		temp = ft_strjoin(pipex->possible_paths[i], "/");
// 		char *full_cmd = ft_strjoin(temp, *cmd);
// 		free(temp);
// 		if (!full_cmd)
// 		{
// 			write(2, "error: Unable to allocate dynamic memory\n", 41);
// 			clean_nicely(pipex);
// 			exit(EXIT_FAILURE);
// 		}
		
// 		// Check access to the path
// 		if (access(full_cmd, X_OK | F_OK) == 0)
// 		{
// 			free(*cmd);
// 			*cmd = full_cmd;
// 			return (1);
// 		}

// 		free(full_cmd);
// 		i++;
// 	}
// 	fprintf(stderr, "Command not found in any path\n");
// 	return (0);
// }

// void first_child(t_pipex *pipex, t_shell *shell, t_tree *tree_node, char **envp)
// {
// 	char **argv;
// 	int is_found = 0;

// 	argv = make_cmd(shell, tree_node->left->start_token, tree_node->left->end_token)->args;
// 	fprintf(stderr, "cmd1[0] %s\n", argv[0]);
// 	if (access(argv[0], X_OK | F_OK) == 0)
// 		is_found = 1;
// 	else
// 	{
// 		is_found = find_correct_path(pipex, argv);
// 	}
// 	close (pipex->fd[READ_END]);

// 	if (dup2(pipex->fd[WRITE_END], STDOUT_FILENO) == -1)
// 	{
// 		perror("dup2 failed for stdout");
// 		clean_nicely(pipex);
// 	}

// 	if (close(pipex->fd[WRITE_END]) == -1)
// 		perror("close");

// 	if (is_found)
// 	{
// 		fprintf (stderr, "execving cmd1 %s\n", argv[0]);
// 		execve(argv[0], argv, envp);
// 		exit(EXIT_FAILURE);
// 	}

// 	ft_putstr_fd(pipex->cmd1[0], 2);
// 	write(2, ": command not found\n", 20);
// 	clean_nicely(pipex);
// 	exit(127);
// }

// void second_child(t_pipex *pipex, t_shell *shell, t_tree *tree_node, char **envp)
// {
// 	int status;
// 	char **argv;
// 	if (dup2(pipex->fd[READ_END], STDIN_FILENO) == -1)
// 	{
// 		fprintf(stderr, "dup2 failed for stdin");
// 		clean_nicely(pipex);
// 		exit(EXIT_FAILURE);
// 	}
// 	close_fds(pipex->fd[READ_END], pipex->fd[WRITE_END]);
	
// 	pid_t pid3 = fork();
// 	if (pid3 == 0)
// 	{
// 		argv = make_cmd(shell, tree_node->right->left->start_token, tree_node->right->left->end_token)->args;
// 		fprintf(stderr, "cmd2[0] %s\n", argv[0]);
// 		int is_found = find_correct_path(pipex, argv);
// 		fprintf (stderr, "is found? %d\n", is_found);
// 		if (is_found)
// 		{
// 			fprintf (stderr, "execving cmd2 %s\n", argv[0]);
// 			execve(argv[0], argv, envp);
// 			exit(EXIT_FAILURE);
// 		}
// 		else
// 		{
// 			fprintf (stderr, "failed cmd2\n");
// 			exit(EXIT_FAILURE);
// 		}
// 	}
	
// 	// Wait for the first child process to finish
// 	if (waitpid(pid3, &status, 0) == -1)
// 	{
// 		perror("waitpid failed");
// 	}
	
// 	// Check if the child process terminated normally
// 	if (WIFEXITED(status))
// 	{
// 		fprintf(stderr, "status of cmd2 is %d\n", WEXITSTATUS(status));
// 	}
// 	else
// 	{
// 		fprintf(stderr, "cmd2 did not terminate normally\n");
// 	}

// 	pid_t pid4 = fork();
// 	if (pid4 == 0)
// 	{
// 		argv = make_cmd(shell, tree_node->right->right->start_token, tree_node->right->right->end_token)->args;
// 		fprintf(stderr, "cmd3[0] %s\n", argv[0]);
// 		int is_found = find_correct_path(pipex, argv);
// 		if (is_found)
// 		{
// 			fprintf (stderr, "execving cmd3 %s\n", argv[0]);
// 			execve(argv[0], argv, envp);
// 		}
// 		else
// 		{
// 			fprintf(stderr, "failed cmd3\n");
// 			exit(EXIT_FAILURE);
// 		}
// 	}
// 	wait(NULL);
// 	exit(EXIT_SUCCESS);
// }

// char *locate_env_path(t_env **envp)
// {
// 	t_env *current = *envp;  // Start at the head of the list

// 	// Traverse the linked list
// 	while (current)
// 	{
// 		// Compare the key portion with "PATH"
// 		if (ft_strncmp("PATH", current->keyvalue, 4) == 0)
// 			return (current->keyvalue);  // Return the actual string
		
// 		current = current->next;  // Move to the next node
// 	}
// 	return (NULL);  // If no "PATH" variable was found
// }

// void find_possible_paths(t_pipex *pipex, t_env **envp)
// {
// 	char *envp_path;

// 	envp_path = locate_env_path(envp);
// 	if (!envp_path)
// 		return;

// 	// Split the path and store it in pipex->possible_paths
// 	pipex->possible_paths = ft_split(envp_path + 5, ':');
	
// 	// Check if ft_split was successful
// 	if (!pipex->possible_paths)
// 	{
// 		// Handle error (e.g., log or return)
// 		return;
// 	}
// }


// void free_array(char **arr)
// {
// 	int i = 0;

// 	while (arr && arr[i])
// 	{
// 		free(arr[i]);
// 		i++;
// 	}
// 	free(arr);
// }

// // int create_pipe(t_pipex *pipex, t_shell *shell, t_tree *tree_node, t_env **envp)
// int create_pipe(t_pipex *pipex, t_shell *shell, t_tree *tree_node, char **envp)
// {
// 	if (pipe(pipex->fd) < 0)
// 		clean_nicely(pipex);

// 	pipex->pid1 = fork();
// 	if (pipex->pid1 == -1)
// 	{
// 		perror("First fork");
// 		clean_nicely(pipex);
// 		exit(EXIT_FAILURE);
// 	}
// 	else if (pipex->pid1 == 0)
// 		first_child(pipex, shell, tree_node, envp);

// 	pipex->pid2 = fork();
// 	if (pipex->pid2 == -1)
// 	{
// 		perror("Second fork");
// 		clean_nicely(pipex);
// 		exit(EXIT_FAILURE);
// 	}
// 	else if (pipex->pid2 == 0)
// 		second_child(pipex, shell, tree_node, envp);
// 	close_fds(pipex->fd[0], pipex->fd[1]);
// 	while (wait(NULL) != -1)
// 		;
// 	return (SUCCESS);
// }

// int handle_pipe_subtree(t_shell *shell, t_tree *tree_node)
// {
// 	t_pipex pipex;
// 	char **env_array;
	
// 	env_array = create_env_array(shell->env_list);
// 	ft_bzero(&pipex, sizeof(t_pipex));
// 	find_possible_paths(&pipex, &shell->env_list);
// 	create_pipe(&pipex, shell, tree_node, env_array);
// 	clean_nicely(&pipex);
// 	return (0);
// }
