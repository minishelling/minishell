#include "../../../include/minishell.h"

/**
 * @brief No-operation function for syntax checking.
 *
 * This function is a no-op (no operation). It is used for tokens where no specific
 * syntax check is required, such as whitespace tokens, quote tokens, and environment
 * variable tokens.
 *
 * @param prev_token Previous token (unused).
 * @param cur_token Current token (unused).
 * @param env_list The environment list (unused).
 * 
 * @return Always returns PARSING_OK, as no syntax check is performed.
 */
int	syntax_noop(t_token *prev_token, t_token *cur_token, t_env *env_list)
{
	(void) prev_token;
	(void) cur_token;
	(void) env_list;
	return (PARSING_OK);
}

/**
 * @brief Syntax checking for pipe (|) and OR operator (||).
 *
 * This function performs syntax checks for pipe (`|`) and OR operator (`||`) tokens
 * and checks the surrounding tokens to ensure valid syntax. It checks the following cases:
 *
 * - A pipe or OR operator (`|` or `||`) cannot be the first token.
 * - A pipe or OR operator cannot be followed by a closing parenthesis (`)`).
 * - A pipe or OR operator cannot be followed by another pipe or OR operator.
 * - A pipe or OR operator cannot be preceded by a redirection token (`>` or `<`).
 * - An OR operator (`||`) cannot be followed by another pipe (`|`) or OR operator (`||`).
 *
 * If any of these invalid cases are found, an error will be returned.
 *
 * @param prev_token Previous token in the token list (may be NULL).
 * @param cur_token Current token (pipe or OR operator).
 * @param env_list The environment list (unused).
 * 
 * @return Returns `PARSING_OK` if the pipe/OR operator syntax is valid,
 *         or an appropriate error code if invalid syntax is detected:
 *         - `ERR_SYNTAX_PIPE` if invalid pipe syntax is found.
 */
int	syntax_pipe(t_token *prev_token, t_token *cur_token, t_env *env_list)
{
	t_token	*next_token;
	
	(void) env_list;
	next_token = skip_whitespace_and_get_next_token(cur_token);
	if (prev_token == NULL || next_token == NULL)
		return (ERR_SYNTAX_PIPE);
	if ((cur_token->id == PIPE || cur_token->id == OR_OPR) && next_token->id == PAR_CLOSE)
		return (ERR_SYNTAX_PIPE);
	if ((cur_token->id == PIPE || cur_token->id == OR_OPR) && (next_token->id == PIPE || next_token->id == OR_OPR))
			return (ERR_SYNTAX_PIPE);
	if ((cur_token->id == OR_OPR && (next_token->id == OR_OPR || next_token->id == PIPE)) ||
		(cur_token->id == PIPE && next_token->id == OR_OPR))
			return (ERR_SYNTAX_PIPE);
	return (PARSING_OK);
}

/**
 * @brief Syntax checking for AND operator (`&&`).
 *
 * This function performs syntax checks for the AND operator (`&&`) token,
 * and checks the surrounding tokens to ensure valid syntax. It checks the following cases:
 *
 * - The AND operator cannot be preceded by a redirection token (`>` or `<`).
 * - The AND operator cannot be followed by another AND operator (`&&`).
 * - The AND operator cannot be followed by an OR operator (`||`).
 * - The AND operator cannot be followed by a closing parenthesis (`)`).
 *
 * If any of these invalid cases are found, an error will be returned.
 *
 * @param prev_token Previous token in the token list (may be NULL).
 * @param cur_token Current token (AND operator).
 * @param env_list The environment list (unused).
 * 
 * @return Returns `PARSING_OK` if the AND operator syntax is valid,
 *         or an appropriate error code if invalid syntax is detected:
 *         - `ERR_SYNTAX_AND` if invalid AND operator syntax is found.
 */
int	syntax_and_opr(t_token *prev_token, t_token *cur_token, t_env *env_list)
{
	t_token	*next_token;

	(void) env_list;
	next_token = skip_whitespace_and_get_next_token(cur_token);
	if (prev_token == NULL || next_token == NULL)
		return (ERR_SYNTAX_AND);
	if (cur_token->id == AND_OPR && (prev_token->id == GT
		|| prev_token->id == LT))
			return (ERR_SYNTAX_AND);
	if (cur_token->id == AND_OPR && (next_token->str[0]== '&'
		|| next_token->id == OR_OPR || next_token->id == AND_OPR))
			return (ERR_SYNTAX_AND);
	if (cur_token->id == AND_OPR && next_token->id == PAR_CLOSE)
		return (ERR_SYNTAX_AND);
	return(PARSING_OK);
}

