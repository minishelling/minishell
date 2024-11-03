/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   pipe_subtree.c                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: lprieri <lprieri@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/10/31 13:33:13 by lprieri       #+#    #+#                 */
/*   Updated: 2024/10/31 13:33:14 by lprieri       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

#define READ_END 0
#define WRITE_END 1

#define ALL 0
#define LAST_STATUS 1

static void	handle_pipe_left_node(t_shell *shell, t_tree *tree_node, int fd[2]);
static void	handle_pipe_right_node(t_shell *shell,
				t_tree *tree_node, int fd[2]);
static void	close_fds(int fd1, int fd2);
static void	handle_status(t_shell *shell, pid_t last_pid, int *last_status);

/**
 * @brief Executes a subtree where the root is a pipe operator.
 * 
 * This function sets up a pipe and forks the process twice to 
 * execute the left and right child nodes of the pipe operator. 
 * It first creates a pipe, forks the process for the left node, 
 * then waits for it to complete before forking for the right node. 
 * It ensures that all processes and subprocesses are waited upon 
 * before returning.
 * 
 * @param shell A pointer to the shell structure that contains the 
 *              current execution environment.
 * @param tree_node A pointer to the tree node representing the pipe 
 *                  operator, which has left and right child nodes to execute.
 * 
 * @return Returns the exit status of the right child node process. 
 * If there was an error during the pipe creation, forking, or closing 
 * file descriptors, it exits the main process with `EXIT_FAILURE` (1). 
 * If the left node process exits with a signal, it returns the WEXITSTATUS 
 * of that left node instead.
 * 
 * @note: It is assumed that each node will exit normally via an exit function, 
 * and checks for normal versus signal termination are handled in the executor.
 */
int	handle_pipe_subtree(t_shell *shell, t_tree *tree_node)
{
	int		fd[2];
	int		status;
	pid_t	left_node_pid;
	pid_t	right_node_pid;

	if (pipe(fd) == ERROR)
		clean_nicely_and_exit(shell, EXIT_FAILURE);
	left_node_pid = fork();
	if (left_node_pid == ERROR)
		clean_nicely_and_exit(shell, EXIT_FAILURE);
	else if (left_node_pid == 0)
		handle_pipe_left_node(shell, tree_node, fd);
	right_node_pid = fork();
	if (right_node_pid == ERROR)
		clean_nicely_and_exit(shell, EXIT_FAILURE);
	else if (right_node_pid == 0)
		handle_pipe_right_node(shell, tree_node, fd);
	close_fds(fd[READ_END], fd[WRITE_END]);
	handle_status(shell, right_node_pid, &status);
	return (WEXITSTATUS(status));
}

/**
 * @brief Executes the left child node of a pipe subtree.
 * 
 * This function sets up the left node of a pipe structure to write 
 * its output to the pipe's `WRITE_END` (1). It duplicates the `WRITE_END` 
 * file descriptor to STDOUT, closes both pipe file descriptors, and 
 * then executes the command associated with the left node.
 * 
 * @param shell A pointer to the shell structure.
 * @param tree_node A pointer to the current tree node representing the 
 *                  pipe. This node contains the command to be executed 
 *                  in the left subtree.
 * @param fd An array containing the pipe's file descriptors, where 
 *         `fd[0]` is the `READ_END` and `fd[1]` is the `WRITE_END` of the pipe.
 * 
 * @return void
 */
static void	handle_pipe_left_node(t_shell *shell, t_tree *tree_node, int fd[2])
{
	int	status;

	if (dup2(fd[WRITE_END], STDOUT_FILENO) == ERROR)
	{
		perror("dup2");
		clean_nicely_and_exit(shell, EXIT_FAILURE);
	}
	close_fds(fd[READ_END], fd[WRITE_END]);
	status = traverse_tree_and_execute(shell, tree_node->left, tree_node, 0);
	clean_nicely_and_exit(shell, status);
}

