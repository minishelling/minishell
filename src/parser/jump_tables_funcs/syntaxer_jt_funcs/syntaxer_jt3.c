/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntaxer_jt3.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfeuer <tfeuer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 13:25:49 by tfeuer            #+#    #+#             */
/*   Updated: 2024/10/31 13:25:50 by tfeuer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
 * @brief Checks syntax validity for redirection tokens.
 *
 * Verifies that the token following a redirection operator (`>`, `>>`, 
 * `<`, `<<`) is valid. If the next token is an environment variable 
 * following a here-document (`<<`), it is treated as a regular word.
 *
 * @param prev_token Pointer to the previous token (unused here).
 * @param cur_token Pointer to the current token, expected to be a 
 *                  redirection token.
 * @param par_count Pointer to the integer tracking open parentheses 
 *                  count (unused here).
 * @return Error code if invalid, or PARSING_OK if syntax is valid.
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
 * Checks the token after a redirection operator, expected to hold a 
 * file name or heredoc delimiter. Validates syntax and returns error 
 * codes for tokens not allowed immediately following a redirection. 
 * If the token is a quote, removes surrounding quotes and updates 
 * token's ID to WORD.
 *
 * @param after_redir_token Pointer to the token after redirection.
 * @return Error code indicating validation result, possible values:
 *         - ERR_SYNTAX_PIPE: if next token is a pipe.
 *         - ERR_SYNTAX_AND: if next token is an AND operator.
 *         - ERR_SYNTAX_OR: if next token is an OR operator.
 *         - ERR_SYNTAX_OPEN_PAR: if next token is an open parenthesis.
 *         - ERR_SYNTAX_CLOSE_PAR: if next token is a close parenthesis.
 *         - ERR_SYNTAX_REDIR: if next token is another redirection.
 *         - ERR_SYNTAX_AMPER: if next token is an ampersand.
 *         - ERR_MEM: if memory allocation error while removing quotes.
 *         - PARSING_OK: if syntax is valid.
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
 * Modifies the specified token by removing leading and trailing quote 
 * delimiters. Sets token's ID to WORD after removing quotes. Allocates a 
 * new string for token content; returns an error if allocation fails.
 *
 * @param delimiter_token Pointer to the token to remove quotes from.
 * @return Error code for operation result:
 *         - ERR_MEM: if memory allocation fails.
 *         - PARSING_OK: if successful.
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
