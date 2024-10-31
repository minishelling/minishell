#include "../../include/minishell.h"

void		handle_parsing_err(t_shell *shell, t_ecode_p err_no);
static char	*get_parsing_err_msg(int e);
void		handle_cmd_err(t_shell *shell, t_cmd *cmd, char *err_msg);
void		handle_perror(char *str);

/**
 * @brief Handles parsing errors and outputs an appropriate error message.
 *
 * This function takes an error code, constructs an error message based on 
 * the provided error code, and writes it to the standard error output. 
 * It also sets the shell's exit code according to the type of error encountered.
 *
 * @param shell Pointer to the shell structure that holds the state of the shell.
 * @param err_no The error code indicating the type of parsing error that occurred.
 *
 * Possible error codes: PARSING_OK (0), ERR_CMD_SUBSTIT (2), ERR_SYNTAX_NL (2), 
 * ERR_UNCLOSED_QUOTES (2), ERR_SYNTAX_OPEN_PAR (2), ERR_SYNTAX_CLOSE_PAR (2), 
 * ERR_SYNTAX_PIPE (2), ERR_SYNTAX_OR (2), ERR_SYNTAX_AND (2), ERR_SYNTAX_REDIR (2), 
 * ERR_SYNTAX_AMPER (2), ERR_SYNTAX_ERROR (2), ERR_BG_PROCESS (2), 
 * ERR_PARSING_ERROR (2), ERR_MEM (3), ERR_EXPAND (3), ERR_CMD (3), 
 * SIGINT_HDOC (130).
 *
 * This function performs actions based on the error code, including setting 
 * the appropriate exit code and cleaning up the shell state.
 */
void	handle_parsing_err(t_shell *shell, t_ecode_p err_no)
{
	char	*full_msg;

	if (err_no != SIGINT_HDOC)
	{
		shell->exit_code = EXIT_SUCCESSFULLY;
		full_msg = ft_strjoin(ERR_PROMPT, get_parsing_err_msg(err_no));
		if (!full_msg)
		{
			perror("handle_parsing_err");
			exit(EXIT_FAILURE);
		}
		write(2, full_msg, ft_strlen(full_msg));
		free(full_msg);
		if (err_no > PARSING_OK && err_no <= ERR_PARSING_ERROR)
			shell->exit_code = EXIT_SYNTAX_ERROR;
		else
			shell->exit_code = EXIT_MEM_FAILURE;
	}
	else
		shell->exit_code = EXIT_SIGINT;
	if ((err_no > PARSING_OK && err_no <= ERR_PARSING_ERROR) \
		|| err_no == SIGINT_HDOC)
		clean_nicely(shell);
	else
		clean_nicely_and_exit(shell, shell->exit_code);
}

static char	*get_parsing_err_msg(int e)
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
	err_msg[12] = "Running in the background - We don't do that here.\n";
	err_msg[13] = "syntax err: more input is needed\n";
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
