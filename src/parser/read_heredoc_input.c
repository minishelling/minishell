#include "../../include/minishell.h"

static int	handle_heredoc_child(t_shell *shell, const char *file_name, \
	const char *delimiter);
static char	*get_colourful_delimiter(const char *delimiter);
static void	run_heredoc_loop(const char *delimiter, int fd, \
	const char *colourful_delimiter);
void	handle_signals_after_reading_hdoc(t_shell *shell, int wstatus);

int	read_heredoc_input(t_shell *shell, const char *file_name, \
	const char *delimiter)
{
	pid_t	heredoc_parent;
	int		fd;
	int		wstatus;

	wstatus = 0;
	heredoc_parent = fork();
	init_signals(PARENT_HEREDOC);
	if (!heredoc_parent)
		handle_heredoc_child(shell, file_name, delimiter);
	waitpid(heredoc_parent, &wstatus, 0);
	handle_signals_after_reading_hdoc(shell, wstatus);
	if (g_signalcode == SIGINT)
		return (ERROR);
	fd = open(file_name, O_RDONLY);
	if (fd == -1)
	{
		handle_perror("read_heredoc_input");
		clean_nicely_and_exit(shell, EXIT_FAILURE);
	}	
	unlink (file_name);
	return (fd);
}

static int	handle_heredoc_child(t_shell *shell, const char *file_name, \
	const char *delimiter)
{
	int		fd;
	char	*colourful_delimiter;

	fd = open(file_name, O_RDWR | O_CREAT | O_TRUNC, 0644);
	if (!fd)
		handle_perror("handle_heredoc_child");
	init_signals(CHILD_HEREDOC);
	colourful_delimiter = get_colourful_delimiter(delimiter);
	if (!colourful_delimiter)
		clean_nicely_and_exit(shell, EXIT_FAILURE);
	run_heredoc_loop(delimiter, fd, colourful_delimiter);
	ft_free((void **)&colourful_delimiter);
	close(fd);
	exit(g_signalcode);
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
		handle_perror("handle_heredoc_child");
		return (NULL);
	}
	ft_strlcpy(col_delimiter, "heredoc [", col_delimiter_len + 1);
	ft_strlcat(col_delimiter, MAGENTA_TEXT, col_delimiter_len + 1);
	ft_strlcat(col_delimiter, delimiter, col_delimiter_len + 1);
	ft_strlcat(col_delimiter, RESET_COLOR, col_delimiter_len + 1);
	ft_strlcat(col_delimiter, "]: ", col_delimiter_len + 1);
	return (col_delimiter);
}

static void	run_heredoc_loop(const char *delimiter, int fd, \
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
		if ((!ft_strncmp(line, delimiter, ft_strlen(delimiter)))
			&& (line[ft_strlen(delimiter)] == '\0'))
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