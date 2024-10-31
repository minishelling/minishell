#include "../../../../include/minishell.h"

t_ecode_p			syntax_redir(t_token *prev_token, t_token *cur_token, \
	int *par_count);
static t_ecode_p	handle_token_after_redir(t_token *after_redir_token);
static t_ecode_p	remove_delimiter_quotes(t_token *delimiter_token);
t_ecode_p			syntax_quote(t_token *prev_token, t_token *cur_token, \
	int *par_count);
t_ecode_p			syntax_env_var(t_token *prev_token, t_token *cur_token, \
	int *par_count);

/**
 * @brief Handles syntax checking for redirection tokens.
 *
 * This function checks the token following a redirection operator (e.g., 
 * `>`, `>>`, `<`, `<<`) to ensure it is valid. If the next token is an 
 * environment variable following a here-document operator (`<<`), it 
 * is treated as a regular word.
 *
 * @param prev_token Pointer to the previous token (not used in this function).
 * @param cur_token Pointer to the current token, which should be a redirection token.
 * @param par_count Pointer to the integer tracking the count of open parentheses (not used).
 * @return An error code indicating the result of the operation, or 
 *         PARSING_OK if the syntax is valid.
 */
t_ecode_p	syntax_redir(t_token *prev_token, t_token *cur_token, \
	int *par_count)
{
	t_token		*next_token;
	t_ecode_p	err_no;

	(void)prev_token;
	(void)par_count;
	next_token = skip_whitespace_and_get_next_token(cur_token);
	if (!next_token)
		return (ERR_SYNTAX_NL);
	if (next_token->id == ENV_VAR && !ft_strncmp((cur_token->str), "<<", 3))
	{
		next_token->id = WORD;
		return (PARSING_OK);
	}
	err_no = handle_token_after_redir(next_token);
	if (err_no)
		return (err_no);
	return (PARSING_OK);
}

/**
 * @brief Validates the token following a redirection operator.
 *
 * This function checks the token that comes after a redirection operator. 
 * This token is expected to hold the file name or the heredoc delimiter. 
 * The function validates the syntax and returns appropriate error codes 
 * for specific tokens that are not allowed immediately following a redirection. 
 * If the token is a quote, it removes the surrounding quotes and updates 
 * the token's ID to WORD.
 *
 * @param after_redir_token Pointer to the token that follows the redirection token.
 * @return An error code indicating the result of the validation. Possible 
 *         return values include:
 *         - ERR_SYNTAX_PIPE: if the next token is a pipe.
 *         - ERR_SYNTAX_AND: if the next token is an AND operator.
 *         - ERR_SYNTAX_OR: if the next token is an OR operator.
 *         - ERR_SYNTAX_OPEN_PAR: if the next token is an open parenthesis.
 *         - ERR_SYNTAX_CLOSE_PAR: if the next token is a close parenthesis.
 *         - ERR_SYNTAX_REDIR: if the next token is another redirection operator.
 *         - ERR_SYNTAX_AMPER: if the next token is an ampersand.
 *         - ERR_MEM: if there is a memory allocation error while 
 *           removing quotes.
 *         - PARSING_OK: if the syntax is valid.
 */
static t_ecode_p	handle_token_after_redir(t_token *after_redir_token)
{
	if (after_redir_token->id == PIPE)
		return (ERR_SYNTAX_PIPE);
	if (after_redir_token->id == AND_OPR)
		return (ERR_SYNTAX_AND);
	if (after_redir_token->id == OR_OPR)
		return (ERR_SYNTAX_OR);
	if (after_redir_token->id == PAR_OPEN)
		return (ERR_SYNTAX_OPEN_PAR);
	if (after_redir_token->id == PAR_CLOSE)
		return (ERR_SYNTAX_CLOSE_PAR);
	if (after_redir_token->id == GT || after_redir_token->id == LT)
		return (ERR_SYNTAX_REDIR);
	if (after_redir_token->id == AMPERSAND)
		return (ERR_SYNTAX_AMPER);
	if (after_redir_token->id == SQUOTE || after_redir_token->id == DQUOTE)
		if (remove_delimiter_quotes(after_redir_token) != PARSING_OK)
			return (ERR_MEM);
	return (PARSING_OK);
}

static t_ecode_p	remove_delimiter_quotes(t_token *delimiter_token)
{
	delimiter_token->id = WORD;
	if (safe_assign_str(&(delimiter_token->str), \
		(ft_substr(delimiter_token->str, 1, \
			(ft_strlen(delimiter_token->str) - 2)))) != SUCCESS)
		return (ERR_MEM);
	return (PARSING_OK);
}

/**
 * @brief Removes surrounding quotes from a token's string.
 *
 * This function modifies the specified token by removing the leading 
 * and trailing characters that act as delimiters. The token's ID is 
 * updated to WORD after the quotes are removed. The function allocates 
 * a new string for the token's content, and if the memory allocation 
 * fails, it returns an error code.
 *
 * @param delimiter_token Pointer to the token from which quotes are to be removed.
 * @return An error code indicating the result of the operation:
 *         - ERR_MEM: if there is a memory allocation error.
 *         - PARSING_OK: if the operation was successful.
 */
t_ecode_p	syntax_quote(t_token *prev_token, t_token *cur_token, \
	int *par_count)
{
	t_token	*next_token;

	(void)prev_token;
	(void)par_count;
	next_token = skip_whitespace_and_get_next_token(cur_token);
	if (next_token && next_token->id == PAR_OPEN)
		return (ERR_SYNTAX_OPEN_PAR);
	return (PARSING_OK);
}

/**
 * @brief Checks the syntax of an environment variable token.
 *
 * This function verifies that the token following the current environment 
 * variable token does not open a parenthesis. If a parenthesis is found, 
 * it indicates a syntax error.
 *
 * Command substitution is already handled in tokenization in the 
 * `advance_pos_env_var` function.
 *
 * @param prev_token Pointer to the previous token.
 * @param cur_token Pointer to the current token being processed.
 * @param par_count Pointer to the count of open parentheses.
 * @return An error code indicating the result of the operation:
 *         - ERR_SYNTAX_ERROR: if the next token is an opening parenthesis.
 *         - PARSING_OK: if the operation was successful.
 */
t_ecode_p	syntax_env_var(t_token *prev_token, t_token *cur_token, \
	int *par_count)
{
	t_token	*next_token;

	(void)prev_token;
	(void)cur_token;
	(void)par_count;
	next_token = skip_whitespace_and_get_next_token(cur_token);
	if (next_token && next_token->id == PAR_OPEN)
		return (ERR_SYNTAX_ERROR);
	return (PARSING_OK);
}
