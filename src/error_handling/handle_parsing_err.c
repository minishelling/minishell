#include "../../include/minishell.h"

void	handle_parsing_err(t_shell *shell, t_ecode_p err_no);
char	*get_parsing_err_msg(int e);
void	handle_cmd_err(t_shell *shell, t_cmd *cmd, char *err_msg);
void	handle_perror(char *str);

void	handle_parsing_err(t_shell *shell, t_ecode_p err_no)
{
	char	*err_msg;
	char	*full_msg;

	if (err_no != SIGINT_HDOC)
	{
		err_msg = get_parsing_err_msg(err_no);
		full_msg = ft_strjoin(ERR_PROMPT, err_msg);
		if (!full_msg)
		{
			perror("handle_parsing_err");
			exit(EXIT_FAILURE);
		}
		write(2, full_msg, ft_strlen(full_msg));
		free(full_msg);
		if (err_no > PARSING_OK && err_no <= ERR_PARSING_ERROR)
			shell->exit_code = SYNTAX_FAILURE;
		else if (err_no == ERR_BG_PROCESS)
			shell->exit_code = FAILURE; // agreed?
	}
	if ((err_no > PARSING_OK && err_no <= ERR_PARSING_ERROR) \
		|| err_no == SIGINT_HDOC)
		clean_nicely(shell);
	else
		clean_nicely_and_exit(shell, err_no);
}

char	*get_parsing_err_msg(int e)
{
	char	*err_msg[18];

	err_msg[0] = "OK";
	err_msg[1] = "This is either a command substitution or an arithmetic expansion. \
		We don't do these here.\n";
	err_msg[2] = "syntax error near unexpected token `newline`\n";
	err_msg[3] = "syntax error: unclosed quotes\n";
	err_msg[4] = "syntax error near unexpected token `(`\n";
	err_msg[5] = "syntax error near unexpected token `)`\n";
	err_msg[6] = "syntax error near unexpected token `|`\n";
	err_msg[7] = "syntax error near unexpected token `||`\n";
	err_msg[8] = "syntax error near unexpected token `&&`\n";
	err_msg[9] = "syntax error near unexpected token `<` or `>`\n";
	err_msg[10] = "syntax error near unexpected token `&`\n";
	err_msg[11] = "syntax error\n";
	err_msg[12] = "syntax err: more input is needed\n";
	err_msg[13] = "Running in the background - We don't do that here.\n";
	err_msg[14] = "Error: unable to allocate dynamic memory\n";
	err_msg[15] = "Error while expanding: unable to allocate dynamic memory\n";
	err_msg[16] = "Error while forming a command: \
		unable to allocate dynamic memory\n";
	err_msg[17] = "SIGINT_HDOC";
	return (err_msg[e]);
}

void	handle_cmd_err(t_shell *shell, t_cmd *cmd, char *err_msg)
{
	const char	*colon = ": ";
	const char	*newline = "\n";
	char		*full_msg;

	full_msg = (char *)malloc(ft_strlen(cmd->args[0]) + ft_strlen(colon) + \
		ft_strlen(err_msg) + ft_strlen(newline) + 1);
	if (!full_msg)
	{
		perror("handle_cmd_err");
		clean_nicely_and_exit(shell, EXIT_FAILURE);
	}
	ft_strlcpy(full_msg, cmd->args[0], ft_strlen(cmd->args[0]) + 1);
	ft_strlcat(full_msg, colon, ft_strlen(full_msg) + ft_strlen(colon) + 1);
	ft_strlcat(full_msg, err_msg, ft_strlen(full_msg) + ft_strlen(err_msg) + 1);
	ft_strlcat(full_msg, newline, ft_strlen(full_msg) + ft_strlen(newline) + 1);
	write(2, full_msg, ft_strlen(full_msg));
	free(full_msg);
}

void	handle_perror(char *str)
{
	const char	*err_prompt;
	char		full_err_msg[ARG_MAX + 12];
	size_t		cmd_len;
	size_t		prompt_len;
	size_t		total_len;

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
