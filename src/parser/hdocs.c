#include "../../include/minishell.h"

int		handle_hdocs (t_shell *shell, t_token *token_list);
int		overwrite_token_str_w_hdoc_filename(int *hdoc_counter, t_token *delim_token);
char	*create_temp_hdoc_filename(int *counter);
int 	overwrite_token_str_w_hdoc_fd (int hdoc_fd, t_token *delim_token);

//str of hdoc token that is holding the hdoc delim will be overwritten, so we strdup it
// before we overwite.
int	handle_hdocs (t_shell *shell, t_token *token_list)
{
	const char	*hdoc_delim;
	int		hdoc_counter;
	int		err_no;
	int		fd;
	
	hdoc_counter = 0;
	while (token_list)
	{
		if (ft_strncmp(token_list->str, "<<", 2) == 0 && token_list->id == LT)
		{
			hdoc_delim = ft_strdup(token_list->next->str);
			if (!hdoc_delim)
				return (ERR_MEM);
			err_no = overwrite_token_str_w_hdoc_filename(&hdoc_counter, token_list->next);
			if (err_no)
				return (ft_free((void **) &hdoc_delim), err_no);
			fd = read_hdoc_input(shell, token_list->next->str, &hdoc_delim);
			if (g_signalcode == SIGINT)
			{
				shell->exit_code = E_SIGINT;
				// ft_free((void **) &hdoc_delim);  // added this in read_hdoc_input
				//printf ("hdoc_delim is %p\n", hdoc_delim);
				return (SIGINT_HDOC);
			}
			err_no = overwrite_token_str_w_hdoc_fd(fd, token_list->next);
			if (err_no)
				return(err_no);
		}
		token_list = token_list->next;
	}
	return (PARSING_OK);
}

int	overwrite_token_str_w_hdoc_filename(int *hdoc_counter, t_token *delim_token)
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

int overwrite_token_str_w_hdoc_fd (int hdoc_fd, t_token *delim_token)
{
	char	*fd_string;
	
	fd_string = ft_itoa(hdoc_fd);
	safe_assign_str(&delim_token->str, fd_string);
	ft_free((void **) &fd_string);
	if (!delim_token->str)
		return (ERR_MEM);
	return (PARSING_OK);
}

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