/**
 * @brief Syntax checking for parentheses (`(` and `)`).
 *
 * This function performs syntax checks for the parentheses tokens (`(` and `)`),
 * and ensures they are correctly placed within the shell command structure. It checks the following cases:
 *
 * - An opening parenthesis (`(`) cannot be followed by an AND operator (`&&`).
 * - An opening parenthesis (`(`) cannot be followed by an OR operator (`||`).
 * - An opening parenthesis (`(`) cannot be followed by a pipe (`|`).
 *
 * If any of these invalid cases are found, an error will be returned.
 *
 * @param prev_token Previous token in the token list (may be NULL).
 * @param cur_token Current token (either `(` or `)`).
 * @param env_list The environment list (unused).
 * 
 * @return Returns `PARSING_OK` if the parentheses syntax is valid,
 *         or an appropriate error code if invalid syntax is detected:
 *         - `ERR_SYNTAX_AND` if an opening parenthesis is followed by an AND operator.
 *         - `ERR_SYNTAX_OR` if an opening parenthesis is followed by an OR operator.
 *         - `ERR_SYNTAX_PIPE` if an opening parenthesis is followed by a pipe.
 */
int	syntax_parens(t_token *prev_token, t_token *cur_token, t_env *env_list)
{
	t_token	*next_token;
	(void) prev_token;
	(void) env_list;
	(void) cur_token;
	next_token = skip_whitespace_and_get_next_token(cur_token);
	if (next_token)
	{
		if (cur_token->id == PAR_OPEN && next_token->id == AND_OPR)
			return (ERR_SYNTAX_AND);
		if (cur_token->id == PAR_OPEN && next_token->id == OR_OPR)
			return (ERR_SYNTAX_OR);
		if (cur_token->id == PAR_OPEN && next_token->id == PIPE)
			return (ERR_SYNTAX_PIPE);
	}
	return (PARSING_OK);
}


int	remove_delimiter_quotes(t_token *delimiter_token)
{
	delimiter_token->id = WORD;
		if (safe_assign_str(&(delimiter_token->str), (ft_substr(delimiter_token->str, 1, (ft_strlen(delimiter_token->str) - 2)))) != SUCCESS)
			return (ERR_MEM);
	return (PARSING_OK);
}

int	syntax_redir(t_token *prev_token, t_token *cur_token, t_env *env_list)
{
	t_token	*next_token;
	(void)env_list;
	(void) prev_token;
	next_token = skip_whitespace_and_get_next_token(cur_token);  //next token
	if (!next_token)
		return (ERR_SYNTAX_NL);
	if (next_token->id == PAR_OPEN)
		return (ERR_SYNTAX_UNEXPECT_OPEN);
	if (next_token->id == PAR_CLOSE)
		return (ERR_SYNTAX_UNEXPECT_CLOSE);
 	if (next_token->id == PIPE || next_token->id == OR_OPR)
		return (ERR_SYNTAX_PIPE);
	if (next_token->id == AND_OPR)
		return (ERR_SYNTAX_AND);
	if (next_token->id == GT || next_token->id == LT)
		return (ERR_SYNTAX_REDIR);
	if (cur_token->id == LT && (next_token->id == DQUOTE || next_token->id == DQUOTE))
		return (ERR_SYNTAX_ERROR);
	if (cur_token->id == GT && (next_token->id == SQUOTE || next_token->id == DQUOTE))
		if (remove_delimiter_quotes(next_token) != SUCCESS)
			return (ERR_MEM);
	if (next_token->id != WORD && next_token->id != DQUOTE)
		return (ERR_SYNTAX_ERROR);
	return (PARSING_OK);
}

/**
 * @brief Checks the syntax for word tokens in the shell command.
 * 
 * This function verifies that the `WORD` token is not followed by a `PAR_OPEN`
 * in a way that would create a syntax error. If a `WORD` token is followed by
 * an open parenthesis `(`, it will check for subsequent tokens to ensure proper
 * syntax. If the next token is `PAR_OPEN` without any token after it or an 
 * unexpected opening parenthesis, an error is returned.
 * 
 * @param prev_token The previous token before the current token.
 * @param cur_token The current token being evaluated.
 * @param env_list The environment list (not used in this function).
 * 
 * @return PARSING_OK if the syntax is valid, or an error code if the syntax is incorrect.
 * 
 * Error cases:
 * - If the `WORD` token is immediately followed by a `PAR_OPEN` and there is no next token,
 *   it returns `ERR_SYNTAX_NL`.
 * - If the `WORD` token is immediately followed by a `PAR_OPEN` and the next token is not valid,
 *   it returns `ERR_SYNTAX_UNEXPECT_OPEN`.
 */
int	syntax_word(t_token *prev_token, t_token *cur_token, t_env *env_list)
{
	t_token	*next_token;
	(void) prev_token;
	(void) env_list;
	(void) cur_token;
	next_token = skip_whitespace_and_get_next_token(cur_token);
	if (next_token)
	{
		if (cur_token->id == WORD && next_token->id == PAR_OPEN)
		{
			if (next_token->next == NULL)
				return (ERR_SYNTAX_NL);
			else
				return (ERR_SYNTAX_UNEXPECT_OPEN);
		}
	}
	return (PARSING_OK);
}
