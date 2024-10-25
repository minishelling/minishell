
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

char	*create_temp_file_for_heredoc(int counter) 
{
	const char	*prefix;
	char		*file_name;
	char		*counter_str;
	int			total_length;

	prefix = "/tmp/heredoc";
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


int	handle_heredocs(t_shell *shell, t_token *token_list) 
{
	t_token	*next_token;
	char	*delimiter;
	char	*file_name;
	int		fd;
	int		heredoc_counter;

	heredoc_counter = 0;
	while (token_list)
	{
		if (ft_strncmp(token_list->str, "<<", 2) == 0)
		{
			next_token = token_list->next;
				file_name = create_temp_file_for_heredoc(heredoc_counter);
				heredoc_counter++;
				if (file_name == NULL)
					return (ERR_MEM);
				delimiter = ft_strdup(next_token->str);
				safe_assign_str(&next_token->str, file_name); //protect better
				fd = read_heredoc_input(shell, next_token->str, delimiter);
				if (g_signalcode == SIGINT)
				{
					shell->exit_code = E_SIGINT;
					return (SIGINT_HDOC);
				}
				free(file_name);
				safe_assign_str(&next_token->str, ft_itoa(fd)); //protect better
		}
		token_list = token_list->next;
	}
	return (PARSING_OK);
}
