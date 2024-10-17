#include "../../include/minishell.h"

void free_token(t_token **token)
{
	if (token && *token)
	{
		if ((*token)->str)
		{
			free((*token)->str);
			(*token)->str = NULL;
		}
		else
		free(*token);
		*token = NULL;
	}
}

void	free_token_list(t_token **token_list)
{
	t_token *current;
	t_token *next;

	if (!token_list || !(*token_list))
		return;
	current = *token_list;
	while (current)
	{
		next = current->next;
		free_token(&current);
		current = next;
	}
	*token_list = NULL;
}

/**
 * @brief Removes a specific token from the linked list.
 *
 * This function removes the given token from the linked list and returns
 * the updated list head. If the token to be removed is the first token, 
 * it updates the head of the list. The token is freed after removal.
 *
 * @param start_token The head of the token list.
 * @param token_to_remove The token to be removed.
 * @return The updated head of the list.
 */
t_token *remove_token(t_token *start_token, t_token *token_to_remove)
{
	t_token *current;
	t_token *prev;

	current = start_token;
	prev = NULL;
	if (current == NULL || token_to_remove == NULL)
		return start_token;
	while (current)
	{
		if (current == token_to_remove)
		{
			if (prev == NULL)
				start_token = current->next;
			else
				prev->next = current->next;
			free_token(&current);
			return start_token;
		}
		prev = current;
		current = current->next;
	}
	return start_token;
}

