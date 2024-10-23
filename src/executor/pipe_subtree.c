#include "../../include/minishell.h"

static void	handle_pipe_left_node(t_shell *shell, t_tree *tree_node, int fd[2]);
static void	handle_pipe_right_node(t_shell *shell,
				t_tree *tree_node, int fd[2]);
static void	close_fds(int fd1, int fd2);

/**
 * @brief Executes a subtree where the root is a pipe operator.
 * 
 * This function creates a pipe and then forks the process twice,
 * but before forking for a second time it waits until the execution
 * of the first process is finished.
 * The first process is supposed to execute the left node of the tree,
 * while the second is suposed to execute the right node.
 * Before returning, all processes and subprocesses are waited to terminate.
 * 
 * @param shell A pointer to the shell structure.
 * @param tree_node A pointer to the tree node to be executed.
 * 
 * @return If there was an error in piping, forking or closing a fd,
 * then it exits the main process with the exit code EXIT_FAILURE (1).
 * If the left node process has exited with a signal code, then
 * the execution stops and it returns the WEXITSTATUS of that left node.
 * Otherwise it returns the WEXITSTATUS of the right node.
 * 
 * Note that a check for if the node processes have exited normally
 * (with an exit function) or by a signal is not needed since each node
 * will always exit with the exit function, and the exit code of execution,
 * and the check is provided there (in the executor).
 */
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
	right_node_pid = fork();
	if (right_node_pid == ERROR)
		exit(EXIT_FAILURE);
	else if (right_node_pid == 0)
		handle_pipe_right_node(shell, tree_node, fd);
	close_fds(fd[READ_END], fd[WRITE_END]);
	waitpid(right_node_pid, &status, 0);
	while(wait(NULL) != ERROR)
		;
	return (WEXITSTATUS(status));
}

/**
 * @brief Executes the left node a pipe subtree.
 * 
 * This node is supposed to write the command's output into
 * the WRITE_END (1) of the pipe. Thus it duplicates the fd[WRITE_END]
 * into STDOUT, and then closes both pipe file descriptors before executing.
 * 
 * @param shell A pointer to the shell structure.
 * @param tree_node A pointer to the parent tree node (the pipe node).
 * @param fd The pipe's file descriptors.
 * 
 * @return void
 */
static void	handle_pipe_left_node(t_shell *shell, t_tree *tree_node, int fd[2])
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

/**
 * @brief Executes the right node a pipe subtree.
 * 
 * This node is supposed to read from the pipe READ_END (0)
 * for the command's input. Thus it duplicates the fd[READ_END]
 * into STDIN, and then closes both pipe file descriptors before executing.
 * 
 * @param shell A pointer to the shell structure.
 * @param tree_node A pointer to the parent tree node (the pipe node).
 * @param fd The pipe's file descriptors.
 * 
 * @return void
 */
static void	handle_pipe_right_node(t_shell *shell, t_tree *tree_node, int fd[2])
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

/**
 * @brief Closes the given file descriptors.
 * 
 * @param fd1 File descriptor 1.
 * @param fd2 File descriptor 2.
 * 
 * @return void
 * If there was an error closing a file descriptor it prints an error message.
 */
static void	close_fds(int fd1, int fd2)
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
