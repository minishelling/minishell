#include "../../include/minishell.h"

t_ecode_p	syntax(t_shell *shell);
t_ecode_p	process_syntax_checks(t_shell *shell, t_token **prev_token);
void		initialize_syntaxing_funcs(t_syntax_func check_syntax[15]);
t_ecode_p	check_parens(t_token *list);
t_ecode_p	check_quotes(t_token *list);

/**
 * @brief Processes syntax checks on the token list.
 *
 * Iterates through the token list and applies the corresponding syntax 
 * checks for each token. It first verifies that there are no more 
 * closing parentheses than opening ones. Then, it checks the balance of 
 * parentheses throughout the entire list and ensures that quotes are 
 * correctly opened and closed by invoking relevant syntax checking functions.
 * After processing all tokens, it checks for the presence of ampersands, 
 * as background process execution is not handled in this implementation.
 *
 * @param shell Pointer to the shell structure containing the token list.
 * @param prev_token Pointer to the previous token for syntax checks (not used).
 * 
 * @return 
 * - PARSING_OK if successful, or an error code if any syntax issue is detected,
 *   including ERR_BG_PROCESS if an ampersand is found in the token list.
 */
t_ecode_p	syntax(t_shell *shell)
{
	t_token		*prev_token;
	t_token		*cur_token;
	t_ecode_p	err_no;

	prev_token = NULL;
	err_no = process_syntax_checks(shell, &prev_token);
	if (err_no)
		return (err_no);
	err_no = check_parens(shell->token);
	if (err_no)
		return (err_no);
	err_no = check_quotes(shell->token);
	if (err_no)
		return (err_no);
	cur_token = shell->token;
	while (cur_token)
	{
		if (cur_token->id == AMPERSAND)
			return (ERR_BG_PROCESS);
		cur_token = cur_token->next;
	}
	return (PARSING_OK);
}

/**
 * @brief Processes syntax checks on the token list.
 *
 * Iterates through the token list and applies the corresponding syntax 
 * checks for each token. Updates the previous token pointer and tracks 
 * the balance of parentheses.
 *
 * @param shell Pointer to the shell structure containing the token list.
 * @param prev_token Pointer to the previous token for syntax checks.
 * @return t_ecode_p PARSING_OK if successful, or an error code if any
 *         syntax issue is detected.
 */
t_ecode_p	process_syntax_checks(t_shell *shell, t_token **prev_token)
{
	t_token			*current_token;
	t_syntax_func	check_syntax[15];
	t_ecode_p		err_no;
	int				par_count;

	par_count = 0;
	initialize_syntaxing_funcs(check_syntax);
	current_token = shell->token;
	while (current_token)
	{
		err_no = check_syntax[current_token->id] \
		(*prev_token, current_token, &par_count);
		if (err_no)
			return (err_no);
		*prev_token = current_token;
		current_token = skip_whitespace_and_get_next_token(current_token);
	}
	return (PARSING_OK);
}

/**
 * @brief Initializes the syntax check functions.
 *
 * Sets up an array of function pointers for syntax checks corresponding 
 * to different token IDs.
 *
 * @param check_syntax Array of function pointers for syntax checking.
 */
void	initialize_syntaxing_funcs(t_syntax_func check_syntax[15])
{
	check_syntax[0] = syntax_noop;
	check_syntax[1] = syntax_noop;
	check_syntax[2] = syntax_noop;
	check_syntax[3] = syntax_pipe;
	check_syntax[4] = syntax_and_opr;
	check_syntax[5] = syntax_open_paren;
	check_syntax[6] = syntax_close_paren;
	check_syntax[7] = syntax_redir;
	check_syntax[8] = syntax_redir;
	check_syntax[9] = syntax_quote;
	check_syntax[10] = syntax_quote;
	check_syntax[11] = syntax_env_var;
	check_syntax[12] = syntax_word;
	check_syntax[13] = syntax_or_opr;
	check_syntax[14] = syntax_ampersand;
}

/**
 * @brief Checks the balance of parentheses in the token list.
 *
 * This function traverses the linked list of tokens and checks for 
 * balanced parentheses by maintaining a balance counter. It increments 
 * the counter for each open parenthesis and decrements it for each 
 * close parenthesis. If the counter is greater than zero at the end 
 * of the traversal, it indicates that there are unmatched open 
 * parentheses, returning an error code for parsing.
 *
 * This function complements `syntax_open_paren`, which checks if an 
 * open parenthesis is followed by an allowed token or is left dangling. 
 * It also works in conjunction with `update_parens_balance` and 
 * `syntax_close_paren`, which handle the opposite situation of closing 
 * parentheses and their balance.
 *
 * @param list Pointer to the head of the token list.
 * @return An error code indicating the result of the check: 
 *         `ERR_PARSING_ERROR` if there are unmatched open parentheses, 
 *         or `PARSING_OK` if the parentheses are balanced.
 */
t_ecode_p	check_parens(t_token *list)
{
	int		balance;
	t_token	*cur_token;

	balance = 0;
	cur_token = list;
	while (cur_token)
	{
		if (cur_token->id == PAR_OPEN)
			balance++;
		else if (cur_token->id == PAR_CLOSE)
			balance--;
		cur_token = cur_token->next;
	}
	if (balance > 0)
		return (ERR_PARSING_ERROR);
	return (PARSING_OK);
}

/**
 * @brief Checks for balanced quotes in the token list.
 *
 * Iterates through the token list to ensure that every opening quote 
 * (single or double) has a corresponding closing quote. If a token 
 * is found with an unmatched quote or if a quote token consists of 
 * only a single character, an error code is returned.
 *
 * @param list Pointer to the token list to check.
 * @return t_ecode_p PARSING_OK if all quotes are balanced, or an 
 *         error code (ERR_UNCLOSED_QUOTES) if any unmatched quotes 
 *         are found.
 */
t_ecode_p	check_quotes(t_token *list)
{
	t_token	*cur_token;
	size_t	str_len;
	char	*str;

	cur_token = list;
	while (cur_token)
	{
		if (cur_token->id == DQUOTE || cur_token->id == SQUOTE)
		{
			str_len = ft_strlen(cur_token->str);
			str = cur_token->str;
			if (str_len == 1 || str[0] != str[str_len - 1])
				return (ERR_UNCLOSED_QUOTES);
		}
		cur_token = cur_token->next;
	}
	return (PARSING_OK);
}
