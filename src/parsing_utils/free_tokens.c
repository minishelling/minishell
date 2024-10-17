#include "../../include/minishell.h"

void free_token2(t_token **token)
{
    if (token && *token)
    {
        if ((*token)->str)
        {
            printf("Freeing token string at %p: '%s'\n", (void*)((*token)->str), (*token)->str);
            free((*token)->str);
            (*token)->str = NULL;
        }
        else
            printf("Token string at %p is already freed or NULL\n", (void*)((*token)->str));
        printf("Freeing token struct at %p\n", (void*)(*token));
        free(*token);
        *token = NULL;
    }
    else
        printf("Token or token pointer is NULL or already freed, nothing to free.\n");
}


void	free_token_list2(t_token **token_list)
{
    t_token *current;
    t_token *next;

    if (!token_list || !(*token_list))
    {
        printf("Token list is NULL or already freed, nothing to free.\n");
        return;
    }

    printf("Freeing token list at %p\n", (void*)*token_list);
    current = *token_list;
    while (current)
    {
        next = current->next;
        printf("Freeing token at %p\n", (void*)current);
        free_token2(&current);
        current = next;
    }
    *token_list = NULL;
    printf("Token list has been freed and set to NULL.\n");
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
			free_token2(&current);
			return start_token;
		}
		prev = current;
		current = current->next;
	}
	return start_token;
}

