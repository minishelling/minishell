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
	t_token *current;
	t_token *prev;

	current = start_token;
	prev = NULL;
	while (current)
	{
		if (current == token_to_remove)
		{
			if (prev == NULL)  // Removing the first token
				start_token = current->next;
			else               // Removing a token in the middle or end
				prev->next = current->next;
			if (current->str)
			{
				free(current->str);
				current->str = NULL;
			}
			free(current);       // Free the token itself
			return start_token;  // Return the updated list head
		}
		prev = current;
		current = current->next;
	}
	return start_token;
}

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
			free_token(temp);
		}
		else
		{
			prev = current;
			current = current->next;
		}
	}
}



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
	outer_open->id = ARITH_EXPAN;
	outer_open->str = "((";
	outer_close->id = ARITH_EXPAN;
	outer_close->str = "))";
	// printf("after handling arith expan:\n");
	// print_token(*head);
	return (*head);
}
