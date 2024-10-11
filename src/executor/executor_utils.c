#include "../../include/minishell.h"

/**
 * @brief Checks if the given command name is a valid built-in.
 * 
 * @param cmd_name The command name to be checked if is a built-in.
 * @return The builtin code if it matches the name of a built-in.
 * Returns the code NON_BUILTIN if it doesn't.
 */
t_builtin	check_builtin(char *cmd_name)
{
	char		*builtins[] = {"echo", "cd", "pwd", "export",
						"declare", "unset", "env", "exit", NULL};
	t_builtin	builtin_code;

	builtin_code = ECHO;
	while (builtins[builtin_code])
	{
		if (!ft_strncmp(cmd_name, builtins[builtin_code],
			max_len(cmd_name, builtins[builtin_code])))
			return (builtin_code);
		builtin_code++;
	}
	return (NON_BUILTIN);
}

/**
 * @brief Counts the number of commands in a command list.
 * 
 * @param head The head node of the commands list.
 * @return Returns 0 if the head node is empty or if the cmd arguments are NULL,
 * otherwise it returns the number of nodes there are in the commands list.
 */
size_t	count_cmds(t_cmd *head)
{
	size_t	count;

	if (!head || !head->args)
		return (0);
	count = 0;
	while (head && head->args)
	{
		count++;
		head = head->next;
	}
	return (count);
}

/**
 * @brief Duplicates the current STDIN and STDOUT into the variable given as parameter.
 * 
 * @param backup The variable to hold the duplicates of STDIN and STDOUT.
 * @return SUCCESS if it duplicates both standard file descriptors successfully.
 * If any of them fails to duplicate it prints an error and returns FAILURE.
 */
t_ecode	create_std_backup(int backup[2])
{
	backup[STDIN_FILENO] = dup(STDIN_FILENO);
	if (backup[STDIN_FILENO] == -1)
	{
		handle_perror("dup");
		return (FAILURE);
	}
	backup[STDOUT_FILENO] = dup(STDOUT_FILENO);
	if (backup[STDOUT_FILENO] == -1)
	{
		handle_perror("dup");
		return (FAILURE);
	}
	return (SUCCESS);
}

/**
 * @brief Duplicates oldfd into newfd, and closes oldfd right after.
 * 
 * @param oldfd The file descriptor to put in the place of newfd.
 * @param newfd The file descriptor that we want replaced.
 * @return If dup or close fails, it prints an error and returns FAILURE.
 * Returns SUCCESS otherwise.
 */
t_ecode	dup_and_close(int oldfd, int newfd)
{
	int	status;

	status = dup2(oldfd, newfd);
	if (status == -1)
	{
		handle_perror("dup2");
		return (FAILURE);
	}
	status = close(oldfd);
	if (status == -1)
	{
		handle_perror("close");
		return (FAILURE);
	}
	return (SUCCESS);
}
