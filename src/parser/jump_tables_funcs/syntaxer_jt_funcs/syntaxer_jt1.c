/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntaxer_jt1.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfeuer <tfeuer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 13:25:44 by tfeuer            #+#    #+#             */
/*   Updated: 2024/10/31 13:25:45 by tfeuer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../include/minishell.h"

t_ecode_p	syntax_noop(t_token *prev_token, t_token *cur_token, \
	int *par_count);
t_ecode_p	syntax_and_opr(t_token *prev_token, t_token *cur_token, \
	int *par_count);
t_ecode_p	syntax_ampersand(t_token *prev_token, t_token *cur_token, \
	int *par_count);
t_ecode_p	syntax_or_opr(t_token *prev_token, t_token *cur_token, \
	int *par_count);
t_ecode_p	syntax_pipe(t_token *prev_token, t_token *cur_token, \
	int *par_count);

/**
 * @brief A no-operation function for syntax parsing.
 * 
 * This function is utilized for handling spacing tokens and WORD tokens 
 * during syntax validation. It performs no action and always returns 
 * PARSING_OK. It serves as a placeholder in the syntax validation process 
 * when no operation is needed.
 * 
 * @param prev_token Pointer to the previous token (ignored).
 * @param cur_token Pointer to the current token (ignored).
 * @param par_count Pointer to the parentheses count (ignored).
 * 
 * @return Always returns PARSING_OK.
 */
t_ecode_p	syntax_noop(t_token *prev_token, t_token *cur_token, \
	int *par_count)
{
	(void)prev_token;
	(void)cur_token;
	(void)par_count;
	return (PARSING_OK);
}

/**
 * @brief Validate the syntax for the logical AND operator.
 * 
 * This function checks the token following a logical AND operator to ensure 
 * it adheres to the expected syntax rules. It verifies that the previous 
 * token exists and that the next token is valid. It returns an appropriate 
 * error code if the next token is not a valid command token. 
 * 
 * @param prev_token Pointer to the previous token.
 * @param cur_token Pointer to the current token.
 * @param par_count Pointer to the parentheses count (ignored).
 * 
 * @return 
 * - ERR_SYNTAX_AND if there is no previous token.
 * - ERR_SYNTAX_PIPE if the next token is a PIPE.
 * - ERR_SYNTAX_OR if the next token is an OR operator.
 * - ERR_SYNTAX_AND if the next token is another AND operator.
 * - ERR_SYNTAX_CLOSE_PAR if the next token is a closing parenthesis.
 * - ERR_SYNTAX_AMPER if the next token is an ampersand.
 * - PARSING_OK if the syntax is valid.
 */
t_ecode_p	syntax_and_opr(t_token *prev_token, t_token *cur_token, \
	int *par_count)
{
	t_token	*next_token;

	(void)par_count;
	if (!prev_token)
		return (ERR_SYNTAX_AND);
	next_token = skip_whitespace_and_get_next_token(cur_token);
	if (!next_token)
		return (ERR_PARSING_ERROR);
	if (next_token->id == PIPE)
		return (ERR_SYNTAX_PIPE);
	if (next_token->id == OR_OPR)
		return (ERR_SYNTAX_OR);
	if (next_token->id == AND_OPR)
		return (ERR_SYNTAX_AND);
	if (next_token->id == PAR_CLOSE)
		return (ERR_SYNTAX_CLOSE_PAR);
	if (next_token->id == AMPERSAND)
		return (ERR_SYNTAX_AMPER);
	return (PARSING_OK);
}

/**
 * @brief Validate the syntax for the ampersand operator.
 * 
 * This function checks the token following an ampersand operator to ensure 
 * it adheres to the expected syntax rules. It verifies that the previous 
 * token exists and checks the next token for validity. After processing 
 * all tokens, a separate check will examine the entire list of tokens 
 * to ensure no ampersands are present, as background process execution 
 * is not handled in this implementation.
 * 
 * @param prev_token Pointer to the previous token.
 * @param cur_token Pointer to the current token.
 * @param par_count Pointer to the parentheses count (ignored).
 * 
 * @return 
 * - ERR_SYNTAX_AMPER if there is no previous token.
 * - ERR_SYNTAX_PIPE if the next token is a PIPE.
 * - ERR_SYNTAX_AND if the next token is an AND operator.
 * - ERR_SYNTAX_OR if the next token is an OR operator.
 * - ERR_SYNTAX_AMPER if the next token is another ampersand.
 * - PARSING_OK if the syntax is valid.
 */
