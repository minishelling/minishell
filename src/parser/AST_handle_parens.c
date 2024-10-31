#include "../../include/minishell.h"

t_token	*ignore_first_parens(t_token *start_token, t_token **end_token);
t_token	*get_matching_paren(t_token *start_token);
t_tree	*process_arith_expan(t_shell *shell, t_token *start_token, \
	t_token *end_token);

/**
 * @brief Ignores the first set of parentheses in the token sequence.
 *
 * This function uses the get_matching_paren function to find the 
 * matching closing parenthesis for the given opening parenthesis. 
 * It sets the end_token to the token before the matching closing 
 * parenthesis and returns the token following the opening parenthesis.
 *
 * @param start_token Pointer to the opening parenthesis token.
 * @param end_token Pointer to a pointer for the last token, 
 *                  set to the token before the matching closing parenthesis.
 * @return Pointer to the token following the opening parenthesis.
 */
t_token	*ignore_first_parens(t_token *start_token, t_token **end_token)
{
	t_token	*matching_paren;

	matching_paren = get_matching_paren(start_token);
	start_token = start_token->next;
	if (matching_paren)
		*end_token = non_null_previous(start_token, matching_paren);
	return (start_token);
}

/**
 * @brief Finds the matching closing parenthesis for a given opening parenthesis.
 *
 * This function traverses the token list starting from the given
 * opening parenthesis and counts nested parentheses to find the
 * corresponding closing parenthesis. If a matching closing parenthesis
 * is found, it returns the token; otherwise, it returns NULL.
 *
 * @param start_token Pointer to the opening parenthesis token.
 * @return Pointer to the matching closing parenthesis token, or NULL 
 *         if not found.
 */
t_token	*get_matching_paren(t_token *start_token)
{
	t_token	*cur_token;
	int		parens;

	cur_token = start_token;
	parens = 1;
	while (cur_token->next && parens)
	{
		if (cur_token->next->id == PAR_OPEN)
			parens++;
		else if (cur_token->next->id == PAR_CLOSE)
		{
			parens--;
			if (parens == 0)
				return (cur_token->next);
		}
		cur_token = cur_token->next;
	}
	return (NULL);
}

/**
 * @brief Processes arithmetic expansions within parentheses.
 *
 * This function modifies the start and end tokens to signify they are 
 * part of an arithmetic expansion. It assigns specific strings to 
 * both tokens and creates a leaf node to represent this expansion.
 *
 * @param shell Pointer to the shell structure.
 * @param start_token Pointer to the token representing the start of 
 *                    the arithmetic expansion.
 * @param end_token Pointer to the token representing the end of 
 *                  the arithmetic expansion.
 * @return Pointer to the created leaf node representing the arithmetic 
 *         expansion.
 */
t_tree	*process_arith_expan(t_shell *shell, \
	t_token *start_token, t_token *end_token)
{
	if (safe_assign_str(&(start_token->str), "((") != SUCCESS \
	|| safe_assign_str(&(end_token->str), "))") != SUCCESS)
	{
		handle_parsing_err(shell, ERR_MEM);
		clean_nicely_and_exit(shell, EXIT_FAILURE);
	}
	start_token->id = ARITH_EXPAN;
	end_token->id = ARITH_EXPAN;
	return (init_leaf_node(shell, start_token, end_token));
}
