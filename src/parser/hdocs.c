#include "../../include/minishell.h"

t_ecode_p	handle_hdocs(t_shell *shell, t_token *list);
t_ecode_p	process_hdoc_token(t_shell *shell, t_token *token, \
	int *hdoc_counter);
t_ecode_p	overwrite_str_w_hdoc_fn(int *hdoc_counter, t_token *delim_token);
char		*create_temp_hdoc_filename(int *counter);
t_ecode_p	overwrite_str_w_hdoc_fd(int hdoc_fd, t_token *delim_token);

/**
 * @brief Handles heredoc tokens in the provided token list.
 *
 * This function iterates through the list of tokens and processes each 
 * heredoc token (identified by the "<<"). For each heredoc token found, 
 * it calls `process_hdoc_token` to manage the heredoc input.
 *
 * @param shell Pointer to the shell structure containing necessary state 
 *              information.
 * @param list Pointer to the linked list of tokens to process.
 *
 * @return A code representing the status of the operation, which could 
 *         indicate success or specific errors encountered during 
 *         processing.
 */
t_ecode_p	handle_hdocs(t_shell *shell, t_token *list)
{
	int			hdoc_counter;
	t_ecode_p	err_no;

	hdoc_counter = 0;
	while (list)
	{
		if (ft_strncmp(list->str, "<<", 3) == 0 && list->id == LT)
		{
			err_no = process_hdoc_token(shell, list, &hdoc_counter);
			if (err_no)
				return (err_no);
		}
		list = list->next;
	}
	return (PARSING_OK);
}

/**
 * @brief Processes a heredoc token and handles its input.
 *
 * This function manages the delimiter associated with the delimiter token, 
 * which is the token immediately following the `<<` token. The string of 
 * the delimiter token that holds the delimiter will be overwritten, 
 * so the delimiter is duplicated using `ft_strdup` before it is 
 * overwritten. After writing into the file, the token's string will 
 * again be overwritten, this time with the file descriptor of the 
 * heredoc file.
 *
 * @param shell Pointer to the shell structure containing necessary state 
 *              information.
 * @param token Pointer to the delimiter token to process.
 * @param hdoc_counter Pointer to an integer that tracks the count of 
 *                     heredocs processed.
 *
 * @return A code representing the status of the operation, which could 
 *         indicate success or specific errors encountered during 
 *         processing.
 */
t_ecode_p	process_hdoc_token(t_shell *shell, t_token *token, \
	int *hdoc_counter)
{
	const char	*hdoc_delim;
	int			fd;
	t_ecode_p	err_no;

	hdoc_delim = ft_strdup(token->next->str);
	if (!hdoc_delim)
		return (ERR_MEM);
	err_no = overwrite_str_w_hdoc_fn(hdoc_counter, token->next);
	if (err_no)
		return (ft_free((void **)&hdoc_delim), err_no);
	fd = read_hdoc_input(shell, token->next->str, &hdoc_delim);
	if (g_signalcode == SIGINT)
	{
		shell->exit_code = EXIT_SIGINT;
		return (SIGINT_HDOC);
	}
	err_no = overwrite_str_w_hdoc_fd(fd, token->next);
	if (err_no)
		return (err_no);
	return (PARSING_OK);
}

/**
 * @brief Overwrites the string of the delimiter token with a temporary
 *        heredoc filename.
 *
 * This function creates a temporary heredoc filename and overwrites 
 * the string of the provided delimiter token with this filename. 
 * The filename is necessary for the open command used in 
 * `read_hdoc_input`. The heredoc counter is incremented to ensure 
 * unique filenames for multiple heredocs.
 *
 * @param hdoc_counter Pointer to an integer that tracks the count of 
 *                     heredocs processed.
 * @param delim_token Pointer to the delimiter token whose string will 
 *                    be overwritten.
 *
 * @return A code representing the status of the operation, which could 
 *         indicate success or specific errors encountered during 
 *         processing.
 */
t_ecode_p	overwrite_str_w_hdoc_fn(int *hdoc_counter, t_token *delim_token)
{
	char	*hdoc_filename;

	hdoc_filename = create_temp_hdoc_filename(hdoc_counter);
	if (!hdoc_filename)
		return (ERR_MEM);
	(*hdoc_counter)++;
	safe_assign_str(&delim_token->str, hdoc_filename);
	ft_free((void **) &hdoc_filename);
	if (!delim_token->str)
		return (ERR_MEM);
	return (PARSING_OK);
}

/**
 * @brief Overwrites the string of the delimiter token with the file descriptor 
 *        of the heredoc file and unlinks the temporary file.
 *
 * This function converts the provided heredoc file descriptor into a string 
 * and overwrites the string of the given delimiter token with this file 
 * descriptor string. This occurs after the input has been read into the 
 * heredoc file, allowing subsequent commands to reference the file descriptor 
 * for reading from the file. The temporary file is unlinked, allowing 
 * cleanup while still maintaining access to the file descriptor.
 *
 * @param hdoc_fd The file descriptor of the heredoc file that was written into.
 * @param delim_token Pointer to the delimiter token whose string will 
 *                    be overwritten with the file descriptor string.
 *
 * @return A code representing the status of the operation, which could 
 *         indicate success or specific errors encountered during 
 *         processing.
 */
t_ecode_p	overwrite_str_w_hdoc_fd(int hdoc_fd, t_token *delim_token)
{
	char	*fd_string;

	fd_string = ft_itoa(hdoc_fd);
	safe_assign_str(&delim_token->str, fd_string);
	ft_free((void **) &fd_string);
	if (!delim_token->str)
		return (ERR_MEM);
	return (PARSING_OK);
}

/**
 * @brief Creates a temporary filename for a heredoc file based on a counter.
 *
 * This function generates a unique temporary filename for a heredoc file 
 * by appending a counter value to a predefined prefix. The counter is 
 * incremented to ensure that each heredoc has a distinct filename, 
 * avoiding potential conflicts when multiple heredocs are processed.
 * The generated filename follows the format: "/tmp/heredoc{counter}".
 *
 * @param counter Pointer to an integer that tracks the count of 
 *                heredocs processed. This value is used to create 
 *                a unique filename.
 *
 * @return A pointer to the newly created temporary filename, or NULL 
 *         if memory allocation fails.
 */
char	*create_temp_hdoc_filename(int *counter)
{
	const char	*prefix;
	char		*file_name;
	char		*counter_str;
	int			total_length;

	prefix = "/tmp/heredoc";
	counter_str = ft_itoa(*counter);
	if (!counter_str)
		return (NULL);
	total_length = ft_strlen(prefix) + ft_strlen(counter_str);
	file_name = (char *)malloc((total_length + 1) * sizeof(char));
	if (!file_name)
	{
		free(counter_str);
		return (NULL);
	}
	ft_strlcpy(file_name, prefix, ft_strlen(prefix) + 1);
	ft_strlcat(file_name, counter_str, total_length + 1);
	free(counter_str);
	return (file_name);
}
