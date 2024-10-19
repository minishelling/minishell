#include "../../include/minishell.h"

/**
 * @brief Joins two tokens based on their quote types (single or double).
 *
 * This function allocates a new string that combines the strings of the
 * previous token and the current token. It frees the current token after
 * joining.
 *
 * @param previous_token Pointer to a pointer of the previous token.
 * @param current_token Pointer to a pointer of the current token.
 * @return Pointer to the newly allocated string if successful; NULL otherwise.
 */
static char *join_quotes(t_token **prev_token, t_token **cur_token)
{
	size_t	len_prev;
	size_t	len_cur;
	char	*new_str;

	len_prev = ft_strlen((*prev_token)->str);
	len_cur = ft_strlen((*cur_token)->str);

	new_str = malloc(len_prev + len_cur + 1);
	if (!new_str)
		return NULL; // Handle allocation failure
	ft_strlcpy(new_str, (*prev_token)->str, len_prev + 1);
	ft_strlcat(new_str, (*cur_token)->str, len_prev + len_cur + 1);
	free(*cur_token);
	return new_str;
}

/**
 * @brief Joins tokens that are enclosed in quotes.
 *
 * This function iterates through the token list in the shell structure,
 * checking if consecutive tokens are quoted. If they are, it joins them
 * using the join_quotes function.
 *
 * @param shell Pointer to the shell structure containing the token list.
 * @return true if all operations succeeded; false otherwise.
 */
bool join_quotes_tokens(t_shell *shell)
{
	t_token *cur_token;
	t_token *prev_token;
	char *new_str;

	prev_token = NULL;
	cur_token = shell->token;

	while (cur_token)
	{
	   	new_str = NULL;
		if (is_dquote(prev_token) && is_dquote(cur_token))
			new_str = join_quotes(&prev_token, &cur_token);
		else if (is_squote(prev_token) && is_squote(cur_token))
			new_str = join_quotes(&prev_token, &cur_token);
		else if ((is_dquote(prev_token) && is_squote(cur_token)) ||
				 (is_squote(prev_token) && is_dquote(cur_token)))
			new_str = join_quotes(&prev_token, &cur_token);
		if (new_str)
			safe_assign_str(&prev_token->str, new_str); // protect better
		else
			return false;
		prev_token = cur_token;
		cur_token = cur_token->next;
	}
	return true;
}

void process_and_remove_parens(t_shell *shell, t_token *current, t_token *word_token)
{
	t_token *token_after_par = word_token->next;
	t_token *token_before_par = previous_token_if_exists(shell->token, current);

	// Remove both opening and closing parentheses
	shell->token = remove_token_by_reference(shell->token, current);   // Remove PAR_OPEN
	shell->token = remove_token_by_reference(shell->token, word_token);  // Remove PAR_CLOSE

	// Handle arithmetic expansion if both parentheses are balanced
	if (token_before_par && token_before_par->id == PAR_OPEN &&
		token_after_par && token_after_par->id == PAR_CLOSE)
			handle_arith_expan(&shell->token, &token_before_par, &token_after_par);
}

void check_and_process_parens(t_shell *shell, t_token **current_token)
{
	t_token *next_token = (*current_token)->next;

	while (next_token && next_token->id == WORD && next_token->next)
		next_token = next_token->next;

	if (next_token && next_token->id == PAR_CLOSE)
	{
		process_and_remove_parens(shell, *current_token, next_token);
		t_token *token_after_par = next_token->next;
		if (token_after_par)
			*current_token = token_after_par;
		else
			return;
	}
	else
		*current_token = (*current_token)->next;
}

void remove_subshell_parens(t_shell *shell)
{
	t_token *current_token;
	
	current_token = shell->token;
	while (current_token && current_token->next)
	{
		if (current_token->id == PAR_OPEN)
		{
			check_and_process_parens(shell, &current_token);
			if (!current_token)
				break;
		}
		else
			current_token = current_token->next;
	}
}


/**
 * @brief Joins consecutive word or environment variable tokens.
 *
 * This function iterates through the token list in the shell structure,
 * joining consecutive tokens of type word or environment variable. It frees
 * the current token after joining.
 *
 * @param shell Pointer to the shell structure containing the token list.
 * @return true if all operations succeeded; false otherwise.
 */
bool join_env_var_quote_n_word_str(t_shell *shell)
{
	t_token	*cur_token;
	t_token	*prev_token;
	t_token	*temp_token;
	char	*joined_str;

	cur_token = shell->token;
	prev_token = NULL;
	while (cur_token)
	{
		if ((is_word(prev_token) || is_env_var(prev_token) ||
			 is_squote(prev_token) || is_dquote(prev_token)) &&
			(is_word(cur_token) || is_env_var(cur_token) ||
			 is_squote(cur_token) || is_dquote(cur_token)))
		{
			joined_str = ft_strjoin_fs1(&prev_token->str, cur_token->str);
			if (!joined_str)
				return (ERR_MEM);
			safe_assign_str(&prev_token->str, joined_str);  //protect better
			temp_token = cur_token->next;
			free_token(&cur_token);
			prev_token->next = temp_token;
			cur_token = temp_token;
			continue;
		}
		prev_token = cur_token;
		cur_token = cur_token->next;
	}
	return (PARSING_OK);
}

int append (t_shell *shell)
{
	int err_no;
	
	err_no = join_quotes_tokens(shell);
	if (err_no)
		return (free_token_list(&shell->token), err_no);
	// printf ("after joining quotes tokens:\n");
	// print_token(shell->token);
	err_no = join_env_var_quote_n_word_str(shell);
	if (err_no)
	return (free_token_list(&shell->token), err_no);
	// printf ("after concat words and env_vars tokens\n");
	// print_token(shell->token);
	remove_space_tokens(&shell->token);
	// printf ("after removing space tokens\n");
	// print_token(shell->token);
	remove_subshell_parens(shell);
	// printf ("after removing subshell_parens\n");
	// print_token(shell->token);
	return (PARSING_OK);
}

