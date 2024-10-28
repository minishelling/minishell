#include "../../include/minishell.h"

/**
 * @brief Checks if a given command name corresponds to a valid builtin command.
 * 
 * This function compares the provided command name against a list of known 
 * built-in commands. If a match is found, it returns the corresponding 
 * builtin code; otherwise, it returns a code indicating that the command 
 * is not a builtin.
 * 
 * @param[in] cmd_name The command name to be checked for validity as a builtin.
 * 
 * @return t_builtin Returns the builtin code if a match is found. 
 * Returns `NON_BUILTIN` if the command name does not correspond to any 
 * recognized builtin command.
 */
t_builtin	check_builtin(char *cmd_name)
{
	char		*builtins[BUILTIN_COUNT];
	t_builtin	builtin_code;

	builtins[ECHO] = "echo";
	builtins[CD] = "cd";
	builtins[PWD] = "pwd";
	builtins[EXPORT] = "export";
	builtins[DECLARE] = "declare";
	builtins[UNSET] = "unset";
	builtins[ENV] = "env";
	builtins[EXIT] = "exit";
	builtins[NON_BUILTIN] = NULL;
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
 * @brief Counts the number of commands in a linked list of commands.
 * 
 * This function traverses a linked list of command nodes, counting each node 
 * that contains a valid command (i.e., `args` is not NULL).
 * 
 * @param[in] head The head node of the linked list of commands.
 * 
 * @return size_t The number of nodes with valid commands in the list. 
 * Returns 0 if the head node is NULL or if the `args` field of the head node 
 * is NULL.
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
 * @brief Duplicates the current STDIN and STDOUT file descriptors for backup.
 * 
 * This function saves copies of the standard input and output file descriptors 
 * (STDIN and STDOUT) into the provided array. This is typically used to allow 
 * temporary redirection of these file descriptors, with the ability to restore 
 * them later.
 * 
 * @param[out] backup An array of two integers where the duplicated file 
 *                    descriptors will be stored:
 *                    - `backup[0]` for the STDIN duplicate.
 *                    - `backup[1]` for the STDOUT duplicate.
 * 
 * @return t_ecode Returns:
 * - `SUCCESS` if both file descriptors are duplicated successfully.
 * - `FAILURE` if either duplication fails, in which case an error message 
 *   is printed.
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
 * @brief Replaces a file descriptor with another and closes the original.
 * 
 * This function duplicates `oldfd` onto `newfd`, making `newfd` refer to 
 * the same open file description as `oldfd`, and then closes `oldfd` to 
 * prevent resource leaks.
 * 
 * @param[in] oldfd The file descriptor to duplicate onto `newfd`. This file 
 *                  descriptor is closed after duplication.
 * @param[in] newfd The file descriptor to be replaced by `oldfd`.
 * 
 * @return t_ecode Returns:
 * - `FAILURE` if `dup2` or `close` fails, after printing an error message.
 * - `SUCCESS` if both operations complete without error.
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
