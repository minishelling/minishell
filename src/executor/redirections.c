#include "../../include/minishell.h"

static t_ecode	open_current_redir(t_redir_id redir_id,
					char *redir_file, int *fd);
static t_ecode	replace_redir_fd(t_cmd *cmd, t_redir *redir);
static t_ecode	close_and_replace(int replacement, int *oldfd);

/**
 * @brief Opens all input and output redirections of a command.
 * 
 * @param shell A pointer to the shell structure.
 * @param current_cmd A pointer to the structure of the current command.
 * 
 * @return If a redirection fails to open, if there's a dup failure,
 * or if the structures were NULL in the first place, it returns FAILURE,
 * otherwise it returns SUCCESS.
 */
t_ecode	open_redirections(t_shell *shell, t_cmd *current_cmd)
{
	t_redir	*current_redir;

	if (!shell || !current_cmd)
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
 * @brief Opens the current redirection.
 * 
 * It checks what kind of redirection it is and opens the file
 * appropriately. If it is a HEREDOC, instead of opening a file
 * it converts the string to an integer, since the file with
 * the heredoc contents it's already open and unlinked.
 * 
 * @param redir_id The type of input or output redirection.
 * @param redir_file The redirection file to be opened.
 * @param fd The file descriptor in which to store the open value.
 * 
 * @return If the file descriptor is ERROR (-1), it prints an error message
 * and returns FAILURE. Otherwise it returns SUCCESS.
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
 * @brief Replaces the 'latest_in' 'latest_out' redirection variables
 * with the current redirection.
 * 
 * 
 * 
 * @param cmd A pointer to the current command's structure.
 * @param redir A pointer to the current command's redirection structure.
 * 
 * @return If the current redirection is ERROR (-1), it closes the latest
 * redirection and replaces it with the error number, and returns FAILURE.
 * Otherwise it will attempt to duplicate the current redirection into the
 * 'latest' variable, and close the current redirection file descriptor
 * afterwards, returning SUCCESS. On error it returns FAILURE.
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
 * @brief It closes the oldfd and replaces it the the replacement.
 * 
 * @param replacement The fd to replace into oldfd.
 * @param oldfd A pointer to the oldfd that we want to close and replace.
 * 
 * @return If oldfd after being replaced is ERROR (-1) it returns FAILURE,
 * (which is probably needed as an error code for the calling function)
 * otherwise it returns SUCCESS.
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
