#include "../../include/minishell.h"

static t_ecode	open_current_redir(t_redir_id redir_id,
					char *redir_file, int *fd);
static t_ecode	replace_redir_fd(t_cmd *cmd, t_redir *redir);
static t_ecode	close_and_replace(int replacement, int *oldfd);

/**
 * @brief Opens all input and output redirections for a given command.
 * 
 * This function iterates through the list of redirection structures associated 
 * with the current command, attempting to open each redirection and replace 
 * the corresponding file descriptor. If any operation fails, it returns an error 
 * code. 
 * 
 * @param[in] current_cmd A pointer to the command structure containing the 
 *                        redirections to be opened.
 * 
 * @return t_ecode Returns:
 * - `FAILURE` if any redirection fails to open, if there is a failure 
 *   in duplicating file descriptors, or if `current_cmd` is NULL.
 * - `SUCCESS` if all redirections are opened successfully.
 */
t_ecode	open_redirections(t_cmd *current_cmd)
{
	t_redir	*current_redir;

	if (!current_cmd)
		return (FAILURE);
	current_redir = current_cmd->redir;
	while (current_redir)
	{
		if (open_current_redir(current_redir->redir_id,
				current_redir->file, &current_redir->fd) != SUCCESS)
			return (FAILURE);
		if (replace_redir_fd(current_cmd, current_redir) != SUCCESS)
			return (FAILURE);
		current_redir = current_redir->next;
	}
	return (SUCCESS);
}

/**
 * @brief Opens the specified redirection based on its type.
 * 
 * This function determines the type of redirection (e.g., input, output, 
 * heredoc) and opens the corresponding file or uses an existing file 
 * descriptor for a heredoc. If the redirection is of type HEREDOC, it 
 * converts the provided string to an integer instead of opening a file, 
 * as the heredoc content is already in an open file.
 * 
 * @param[in] redir_id The type of redirection to be opened (e.g., 
 *                     HEREDOC, IN, OUT, APPEND).
 * @param[in] redir_file The path of the file to be opened for the 
 *                       specified redirection, or a fd in string
 * 						 format in the case of a HEREDOC.
 * @param[out] fd A pointer to an integer where the opened file 
 *                descriptor will be stored. This will be updated 
 *                with the result of the open operation.
 * 
 * @return t_ecode Returns:
 * - `FAILURE` if an error occurs during opening the file or 
 *   converting the heredoc fd, after printing an error message.
 * - `SUCCESS` if the redirection is processed successfully.
 */
static t_ecode	open_current_redir(t_redir_id redir_id,
					char *redir_file, int *fd)
{
	if (!redir_file)
		return (SUCCESS);
	if (redir_id == HERE)
		*fd = ft_atoi(redir_file);
	else if (redir_id == IN)
		*fd = open(redir_file, O_RDONLY);
	else if (redir_id == OUT)
		*fd = open(redir_file, O_WRONLY | O_TRUNC | O_CREAT, 0644);
	else if (redir_id == APP)
		*fd = open(redir_file, O_WRONLY | O_APPEND | O_CREAT, 0644);
	if (*fd == ERROR)
	{
		handle_perror(redir_file);
		return (FAILURE);
	}
	return (SUCCESS);
}

/**
 * @brief Replaces the latest input and output redirection variables with 
 * the current redirection values.
 * 
 * This function updates the `latest_in` and `latest_out` fields of the 
 * command structure based on the current redirection. If the current 
 * redirection is invalid (indicated by an error file descriptor), it 
 * closes the previous redirection and sets it to the error number. 
 * Otherwise, it attempts to duplicate the current redirection into the 
 * respective latest variable and closes the current redirection file 
 * descriptor afterwards.
 * 
 * @param[in] cmd A pointer to the current command's structure that 
 *                 holds the latest redirection variables.
 * @param[in] redir A pointer to the current command's redirection 
 *                   structure that contains the redirection details.
 * 
 * @return t_ecode Returns:
 * - `FAILURE` if the current redirection file descriptor is invalid (-1) 
 *   or if duplicating the file descriptor fails.
 * - `SUCCESS` if the redirection is successfully replaced.
 */
static t_ecode	replace_redir_fd(t_cmd *cmd, t_redir *redir)
{
	t_ecode	status;

	status = SUCCESS;
	if (redir->redir_id == HERE || redir->redir_id == IN)
	{
		if (redir->fd == ERROR)
			return (close_and_replace(redir->fd, &cmd->latest_in));
		else if (cmd->latest_in == STDIN_FILENO)
			cmd->latest_in = redir->fd;
		else
			status = dup_and_close(redir->fd, cmd->latest_in);
	}
	else if (redir->redir_id == OUT || redir->redir_id == APP)
	{
		if (redir->fd == ERROR)
			return (close_and_replace(redir->fd, &cmd->latest_out));
		else if (cmd->latest_out == STDOUT_FILENO)
			cmd->latest_out = redir->fd;
		else
			status = dup_and_close(redir->fd, cmd->latest_out);
	}
	if (status)
		return (FAILURE);
	return (SUCCESS);
}

/**
 * @brief Closes the old file descriptor and replaces it with a new one.
 * 
 * This function attempts to close the old file descriptor referenced by 
 * `oldfd`, and then assigns the provided `replacement` file descriptor 
 * to it. If closing the old file descriptor fails, an error message is 
 * printed. The function checks if the new file descriptor is valid and 
 * returns the appropriate status.
 * 
 * @param[in] replacement The new file descriptor that will replace the 
 *                        old one.
 * @param[out] oldfd A pointer to the old file descriptor to be closed 
 *                    and replaced. This will be updated with the new 
 *                    file descriptor.
 * 
 * @return t_ecode Returns:
 * - `FAILURE` if the new file descriptor is invalid (-1) or if closing 
 *   the old file descriptor fails.
 * - `SUCCESS` if the operation completes without errors.
 */
static t_ecode	close_and_replace(int replacement, int *oldfd)
{
	if (*oldfd != ERROR)
	{
		if (close(*oldfd) == ERROR)
			handle_perror("close_and_replace");
	}
	*oldfd = replacement;
	if (*oldfd == ERROR)
		return (FAILURE);
	return (SUCCESS);
}
