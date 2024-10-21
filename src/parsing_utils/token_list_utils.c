#include "../include/minishell.h"

t_token	*new_token(void)
{
	t_token	*new;

	new = malloc(sizeof(t_token));
	if (!new)
		return (NULL);
	new->next = NULL;
	new->id = -1;
	new->str = NULL;
	return (new);
}

t_token	*last_token(t_token *token_list_head)
{
	t_token *token;

	if (token_list_head == NULL)
		return (NULL);

	token = token_list_head;
	while (token->next)
		token = token->next;
	return (token);
}

t_token	*copy_token(t_token *token)
{
	t_token	*dup_token;

	if (!token)
		return (NULL);
	dup_token = new_token();
	if (!dup_token)
		return (NULL);
	dup_token->id = token->id;
	dup_token->str = ft_strdup(token->str);
	if (dup_token->str == NULL)
		return (free(dup_token), NULL);
	dup_token->next = NULL;
	return (dup_token);
}

void	add_token_in_back(t_token **token_list_head, t_token *new_token)
{
	if (!new_token)
		return ;
	if (!(*token_list_head))
	{
		*token_list_head = new_token;
		return ;
	}
	last_token(*token_list_head)->next = new_token;
	return ;
}

t_token *remove_token_by_reference(t_token *start_token, t_token *token_to_remove)
{
	t_token *prev;
	t_token *current;
	
	prev = NULL;
	current = start_token;
	while (current)
	{
		if (current == token_to_remove)
		{
			if (prev == NULL)
				start_token = current->next;
			else
				prev->next = current->next;
			free_token(&current);
			return (start_token);
		}

		// Move to the next token
		prev = current;
		current = current->next;
	}
	return start_token;
}





/**
 * @brief Finds the token immediately before a specified token in the list.
 * 
 * This function traverses the token list starting from the given `start_token`,
 * and returns the token that comes immediately before the specified token (`before_what`).
 * If the specified token is not found in the list,
 * the function returns the first token in the list.
 * 
 * @param start_token The token to start searching from.
 * @param before_what The token that we are searching for its preceding token.
 * 
 * @return The token that comes immediately before the `before_what` token, or the first token
 *         if no valid preceding token is found.
 */
t_token *non_null_previous(t_token *start_token, t_token *before_what)
{
	t_token		*return_token;

	return_token = start_token;
	while (return_token->next && return_token->next != before_what)
		return_token = return_token->next;
	return (return_token);
}

/**
 * @brief Finds the token immediately preceding a specified token in the list.
 * 
 * This function searches the token list starting from the given `head` token and returns
 * the token that is immediately before the specified `target` token. If the `head` is 
 * the same as the `target` token or the `target` token is not found, it returns `NULL`.
 * 
 * @param head The token to start searching from.
 * @param target The token for which the preceding token is being searched.
 * 
 * @return The token that comes immediately before the `target` token, or `NULL` if no
 *         preceding token exists (e.g., if the target is the first token or the target
 *         token is not found).
 */
t_token *previous_token_if_exists(t_token *head, t_token *target)
{
	t_token *current;

	if (head == target)
		return (NULL);
  	current = head;
	while (current && current->next != target)
		current = current->next;
	return current;
}

t_token *handle_arith_expan(t_token **head, t_token **cur_open, t_token **cur_close)
{
	t_token *outer_open;
	t_token *outer_close;
	//printf ("in arith expan\n");
	//printf (">cur_open is %s , cur_close is %s\n", (*cur_open)->str, (*cur_close)->str);
	while (*cur_open && cur_close && (*cur_open)->id == PAR_OPEN && (*cur_close)->id == PAR_CLOSE)
	{
		//printf ("I'm in the while loop\n");
	
		outer_open = (*cur_open);
		outer_close = *cur_close;
		//printf (">outer_open is %s , outer_close is %s\n", outer_open->str, outer_close->str);
		(*cur_open) = previous_token_if_exists(*head, (*cur_open));
		(*cur_close) = (*cur_close)->next;
		// if (cur_open && cur_close)
			//printf (">cur_open is %s , cur_close is %s\n", cur_open->str, cur_close->str);
	}
	//printf ("outer open is %s , outer close %s\n", outer_open->str, outer_close->str);
	if(!safe_assign_str(&outer_open->str, "((") || !safe_assign_str(&outer_close->str, "))"))
		exit (FAILURE); //protect better
	outer_open->id = ARITH_EXPAN;
	outer_close->id = ARITH_EXPAN;
	// printf("after handling arith expan:\n");
	// print_token(*head);
	return (*head);
}
