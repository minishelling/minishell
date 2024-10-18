#include "../../include/minishell.h"

static void close_fds(int fd1, int fd2)
{
	if (fd1 != -1)
	{
		if (close(fd1) == -1)
			perror("close fd1");
	}
	if (fd2 != -1)
	{
		if (close(fd2) == -1)
			perror("close fd2");
	}
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
	int	status;
	if (dup2(fd[READ_END], STDIN_FILENO) == -1)
	{
		perror("dup2");
		exit(EXIT_FAILURE);
	}
	close_fds(fd[WRITE_END], fd[READ_END]);
	status = pre_execute(shell, tree_node->right, tree_node, 0);
	exit(status);
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
	close_fds(fd[READ_END], fd[WRITE_END]);
	waitpid(right_node_pid, &status, 0);
	while (wait(NULL) != -1)
		;
	return (status);
}
