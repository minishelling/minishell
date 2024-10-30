#include "../../../include/minishell.h"

t_token	*skip_whitespace_and_get_next_token(t_token *token);
t_token	*get_after_matching_arith_expan(t_token *first_arith_expan_token);
t_token	*non_null_previous(t_token *list, t_token *before_what);

/**
 * @brief Skips whitespace tokens and retrieves the next token.
 *
 * This function iterates through a linked list of tokens, starting from 
 * the given token, and skips any tokens that represent whitespace 
 * characters. It returns the first non-whitespace token encountered.
 *
 * This function is used in syntax functions, as whitespace tokens will 
 * be removed later in the append process.
 *
 * @param token Pointer to the current token from which to start skipping.
 * @return Pointer to the next non-whitespace token, or NULL if no such 
 *         token exists.
 */
t_token	*skip_whitespace_and_get_next_token(t_token *token)
{
	t_token	*return_token;

	if (!token)
		return (NULL);
	return_token = token->next;
	while (return_token && (return_token->id == SPACE_CHAR
			|| return_token->id == TAB_CHAR || return_token->id == NL))
		return_token = return_token->next;
	return (return_token);
}

/**
 * @brief Retrieves the token following the matching arithmetic expansion token.
 *
 * This function traverses the linked list of tokens starting from 
 * the first arithmetic expansion token. It continues until it 
 * finds the next token that follows the matching arithmetic parentheses 
 * or reaches the end of the list.
 *
 * This function is called in `traverse_tokens_to_make_cmd` during the 
 * parsing process to handle arithmetic expansions.
 *
 * @param first_arith_expan_token Pointer to the first arithmetic 
 *                                  expansion token.
 * @return Pointer to the token following the arithmetic expansion, or 
 *         NULL if no such token exists.
 */
t_token	*get_after_matching_arith_expan(t_token *first_arith_expan_token)
{
	t_token	*cur_token;

	cur_token = first_arith_expan_token->next;
	while (cur_token && cur_token->id && cur_token->id != ARITH_EXPAN)
		cur_token = cur_token->next;
	return (cur_token);
}

/**
 * @brief Finds the token immediately preceding the specified token in the list.
 *
 * This function traverses the linked list of tokens to locate the token 
 * that comes before the specified token (`before_what`). It returns the 
 * found token or the first token in the list if the specified token 
 * is not found.
 *
 * This function is used to find the end token of the left-side portion 
 * of a logical operation in the `make_tree` function. It also helps 
 * determine the end token after ignoring the first parentheses, and 
 * assists in deciding the nature of the syntax message if there are 
 * more open parentheses than close parentheses.
 *
 * @param list Pointer to the head of the token list.
 * @param before_what Pointer to the token for which the preceding token is sought.
 * @return Pointer to the token preceding `before_what`, or the first token 
 *         in the list if `before_what` is not found.
 */
t_token	*non_null_previous(t_token *list, t_token *before_what)
{
	t_token	*return_token;

	return_token = list;
	while (return_token->next && return_token->next != before_what)
		return_token = return_token->next;
	return (return_token);
}
