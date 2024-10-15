#include "../../include/minishell.h"

/**
 * @brief Frees the memory used by a token, including its string and structure.
 *
 * This function frees the memory allocated for the `str` field of the given token,
 * then frees the token structure itself.
 * The token pointer is set to NULL after freeing, but this does not affect the caller's 
 * reference to the token.
 *
 * @param token The token to free. If NULL, no operation is performed.
 */
void free_token(t_token *token)
{
	if (token == NULL)
		return;
	if (token->str)
	{
		free(token->str);
		token->str = NULL;
	}
	free(token);
	token = NULL;
}

/**
 * @brief Frees a linked list of tokens.
 *
 * This function iterates through a linked list of tokens, freeing each token and its associated
 * memory (string and structure). After the entire list is freed, the list pointer is set to NULL.
 *
 * @param token_list A pointer to the head of the token list to free. If NULL, no operation is performed.
 */
void free_token_list(t_token *token_list)
{
	t_token *temp;
	
	if (!token_list)
		return;
	while (token_list)
	{
		temp = token_list;
		token_list = token_list->next;
		free_token(temp);
	}
	token_list = NULL;
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
			free_token(current);
			return start_token;
		}
		prev = current;
		current = current->next;
	}
	return start_token;
}

