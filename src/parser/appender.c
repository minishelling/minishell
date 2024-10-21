#include "../../include/minishell.h"

/**
 * remove_space_tokens - Removes all spacing tokens from the token list.
 * 
 * This function traverses the list of tokens and removes the space, tab, 
 * and newline tokens that serve only to separate tokens. Now that the necessary 
 * strings are concatenated, there is no need for spacing tokens anymore. For 
 * example, `"hello""world"` and `"hello" "world"` will be treated differently. 
 * The memory used by the removed tokens is properly freed.
 *
 * @head: Pointer to the head of the token list.
 */
void	remove_space_tokens(t_token **head)
{
	t_token *current;
	t_token *prev;
	t_token *temp;

	current = *head;
	prev = NULL;

	while (current != NULL)
	{
		if (current->id == SPACE_CHAR || current->id == TAB_CHAR || current->id == NL)
		{
			if (prev == NULL)
				*head = current->next;
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

static	int join_strs_and_free_cur_token(t_token **prev_token, t_token **cur_token)
{
	char	*joined_str;
	t_token	*temp;
	
	temp = (*cur_token)->next;
	joined_str = ft_strjoin((*prev_token)->str, (*cur_token)->str);
	if (!joined_str)
		return (ERR_MEM);
	if (safe_assign_str(&(*prev_token)->str, joined_str) != SUCCESS)
		return(free(joined_str), ERR_MEM);
	free(joined_str);
	free_token(&(*cur_token));
	*cur_token = temp;
	(*prev_token)->next = *cur_token;
	return (PARSING_OK);
}


int	join_env_var_quotes_and_word_str(t_shell *shell)
{
	t_token	*cur_token;
	t_token	*prev_token;
	int		err_no;

	prev_token = shell->token;
	cur_token = shell->token->next;
	while (prev_token && cur_token)
	{
		if ((prev_token->id == WORD || prev_token->id == ENV_VAR ||
			prev_token->id == SQUOTE || prev_token->id == DQUOTE) &&
			(cur_token->id == WORD || cur_token->id == ENV_VAR ||
			cur_token->id == SQUOTE || cur_token->id == DQUOTE))
			{
				err_no = join_strs_and_free_cur_token(&prev_token, &cur_token);
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


int join_quotes_tokens(t_shell *shell)
{
	t_token	*cur_token;
	t_token	*prev_token;
	int		err_no;

	prev_token = shell->token;
	cur_token = shell->token->next;
	while (prev_token && cur_token)
	{
		if ((prev_token->id == SQUOTE && cur_token->id == SQUOTE) ||
			(prev_token->id == DQUOTE && cur_token->id == DQUOTE) ||
			(prev_token->id == SQUOTE && cur_token->id == DQUOTE) ||
			(prev_token->id == DQUOTE && cur_token->id == SQUOTE))
			{
				err_no = join_strs_and_free_cur_token(&prev_token, &cur_token);
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


int append (t_shell *shell)
{
	int err_no;
	
	err_no = join_quotes_tokens(shell);
	if (err_no)
		return (free_token_list(&shell->token), err_no);
	// printf ("after joining quotes tokens:\n");
	// print_token(shell->token);
	
	err_no = join_env_var_quotes_and_word_str(shell);
	if (err_no)
	return (free_token_list(&shell->token), err_no);
	// printf ("after concat quote, words and env_vars tokens\n");
	// print_token(shell->token);
	
	remove_space_tokens(&shell->token);
	// printf ("after removing space tokens\n");
	// print_token(shell->token);
	
	//print_token(shell->token);
	return (PARSING_OK);
}

