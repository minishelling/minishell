#include "../../include/minishell.h"

static int	handle_pipe_left_node(t_shell *shell, t_tree *tree_node, int fd[2]);
static int	handle_pipe_right_node(t_shell *shell, t_tree *tree_node, int fd[2]);
static void close_fds(int fd1, int fd2);

int	handle_pipe_subtree(t_shell *shell, t_tree *tree_node)
{
	int		fd[2];
	int		status;
	pid_t	left_node_pid;
	pid_t	right_node_pid;

	if (pipe(fd) == ERROR)
		exit(EXIT_FAILURE);
	left_node_pid = fork();
	if (left_node_pid == ERROR)
		exit(EXIT_FAILURE);
	else if (left_node_pid == 0)
		handle_pipe_left_node(shell, tree_node, fd);
	waitpid(left_node_pid, &status, 0);
	right_node_pid = fork();
	if (right_node_pid == ERROR)
		exit(EXIT_FAILURE);
	else if (right_node_pid == 0)
		handle_pipe_right_node(shell, tree_node, fd);
	close_fds(fd[READ_END], fd[WRITE_END]);
	waitpid(right_node_pid, &status, 0);
	while (wait(NULL) != ERROR)
		;
	return (status);
}

static int	handle_pipe_left_node(t_shell *shell, t_tree *tree_node, int fd[2])
{
	int	status;

	if (dup2(fd[WRITE_END], STDOUT_FILENO) == ERROR)
	{
		perror("dup2");                                                                                         
		exit(EXIT_FAILURE);
	}
	close_fds(fd[READ_END], fd[WRITE_END]);
	status = pre_execute(shell, tree_node->left, tree_node, 0);
	exit(status);
}

static int	handle_pipe_right_node(t_shell *shell, t_tree *tree_node, int fd[2])
{
	int	status;
	if (dup2(fd[READ_END], STDIN_FILENO) == ERROR)
	{
		perror("dup2");
		exit(EXIT_FAILURE);
	}
	close_fds(fd[WRITE_END], fd[READ_END]);
	status = pre_execute(shell, tree_node->right, tree_node, 0);
	exit(status);
}

static void close_fds(int fd1, int fd2)
{
	if (fd1 != ERROR)
	{
		if (close(fd1) == ERROR)
			perror("close fd1");
	}
	if (fd2 != ERROR)
	{
		if (close(fd2) == ERROR)
			perror("close fd2");
	}
}