/**
 * @brief Executes the right child node of a pipe subtree.
 * 
 * This function is designed to set up the right node of a pipe structure 
 * to read input from the pipe's `READ_END` (0). It duplicates the `READ_END` 
 * file descriptor to STDIN, closes both pipe file descriptors, and then 
 * executes the command associated with the right node.
 * 
 * @param shell A pointer to the shell structure.
 * @param tree_node A pointer to the current tree node representing the 
 *                  pipe. This node contains the command to be executed 
 *                  in the right subtree.
 * @param fd An array containing the pipe's file descriptors, where 
 *          `fd[0]` is the `READ_END` and `fd[1]` is the `WRITE_END` of the pipe.
 * 
 * @return void
 */
static void	handle_pipe_right_node(t_shell *shell, t_tree *tree_node, int fd[2])
{
	int	status;

	if (dup2(fd[READ_END], STDIN_FILENO) == ERROR)
	{
		perror("dup2");
		clean_nicely_and_exit(shell, EXIT_FAILURE);
	}
	close_fds(fd[WRITE_END], fd[READ_END]);
	status = traverse_tree_and_execute(shell, tree_node->right, tree_node, 0);
	clean_nicely_and_exit(shell, status);
}

/**
 * @brief Closes the specified file descriptors.
 * 
 * This function attempts to close two file descriptors provided as 
 * arguments. If closing any of the file descriptors fails, an error 
 * message is printed to standard error.
 * 
 * @param fd1 The first file descriptor to close. If it is set to 
 *             `ERROR`, it will be ignored.
 * @param fd2 The second file descriptor to close. If it is set to 
 *             `ERROR`, it will be ignored.
 * 
 * @return void
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

/**
 * @brief Handles the termination status of a child process,
 * checking for exit signals and memory failure conditions.
 * 
 * This function waits for the specified child process to terminate and checks 
 * its exit status. If the child exited due to the signals SIGINT or SIGQUIT, 
 * the global variable `g_signalcode` is updated with the corresponding signal 
 * code. It also continues to wait for any other child processes that may have 
 * exited and updates the signal code accordingly.
 * 
 * Additionally, if the child process exits with a memory failure code, 
 * the function cleans up and exits the shell with the appropriate status code.
 * 
 * @param shell A pointer to the shell structure.
 * @param last_pid The process ID of the last child process that was executed.
 * @param last_status A pointer to an integer where the exit status of the 
 *                    last child process will be stored.
 * 
 * @return void
 * 
 * @note The function handles various exit statuses, updating the global signal 
 *       code for specific signals and managing memory failure scenarios by 
 *       terminating the shell when such an exit status is encountered.
 */
static void	handle_status(t_shell *shell, pid_t last_pid, int *last_status)
{
	int	status;

	waitpid(last_pid, last_status, 0);
	if (*last_status == EXIT_SIGINT || *last_status == EXIT_SIGQUIT)
		g_signalcode = *last_status - EXIT_SIGNAL_CODE;
	else if (*last_status >> 8 == EXIT_SIGINT \
		|| *last_status >> 8 == EXIT_SIGQUIT)
		g_signalcode = *last_status >> 8;
	else if (*last_status == EXIT_MEM_FAILURE)
		clean_nicely_and_exit(shell, EXIT_MEM_FAILURE);
	else if (*last_status >> 8 == EXIT_MEM_FAILURE)
		clean_nicely_and_exit(shell, EXIT_MEM_FAILURE);
	while (wait(&status) != ERROR)
	{
		if (status == EXIT_SIGINT || status == EXIT_SIGQUIT)
			g_signalcode = status - EXIT_SIGNAL_CODE;
		else if (status >> 8 == EXIT_SIGINT || status >> 8 == EXIT_SIGQUIT)
			g_signalcode = status >> 8;
		else if (status == EXIT_MEM_FAILURE)
			clean_nicely_and_exit(shell, EXIT_MEM_FAILURE);
		else if (status >> 8 == EXIT_MEM_FAILURE)
			clean_nicely_and_exit(shell, EXIT_MEM_FAILURE);
	}
}
