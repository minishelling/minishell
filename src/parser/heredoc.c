
#include "../../include/minishell.h"

// Function to convert an integer to a string
char *int_to_string(int num) 
{
    int temp = num, digits = 0;

    // Count the number of digits in the number
    while (temp != 0) 
    {
        temp /= 10;
        digits++;
    }

    // Handle the case where num is 0
    if (num == 0) digits = 1;

    // Allocate memory for the string (including null terminator)
    char *str = (char *)malloc((digits + 1) * sizeof(char));
    if (!str) 
    {
        perror("Failed to allocate memory");
        return NULL;
    }

    // Null terminate the string
    str[digits] = '\0';

    // Fill the string with the digits (from end to beginning)
    while (digits > 0) 
    {
        str[--digits] = '0' + (num % 10);
        num /= 10;
    }

    return str;
}

// Function to create the temporary file name
char *create_temp_file_for_heredoc(int counter) 
{
    const char *prefix = "/tmp/heredoc";
    char *file_name, *counter_str;
    int total_length;

	
    // Convert the counter to a string
    counter_str = int_to_string(counter);
    if (!counter_str) return NULL;  // Handle allocation failure

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

// Function to read input for the heredoc until the delimiter is reached
// int read_heredoc_input(const char *file_name, const char *delimiter) 
// {
//     char *line = NULL;
//     size_t len = 0;
//     ssize_t bytes_read;

// 	printf ("delimiter is %s\n", delimiter);
//     // Open the temporary file for writing
//     int fd = open(file_name, O_WRONLY | O_CREAT | O_TRUNC, 0644);
//     // if (fd == -1) 
//     // {
//     //    // error("Failed to open temporary file");
//     //     return (ERR_OPENING_FILE);
//     // }

//     printf("heredoc> "); // Prompt for input
//     // Read lines from stdin until the delimiter is found
//     while ((bytes_read = getline(&line, &len, stdin)) != -1) 
//     {
// 		printf ("delimiter is |%s|\n line is |%s|", delimiter, line);
// 		printf("line len %zu, delimiter len %zu\n", ft_strlen (line), ft_strlen(delimiter));
// 		// printf ("char is |%c|\n", line[ft_strlen(line) - 1]);
// 		// printf ("char is |%c|\n", line[ft_strlen(line)]);
//         // If the line matches the delimiter, stop reading
//         if ((!ft_strncmp(line, delimiter, ft_strlen(delimiter))) && (line[ft_strlen(delimiter)] == '\n'))
//             break;
    
//         // Write the line to the file followed by a newline
//         write(fd, line, bytes_read);
//     }

    #include <readline/readline.h>  // Make sure the readline library is included
#include <readline/history.h>   // Optional, if you want to use history features

int read_heredoc_input(t_shell *shell, const char *file_name, const char *delimiter) 
{
    char    *line = NULL;
    ssize_t bytes_read;
    pid_t   heredoc_parent;
    char    *colourful_delimiter;
    int     wstatus = 0;

    heredoc_parent = fork();
    init_signals(PARENT_HEREDOC);
    if (!heredoc_parent)
	{
        int fd = open(file_name, O_RDWR | O_CREAT | O_TRUNC, 0644);
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
    int fd = open(file_name, O_RDONLY);
    unlink(file_name);
    return (fd);
}

// Function to handle heredocs in a token list
void handle_heredocs(t_shell *shell, t_token *token_list) 
{
    t_token *current = token_list;
    t_token *next_token;

    int heredoc_counter = 0; // To generate unique file names

    while (current != NULL) 
    {
        // Check if current token string is "<<"
        if (ft_strncmp(current->str, "<<", 2) == 0) 
        {
            // Move to the next token, which should be the heredoc delimiter
            next_token = current->next;
            if (next_token != NULL && next_token->id == WORD) 
            {
                // Create the temporary file and get the file name
                char *file_name = create_temp_file_for_heredoc(heredoc_counter++);

                if (file_name) 
                {
                    // Replace the delimiter (next token's str) with the file name
                    
					char *delimiter = next_token->str;
					printf ("in handle heredocs, delimiter is %s\n", delimiter);
					//free(next_token->str); // Free the old delimiter string
                    next_token->str = file_name; // Assign the file name

                    // Read the heredoc input and write it to the file
                    int fd = read_heredoc_input(shell, next_token->str, delimiter);
                    if (g_signalcode == SIGINT)
                    {
                        shell->exit_code = EXIT_SIGINT;
                        return ;
                    }
                    else if (fd)
                    {
                        free(file_name);
                        next_token->str = ft_itoa(fd);
                    }
                    // else 
                    //print error 
                }
            }
        }
        // Move to the next token in the list
        current = current->next;
    }
}
