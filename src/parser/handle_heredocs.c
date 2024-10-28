#include "../../include/minishell.h"

char *int_to_string(int num)
{
	int		temp;
	int		digits;
	char	*str;

	temp = num;
	digits = 0;
	while (temp != 0)
	{
		temp /= 10;
		digits++;
	}
	if (num == 0)
		digits = 1;
	str = (char *)malloc((digits + 1) * sizeof(char));
	if (!str)
		return (NULL);
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

int	handle_heredocs (t_shell *shell, t_token *token_list)
{
	t_token	*next_token;
	char	*delimiter;
	char	*file_name;
	int		fd;
	int		heredoc_counter;
	char	*fd_string;

	heredoc_counter = 0;
	while (token_list)
	{
		if (ft_strncmp(token_list->str, "<<", 2) == 0)
		{
			next_token = token_list->next;
			file_name = create_temp_file_for_heredoc(heredoc_counter);
			if (!file_name)
				return (ERR_MEM);
			heredoc_counter++;
			delimiter = ft_strdup(next_token->str);
			if (!delimiter)
				return (ft_free((void **) &file_name), ERR_MEM);
			safe_assign_str(&next_token->str, file_name);
			if (!next_token->str)
				return (ft_free((void **) &file_name), \
					ft_free((void **)&delimiter), ERR_MEM);
			fd = read_hdoc_input(shell, next_token->str, (const char **)&delimiter);
			if (g_signalcode == SIGINT)
			{
				shell->exit_code = EXIT_SIGINT;
				ft_free((void **)&delimiter);
				ft_free((void **) &file_name);
				return (SIGINT_HDOC);
			}
			ft_free((void **) &file_name);
			ft_free((void **)&delimiter);
			fd_string = ft_itoa(fd);
			safe_assign_str(&next_token->str, fd_string);
			ft_free((void **) &fd_string);
			if (!next_token->str)
				return (ERR_MEM);
		}
		token_list = token_list->next;
	}
	return (PARSING_OK);
}
