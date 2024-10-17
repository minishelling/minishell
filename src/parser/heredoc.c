
#include "../../include/minishell.h"

// Function to convert an integer to a string
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

// Function to create the temporary file name
char *create_temp_file_for_heredoc(int counter) 
{
	const char *prefix = "/tmp/heredoc";
	char *file_name;
	char *counter_str;
	int total_length;

	counter_str = int_to_string(counter);
	if (!counter_str) 
		return (NULL);  // Handle allocation failure

	// Calculate the total length: length of "heredoc" + counter_str + null terminator
	total_length = strlen(prefix) + strlen(counter_str) + 1;

	// Allocate memory for the final file name
	file_name = (char *)malloc(total_length * sizeof(char));
	if (!file_name) 
	{
		perror("Failed to allocate memory");
		free(counter_str);  // Free previously allocated memory
		return NULL;
	}

	// Manually concatenate "heredoc" and the counter string
	strcpy(file_name, prefix);      // Copy "heredoc" to file_name
	strcat(file_name, counter_str); // Concatenate counter_str to file_name

	// Free the temporary counter string
	free(counter_str);

	return file_name;  // Return the dynamically allocated file name
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
		fd = open(file_name, O_RDWR | O_CREAT | O_TRUNC, 0644);
		init_signals(CHILD_HEREDOC);
		colourful_delimiter = ft_strjoin("heredoc [", MAGENTA_TEXT);
		colourful_delimiter = ft_strjoin_fs1(&colourful_delimiter, delimiter);
		colourful_delimiter = ft_strjoin_fs1(&colourful_delimiter, RESET_COLOR);
		colourful_delimiter = ft_strjoin_fs1(&colourful_delimiter, "]: ");
		while (1)
		{
			rl_clear_history();
			line = readline(colourful_delimiter);
			if (line == NULL)
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
			bytes_read = strlen(line);
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


/**
 * handle_heredocs - Processes heredoc tokens in the token list.
 * 
 * This function scans through the list of tokens to identify heredoc (`<<`)
 * operators and their corresponding delimiters. It then creates a temporary
 * file for the heredoc content and reads user input, using the specified 
 * delimiter to mark the end of the heredoc. The file descriptor of the 
 * heredoc is set in place of the delimiter in the token list.
 * 
 * If an interrupt signal (`SIGINT`) is received, the function returns early
 * with an appropriate error code.
 * 
 * @shell: A pointer to the shell structure containing relevant environment data.
 * @token_list: A pointer to the list of tokens to be processed.
 * 
 * Return: 0 on success, ERR_MEM on memory allocation failure, or SIGINT_HDOC 
 * if interrupted by a signal.
 */
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
			if (next_token != NULL && next_token->id == WORD) 
			{
				file_name = create_temp_file_for_heredoc(heredoc_counter);
				heredoc_counter++;
				if (file_name == NULL)
					return (ERR_MEM);
				delimiter = next_token->str;
				printf("in handle heredocs, delimiter is %s\n", delimiter);
				safe_assign_str(&next_token->str, file_name);  //protect better
				fd = read_heredoc_input(shell, next_token->str, delimiter);
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


