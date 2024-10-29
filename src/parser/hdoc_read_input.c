#include "../../include/minishell.h"

t_ecode_p	read_hdoc_input(t_shell *shell, const char *file_name, \
	const char **hdoc_delim);
static void	handle_hdoc_child(t_shell *shell, const char *file_name, \
	const char *delimiter);
static char	*get_colourful_delimiter(const char *delimiter);
static void	run_hdoc_loop(const char *delimiter, int fd, \
	const char *colourful_delimiter);
void		handle_signals_after_reading_hdoc(t_shell *shell, int wstatus);

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
