#include "../../../../include/minishell.h"

t_ecode_p			syntax_open_paren(t_token *prev_token, \
	t_token *cur_token, int *par_count);
t_ecode_p			syntax_close_paren(t_token *prev_token, \
	t_token *cur_token, int *par_count);
static t_ecode_p	update_parens_balance(t_token *cur_token, int *par_count);
t_ecode_p			syntax_word(t_token *prev_token, t_token *cur_token, \
	int *par_count);

/**
 * @brief Handles the syntax validation for an opening parenthesis token.
 *
 * This function checks the token following an opening parenthesis to ensure
 * that the syntax is valid. It updates the parentheses balance count and 
 * verifies that the next token does not violate parsing rules. 
 * Specifically, it ensures that an opening parenthesis is followed by a 
 * valid token, and not by a pipe, logical operator, closing parenthesis, 
 * or ampersand. It also checks for proper use of parentheses by ensuring 
 * that a closing parenthesis is not incorrectly placed at the beginning of 
 * a command sequence.
 *
 * This function complements `check_parens`, which checks for overall 
 * balance of parentheses in the list, and helps maintain correct parsing 
 * behavior during the evaluation of expressions.
 *
 * @param prev_token Pointer to the previous token before the current token.
 * @param cur_token Pointer to the current token being evaluated.
 * @param par_count Pointer to the count of open parentheses, updated 
 *                  throughout parsing.
 * @return An error code indicating the parsing result, or 
 *         PARSING_OK if the syntax is valid.
 */
t_ecode_p	syntax_open_paren(t_token *prev_token, t_token *cur_token, \
	int *par_count)
{
	t_token	*next_token;

	(void)cur_token;
	update_parens_balance(cur_token, par_count);
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
	{
		if (!prev_token)
			return (ERR_SYNTAX_CLOSE_PAR);
		else
			return (ERR_SYNTAX_OPEN_PAR);
	}
	if (next_token->id == AMPERSAND)
		return (ERR_SYNTAX_AMPER);
	return (PARSING_OK);
}

/**
 * @brief Validates syntax for a closing parenthesis token.
 *
 * Checks the token preceding a closing parenthesis for correct syntax.
 * Verifies a matching opening parenthesis by examining `prev_token`,
 * then updates the parentheses balance count. Also checks the next
 * token (if any) to confirm it is not an opening parenthesis or one of
 * the prohibited types (single quote, double quote, or word).
 *
 * This function works with `check_parens` to validate parentheses
 * balance, helping detect syntax errors.
 *
 * @param prev_token Pointer to token before the current one.
 * @param cur_token Pointer to the closing parenthesis token.
 * @param par_count Pointer to open parentheses count, updated as parsing.
 * @return Error code if invalid or PARSING_OK if syntax is valid.
 */
t_ecode_p	syntax_close_paren(t_token *prev_token, t_token *cur_token, \
	int *par_count)
{
	t_token		*next_token;
	t_ecode_p	err_no;

	(void)cur_token;
	if (!prev_token)
		return (ERR_SYNTAX_CLOSE_PAR);
	err_no = update_parens_balance(cur_token, par_count);
	if (err_no)
		return (err_no);
	next_token = skip_whitespace_and_get_next_token(cur_token);
	if (next_token)
	{
		if (next_token->id == PAR_OPEN)
			return (ERR_SYNTAX_OPEN_PAR);
		if ((next_token->id == SQUOTE || next_token->id == DQUOTE \
			|| next_token->id == WORD))
			return (ERR_SYNTAX_ERROR);
	}
	return (PARSING_OK);
}

/**
 * @brief Updates the balance of parentheses based on the current token.
 *
 * This function increments or decrements the count of open parentheses 
 * based on the type of the current token. If the current token is an 
 * opening parenthesis, the count is incremented; if it's a closing 
 * parenthesis, the count is decremented. 
 *
 * If the parentheses count becomes negative, it indicates a closing 
 * parenthesis without a matching opening parenthesis, and an error 
 * code for this syntax error is returned. However, if there are more 
 * opening parentheses than closing ones, this imbalance will be addressed 
 * later in the `check_parens` function.
 *
 * @param cur_token Pointer to the current token being evaluated.
 * @param par_count Pointer to the integer tracking the count of open 
 *                  parentheses.
 * @return An error code indicating the result of the operation, or 
 *         PARSING_OK if the parentheses count is balanced.
 */
static t_ecode_p	update_parens_balance(t_token *cur_token, int *par_count)
{
	if (cur_token->id == PAR_OPEN)
		(*par_count)++;
	else if (cur_token->id == PAR_CLOSE)
		(*par_count)--;
	if (*par_count < 0)
		return (ERR_SYNTAX_CLOSE_PAR);
	return (PARSING_OK);
}

/**
 * @brief Checks syntax for a WORD token, ensuring it's valid.
 *
 * This function verifies that a WORD token is followed by an 
 * appropriate token or end of line, returning an error if it 
 * encounters specific invalid syntax, such as an unclosed 
 * parenthesis.
 *
 * @param prev_token Previous token in sequence (not used in this context).
 * @param cur_token Current WORD token being analyzed.
 * @param par_count Pointer to the parenthesis count (not used here).
 * @return Parsing error code or PARSING_OK if syntax is valid.
 */
t_ecode_p	syntax_word(t_token *prev_token, t_token *cur_token, int *par_count)
{
	t_token	*next_token;

	(void)par_count;
	(void)prev_token;
	next_token = skip_whitespace_and_get_next_token(cur_token);
	if (next_token)
	{
		if (next_token->id == PAR_OPEN)
		{
			if (next_token->next == NULL)
				return (ERR_SYNTAX_NL);
			else
				return (ERR_SYNTAX_ERROR);
		}
	}
	return (PARSING_OK);
}
