#include "../../include/minishell.h"

t_ecode_p	read_hdoc_input(t_shell *shell, const char *file_name, \
	const char **hdoc_delim);
static void	handle_hdoc_child(t_shell *shell, const char *file_name, \
	const char *delimiter);
static char	*get_colourful_delimiter(const char *delimiter);
static void	run_hdoc_loop(const char *delimiter, int fd, \
	const char *colourful_delimiter);
void		handle_signals_after_reading_hdoc(t_shell *shell, int wstatus);

/**
 * @brief Reads input for a heredoc and handles the associated file descriptor.
 *
 * This function forks a new process to handle the heredoc input. The parent 
 * process waits for the child process to finish and cleans up the temporary 
 * file created for the heredoc. The function returns the file descriptor 
 * of the heredoc file after it has been opened for reading.
 *
 * @param shell Pointer to the shell structure containing necessary state 
 *              information.
 * @param file_name The name of the temporary file created for the heredoc input.
 * @param hdoc_delim Pointer to a string that has the delimiter for the heredoc
 *
 * @return A file descriptor for the opened heredoc file, or an error code 
 *         if the operation fails.
 */
t_ecode_p	read_hdoc_input(t_shell *shell, const char *file_name, \
	const char **hdoc_delim)
{
	pid_t	hdoc_parent;
	int		fd;
	int		wstatus;

	wstatus = 0;
	hdoc_parent = fork();
	init_signals(PARENT_HEREDOC);
	if (!hdoc_parent)
		handle_hdoc_child(shell, file_name, *hdoc_delim);
	waitpid(hdoc_parent, &wstatus, 0);
	ft_free((void **) hdoc_delim);
	if (WEXITSTATUS(wstatus) == EXIT_FAILURE)
		clean_nicely_and_exit(shell, EXIT_FAILURE);
	handle_signals_after_reading_hdoc(shell, wstatus);
	if (g_signalcode == SIGINT)
		return (ERROR);
	fd = open(file_name, O_RDONLY);
	if (fd == -1)
		clean_nicely_and_exit(shell, EXIT_FAILURE);
	unlink (file_name);
	return (fd);
}

/**
 * @brief Handles the child process for reading heredoc input.
 *
 * This function creates and opens the file created before for heredoc 
 * input, sets up the child to listen to SIGINT (Ctrl-C), (in such a 
 * case, closes the standard input to stop `readline`. By closing stdin, 
 * we signal to `readline` that it can no longer receive input, 
 * effectively stopping the reading process. If a signal is received, 
 * the child process will exit the reading loop and clean up resources 
 * accordingly). It then runs a loop to read lines until the delimiter 
 * is encountered.
 *
 * @param shell Pointer to the shell structure containing state info.
 * @param file_name The name of the file to write heredoc input to.
 * @param heredoc_delimiter The delimiter for the heredoc input.
 */
static void	handle_hdoc_child(t_shell *shell, const char *file_name, \
	const char *hdoc_delim)
{
	int		fd;
	char	*colourful_delimiter;

	(void)shell;
	fd = open(file_name, O_RDWR | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
	{
		ft_free((void **) &hdoc_delim);
		handle_perror("handle_heredoc_child");
		clean_nicely_and_exit(shell, EXIT_FAILURE);
	}
	init_signals(CHILD_HEREDOC);
	colourful_delimiter = get_colourful_delimiter(hdoc_delim);
	if (!colourful_delimiter)
	{
		ft_free((void **) &hdoc_delim);
		clean_nicely_and_exit(shell, EXIT_FAILURE);
	}
	run_hdoc_loop(hdoc_delim, fd, colourful_delimiter);
	ft_free((void **) &hdoc_delim);
	ft_free((void **)&colourful_delimiter);
	close(fd);
	clean_nicely_and_exit(shell, SUCCESS);
}

/**
 * @brief Creates a colorful prompt string for the heredoc delimiter.
 *
 * This function constructs a string that includes the heredoc delimiter 
 * formatted with color codes. The resulting string is used to prompt 
 * the user for input in the heredoc process.
 *
 * @param delimiter The delimiter for the heredoc input.
 *
 * @return A dynamically allocated string containing the colorful delimiter 
 *         prompt, or NULL if memory allocation fails. The caller is 
 *         responsible for freeing the returned string.
 */
static char	*get_colourful_delimiter(const char *delimiter)
{
	char	*col_delimiter;
	size_t	col_delimiter_len;

	col_delimiter_len = ft_strlen(delimiter) + ft_strlen("heredoc []: ")
		+ ft_strlen(MAGENTA_TEXT) + ft_strlen(RESET_COLOR);
	col_delimiter = (char *) ft_calloc(col_delimiter_len + 1, sizeof(char));
	if (!col_delimiter)
	{
		handle_perror("get_colourful_delimiter");
		return (NULL);
	}
	ft_strlcpy(col_delimiter, "heredoc [", col_delimiter_len + 1);
	ft_strlcat(col_delimiter, MAGENTA_TEXT, col_delimiter_len + 1);
	ft_strlcat(col_delimiter, delimiter, col_delimiter_len + 1);
	ft_strlcat(col_delimiter, RESET_COLOR, col_delimiter_len + 1);
	ft_strlcat(col_delimiter, "]: ", col_delimiter_len + 1);
	return (col_delimiter);
}

/**
 * @brief Runs the loop to read input for the heredoc until the delimiter 
 *        is hit or a signal is received.
 *
 * This function continuously prompts the user for input using readline 
 * with the specified colorful delimiter. It writes the input to the 
 * file descriptor until the user enters the delimiter or a signal 
 * interrupts the process, at which point it exits the loop.
 *
 * @param delimiter The delimiter string that indicates the end of input.
 * @param fd The file descriptor to write the heredoc input to.
 * @param colourful_delimiter The formatted prompt string for user input.
 */
static void	run_hdoc_loop(const char *delimiter, int fd, \
	const char *colourful_delimiter)
{
	size_t	bytes_read;
	char	*line;

	while (1)
	{
		rl_clear_history();
		line = readline(colourful_delimiter);
		if (!line)
			break ;
		if (!ft_strncmp(line, delimiter, ft_strlen(delimiter) + 1))
		{
			free(line);
			break ;
		}
		bytes_read = ft_strlen(line);
		write(fd, line, bytes_read);
		write(fd, "\n", 1);
		free(line);
	}
}

/**
 * @brief Handles signals in the parent process after heredoc input may 
 * have been stopped by a signal.
 *
 * This function checks if the heredoc reading process exited due to 
 * receiving a SIGINT (Ctrl-C). If so, it updates the global signal code 
 * and sets the shell's exit code to 130, indicating that the process was 
 * interrupted. It also prints a newline to the standard output, moves 
 * the readline cursor to a new line, and clears the current line input.
 * This behavior ensures that the user interface remains clean and that 
 * the shell can continue interactive operation. After processing, it 
 * initializes signals for non-interactive operation.
 *
 * @param shell Pointer to the shell structure containing state info.
 * @param wstatus The exit status of the child process.
 */
void	handle_signals_after_reading_hdoc(t_shell *shell, int wstatus)
{
	if (WIFEXITED(wstatus) && WEXITSTATUS(wstatus) == 130)
	{
		g_signalcode = SIGINT;
		shell->exit_code = 130;
		write(STDOUT_FILENO, "\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
	}
	init_signals(PARENT_NON_INTERACTIVE);
}
