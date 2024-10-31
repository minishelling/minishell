#include "../../include/minishell.h"

t_ecode_p			append(t_shell *shell);
static t_ecode_p	join_quotes_tokens(t_shell *shell);
static t_ecode_p	join_env_var_quotes_and_word_str(t_shell *shell);
static t_ecode_p	join_strs_free_cur_token(t_token **prev_token, \
	t_token **cur_token);
static void			remove_space_tokens(t_token **list, t_token *prev);

/**
 * @brief Appends tokens by joining quotes and environment variables.
 *
 * This function performs a series of operations to join tokens that are
 * quoted or represent environment variables, while also removing
 * unnecessary space tokens from the token list.
 *
 * @param shell Pointer to the shell structure containing token information.
 * @return The result of the parsing operation, indicating success or failure.
 */
t_ecode_p	append(t_shell *shell)
{
	t_ecode_p	err_no;

	err_no = join_quotes_tokens(shell);
	if (err_no)
		return (free_token_list(&shell->token), err_no);
	err_no = join_env_var_quotes_and_word_str(shell);
	if (err_no)
		return (free_token_list(&shell->token), err_no);
	remove_space_tokens(&shell->token, NULL);
	return (PARSING_OK);
}

/**
 * @brief Joins consecutive quote tokens into a single token.
 *
 * This function traverses the token list and joins consecutive single
 * or double quote tokens. It also joins a single quote token with
 * a double quote token if they are adjacent. This operation is performed
 * before removing space tokens to ensure that spaces between tokens are
 * preserved, allowing for a distinction between cases like "$HOME $TERM"
 * and "$HOME$TERM".
 *
 * @param shell Pointer to the shell structure containing token information.
 * @return The result of the operation, indicating success or failure.
 */
static t_ecode_p	join_quotes_tokens(t_shell *shell)
{
	t_token		*cur_token;
	t_token		*prev_token;
	t_ecode_p	err_no;

	prev_token = shell->token;
	cur_token = shell->token->next;
	while (prev_token && cur_token)
	{
		if ((prev_token->id == SQUOTE && cur_token->id == SQUOTE) \
			|| (prev_token->id == DQUOTE && cur_token->id == DQUOTE) \
			|| (prev_token->id == SQUOTE && cur_token->id == DQUOTE) \
			|| (prev_token->id == DQUOTE && cur_token->id == SQUOTE))
		{
			err_no = join_strs_free_cur_token(&prev_token, &cur_token);
			if (err_no)
				return (err_no);
		}
		else
		{
			prev_token = cur_token;
			cur_token = cur_token->next;
		}
	}
	return (PARSING_OK);
}

/**
 * @brief Joins consecutive tokens of words, environment variables,
 *        and quotes into a single token.
 *
 * This function traverses the token list and concatenates adjacent tokens 
 * that are either words, environment variables, or single/double quotes 
 * into a single token. This ensures correct handling during command 
 * expansion in parsing.
 *
 * @param shell Pointer to the shell structure containing token information.
 * @return The result of the operation, indicating success or failure.
 */
static t_ecode_p	join_env_var_quotes_and_word_str(t_shell *shell)
{
	t_token		*prev_token;
	t_token		*cur_token;
	t_ecode_p	err_no;

	prev_token = shell->token;
	cur_token = shell->token->next;
	while (prev_token && cur_token)
	{
		if ((prev_token->id == WORD || prev_token->id == ENV_VAR \
		|| prev_token->id == SQUOTE || prev_token->id == DQUOTE) \
		&& (cur_token->id == WORD || cur_token->id == ENV_VAR \
		|| cur_token->id == SQUOTE || cur_token->id == DQUOTE))
		{
			err_no = join_strs_free_cur_token(&prev_token, &cur_token);
			if (err_no)
				return (err_no);
		}
		else
		{
			prev_token = cur_token;
			cur_token = cur_token->next;
		}
	}
	return (PARSING_OK);
}

/**
 * @brief Joins two token strings into the previous token and frees the current.
 *
 * This function concatenates the string of the current token to the string 
 * of the previous token. It frees the memory allocated for the current 
 * token and updates the linked list accordingly.
 *
 * @param prev_token Pointer to the previous token, which will hold the 
 *                   joined string.
 * @param cur_token Pointer to the current token, which will be freed.
 * @return The result of the operation, indicating success or failure.
 */
static t_ecode_p	join_strs_free_cur_token(t_token **prev_token, \
	t_token **cur_token)
{
	char	*joined_str;
	t_token	*temp;

	temp = (*cur_token)->next;
	joined_str = ft_strjoin((*prev_token)->str, (*cur_token)->str);
	if (!joined_str)
		return (ERR_MEM);
	if (safe_assign_str(&(*prev_token)->str, joined_str) != SUCCESS)
		return (free(joined_str), ERR_MEM);
	free(joined_str);
	free_token(&(*cur_token));
	*cur_token = temp;
	(*prev_token)->next = *cur_token;
	return (PARSING_OK);
}

/**
 * @brief Removes space and newline tokens from the token list.
 *
 * This function traverses the token list and removes tokens identified 
 * as spaces, tabs, or newlines. It updates the linked list accordingly. 
 * After the concatenation of tokens in previous parsing steps, these 
 * tokens are no longer needed.
 *
 * @param list Pointer to the head of the token list.
 * @param prev Pointer to the previous token, used for updating the list 
 *             during removal.
 */
static void	remove_space_tokens(t_token **list, t_token *prev)
{
	t_token	*current;
	t_token	*temp;

	current = *list;
	while (current != NULL)
	{
		if (current->id == SPACE_CHAR || current->id == TAB_CHAR \
			|| current->id == NL)
		{
			if (current->id == NL)
				ft_putstr_fd("We don't treat NL as metachars here.\n", 2);
			if (prev == NULL)
				*list = current->next;
			else
				prev->next = current->next;
			temp = current;
			current = current->next;
			free_token(&temp);
		}
		else
		{
			prev = current;
			current = current->next;
		}
	}
}