t_ecode_p	syntax_ampersand(t_token *prev_token, t_token *cur_token, \
	int *par_count)
{
	t_token	*next_token;

	(void)par_count;
	if (!prev_token)
		return (ERR_SYNTAX_AMPER);
	next_token = skip_whitespace_and_get_next_token(cur_token);
	if (next_token)
	{
		if (next_token->id == PIPE)
			return (ERR_SYNTAX_PIPE);
		if (next_token->id == AND_OPR)
			return (ERR_SYNTAX_AND);
		if (next_token->id == OR_OPR)
			return (ERR_SYNTAX_OR);
		if (next_token->id == AMPERSAND)
			return (ERR_SYNTAX_AMPER);
	}
	return (PARSING_OK);
}

/**
 * @brief Processes syntax checks for logical OR operators.
 *
 * Verifies the syntax surrounding a logical OR operator in the token list.
 * Ensures that the preceding token is valid and checks that the next 
 * token does not cause syntax errors with operators or parentheses.
 *
 * @param prev_token Pointer to the previous token for syntax checks.
 * @param cur_token Pointer to the current token (OR operator).
 * @param par_count Pointer to parentheses count (unused here).
 *
 * @return 
 * - PARSING_OK if syntax is valid,
 * - ERR_SYNTAX_OR for errors with OR operator,
 * - ERR_SYNTAX_AND for errors with AND operators,
 * - ERR_SYNTAX_PIPE for errors with PIPE tokens,
 * - ERR_SYNTAX_CLOSE_PAR for errors with closing parentheses,
 * - ERR_SYNTAX_AMPER if ampersand is found.
 */
t_ecode_p	syntax_or_opr(t_token *prev_token, t_token *cur_token, \
	int *par_count)
{
	t_token	*next_token;

	(void)par_count;
	if (!prev_token)
		return (ERR_SYNTAX_OR);
	next_token = skip_whitespace_and_get_next_token(cur_token);
	if (!next_token)
		return (ERR_PARSING_ERROR);
	if (next_token->id == PIPE)
		return (ERR_SYNTAX_PIPE);
	if (next_token->id == OR_OPR)
		return (ERR_SYNTAX_OR);
	if (next_token->id == AND_OPR)
		return (ERR_SYNTAX_AND);
	if (next_token->id == PAR_CLOSE)
		return (ERR_SYNTAX_CLOSE_PAR);
	if (next_token->id == AMPERSAND)
		return (ERR_SYNTAX_AMPER);
	return (PARSING_OK);
}

/**
 * @brief Processes syntax checks for pipe operators.
 *
 * Verifies syntax surrounding a pipe operator in the token list. 
 * Ensures the preceding token is valid and checks that the next token 
 * does not cause syntax errors with operators or parentheses.
 *
 * @param prev_token Pointer to the previous token for syntax checks.
 * @param cur_token Pointer to the current token (pipe operator).
 * @param par_count Pointer to parentheses count (unused here).
 *
 * @return 
 * - PARSING_OK if syntax is valid,
 * - ERR_SYNTAX_PIPE for errors with pipe operator,
 * - ERR_SYNTAX_AND for errors with AND operators,
 * - ERR_SYNTAX_OR for errors with OR operators,
 * - ERR_SYNTAX_CLOSE_PAR for errors with closing parentheses,
 * - ERR_SYNTAX_AMPER if ampersand is found.
 */
t_ecode_p	syntax_pipe(t_token *prev_token, t_token *cur_token, int *par_count)
{
	t_token	*next_token;

	(void)par_count;
	if (!prev_token)
		return (ERR_SYNTAX_PIPE);
	next_token = skip_whitespace_and_get_next_token(cur_token);
	if (!next_token)
		return (ERR_PARSING_ERROR);
	if (next_token->id == PIPE)
		return (ERR_SYNTAX_PIPE);
	if (next_token->id == AND_OPR)
		return (ERR_SYNTAX_AND);
	if (next_token->id == OR_OPR)
		return (ERR_SYNTAX_OR);
	if (next_token->id == PAR_CLOSE)
		return (ERR_SYNTAX_CLOSE_PAR);
	if (next_token->id == PAR_CLOSE)
		return (ERR_SYNTAX_CLOSE_PAR);
	if (next_token->id == AMPERSAND)
		return (ERR_SYNTAX_AMPER);
	return (PARSING_OK);
}
