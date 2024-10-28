#include "../../include/minishell.h"

void	handle_parsing_err(t_shell *shell, int err_no);
char	*get_parsing_err_msg(int e);
void	handle_cmd_err(t_shell *shell, t_cmd *cmd, char *err_msg);
void	handle_perror(char *str);

void	handle_parsing_err(t_shell *shell, int err_no)
{
	char	*err_msg;
	char	*full_msg;
	size_t	full_msg_len;

	if (err_no != SIGINT_HDOC)
	{
		err_msg = get_parsing_err_msg(err_no);
		full_msg = ft_strjoin(ERR_PROMPT, err_msg);
		if (!full_msg)
		{
			perror("handle_parsing_err");
			exit(EXIT_FAILURE);
		}
		full_msg_len = ft_strlen(full_msg);
		write(2, full_msg, full_msg_len);
		free(full_msg);
		if (err_no > PARSING_OK  && err_no <= ERR_PARSING_ERROR)
			shell->exit_code = SYNTAX_FAILURE;
		else if (err_no == ERR_BG_PROCESS)
			shell->exit_code = FAILURE;  //agreed?
	}
	if ((err_no > PARSING_OK  && err_no <= ERR_PARSING_ERROR) || err_no == SIGINT_HDOC)
		clean_nicely(shell);
	else
		clean_nicely_and_exit(shell, err_no);
}

char	*get_parsing_err_msg(int e)
{
	char	*error_messages[] = { 
		"OK",
		"This is either a command substitution or an arithmetic expansion. We don't do these here.\n",
		"syntax error near unexpected token `newline`\n",
		"syntax error: unclosed quotes\n",
		"syntax error near unexpected token `(`\n",
		"syntax error near unexpected token `)`\n",
		"syntax error near unexpected token `|` \n",
		"syntax error near unexpected token `||`\n",
		"syntax error near unexpected token `&&`\n",
		"syntax error near unexpected token `<` or `>`\n",
		"syntax error near unexpected token `&`\n",
		"syntax error\n",
		"syntax err: more input is needed\n",
		"Running in the background - We don't do that here.\n",
		"Error: unable to allocate dynamic memory\n",
		"Error while expanding: unable to allocate dynamic memory\n",
		"Error while forming a command: unable to allocate dynamic memory\n",
		"SIGINT_HDOC"
	};
	return (error_messages[e]);
}

void	handle_cmd_err(t_shell *shell, t_cmd *cmd, char *err_msg)
{
	char		*full_msg;
	size_t		cmd_len;
	size_t		err_len;
	size_t		total_len;
	const char	*colon = ": ";
	const char	*newline = "\n";

	cmd_len = ft_strlen(cmd->args[0]);
	err_len = ft_strlen(err_msg);
	total_len = cmd_len + ft_strlen(colon) + err_len + ft_strlen(newline);
	full_msg = (char *)malloc(total_len + 1);
	if (!full_msg)
	{
		perror("handle_cmd_err");
		clean_nicely_and_exit(shell, EXIT_FAILURE);
	}
	ft_strlcpy(full_msg, cmd->args[0], cmd_len + 1);
	ft_strlcat(full_msg, colon, total_len + 1);
	ft_strlcat(full_msg, err_msg, total_len + 1);
	ft_strlcat(full_msg, newline, total_len + 1);
	write(2, full_msg, total_len);
	free(full_msg);
}

void	handle_perror(char *str)
{
	const char	*err_prompt;
	char		full_err_msg[ARG_MAX + 12];
	size_t		cmd_len, prompt_len, total_len;

	if (!str)
		return ;
	err_prompt = ERR_PROMPT;
	cmd_len = ft_strlen(str);
	prompt_len = ft_strlen(err_prompt);
	total_len = cmd_len + prompt_len;
	ft_strlcpy(full_err_msg, err_prompt, total_len + 1);
	ft_strlcat(full_err_msg, str, total_len + 1);
	perror(full_err_msg);
}
