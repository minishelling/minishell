
#include "../../include/minishell.h"

char *int_to_string(int num) 
{
	int temp;
	int digits;

	temp = num;
	digits = 0;
	while (temp != 0) 
	{
		temp /= 10;
		digits++;
	}
	if (num == 0)
		digits = 1;
	char *str = (char *)malloc((digits + 1) * sizeof(char));
	if (!str) 
		return (NULL); //ERR_MEM
	str[digits] = '\0';
	while (digits > 0) 
	{
		str[--digits] = '0' + (num % 10);
		num /= 10;
	}
	return (str);
}

char *create_temp_file_for_heredoc(int counter) 
{
	const char *prefix = "/tmp/heredoc";
	char *file_name;
	char *counter_str;
	int total_length;

	counter_str = int_to_string(counter);
	if (!counter_str) 
		return (NULL);
	total_length = ft_strlen(prefix) + ft_strlen(counter_str) + 1;
	file_name = (char *)malloc(total_length * sizeof(char));
	if (!file_name) 
	{
		perror("Failed to allocate memory"); //!
		free(counter_str);
		return NULL;
	}
	strcpy(file_name, prefix);
	strcat(file_name, counter_str);
	free(counter_str);
	return (file_name);
}

int read_heredoc_input(t_shell *shell, const char *file_name, const char *delimiter) 
{
	char    *line = NULL;
	ssize_t bytes_read;
	pid_t   heredoc_parent;
	char    *colourful_delimiter;
	int     fd;
	int     wstatus;

	wstatus = 0;
	heredoc_parent = fork();
	init_signals(PARENT_HEREDOC);
	if (!heredoc_parent)
	{
		fd = open(file_name, O_RDWR | O_CREAT | O_TRUNC, 0644);   //check
		init_signals(CHILD_HEREDOC);
		colourful_delimiter = ft_strjoin("heredoc [", MAGENTA_TEXT);
		colourful_delimiter = ft_strjoin_fs1(&colourful_delimiter, delimiter);
		colourful_delimiter = ft_strjoin_fs1(&colourful_delimiter, RESET_COLOR);
		colourful_delimiter = ft_strjoin_fs1(&colourful_delimiter, "]: ");
		while (1)
		{
			rl_clear_history();
			line = readline(colourful_delimiter);
			if (!line)
			{
				ft_free((void **) &colourful_delimiter);
				close (fd);
				exit(EXIT_SUCCESS);
			}
			if ((!ft_strncmp(line, delimiter, ft_strlen(delimiter))) && (line[ft_strlen(delimiter)] == '\0'))
			{
				free(line);
				ft_free((void **) &colourful_delimiter);
				close (fd);
				exit(EXIT_SUCCESS);
			}
			bytes_read = ft_strlen(line);
			write(fd, line, bytes_read);
			write(fd, "\n", 1);
			free(line);
		}
		ft_free((void **) &colourful_delimiter);
	}
	waitpid(heredoc_parent, &wstatus, 0);
	if (WIFEXITED(wstatus) && WEXITSTATUS(wstatus) == 130)
		shell->exit_code = 130;
	if (g_signalcode == SIGINT)
	{
		init_signals(PARENT_NON_INTERACTIVE);
		return (-1);
	}
	init_signals(PARENT_NON_INTERACTIVE);
	fd = open(file_name, O_RDONLY);
	unlink(file_name);
	return (fd);
}


int handle_heredocs(t_shell *shell, t_token *token_list) 
{
	t_token *current;
	t_token *next_token;
	char *delimiter;
	char *file_name;
	int fd;
	int heredoc_counter;

	current = token_list;
	heredoc_counter = 0;

	while (current != NULL)
	{
		if (ft_strncmp(current->str, "<<", 2) == 0) 
		{
			next_token = current->next;
			if (next_token && next_token->id == WORD) 
			{
				file_name = create_temp_file_for_heredoc(heredoc_counter);
				heredoc_counter++;
				if (file_name == NULL)
					return (ERR_MEM);
				delimiter = ft_strdup(next_token->str);
				safe_assign_str(&next_token->str, file_name);  //protect better
				fd = read_heredoc_input(shell, next_token->str, delimiter);  //protect
				if (g_signalcode == SIGINT)
				{
					shell->exit_code = EXIT_SIGINT;
					return (SIGINT_HDOC);
				}
				else if (fd)
				{
					free(file_name);
					safe_assign_str(&next_token->str, ft_itoa(fd)); //protect better
				}
				// else
				// 	error
			}
		}
		current = current->next;
	}
	return (PARSING_OK);
}


