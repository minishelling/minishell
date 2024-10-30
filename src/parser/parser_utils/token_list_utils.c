#include "../../../include/minishell.h"

t_token	*new_token(void);
t_token	*last_token(t_token *list);
t_token	*copy_token(t_token *token);
void	add_token_in_back(t_token **list, t_token *new_token);

/**
 * @brief Creates a new token.
 *
 * This function allocates memory for a new token of type `t_token` and 
 * initializes its fields. The `id` is set to -1, `str` is set to NULL, 
 * and `next` is initialized to NULL.
 *
 * @return A pointer to the newly created token, or NULL if memory 
 *         allocation fails.
 */
t_token	*new_token(void)
{
	t_token	*new;

	new = ft_calloc(1, sizeof(t_token));
	if (!new)
		return (NULL);
	new->next = NULL;
	new->id = -1;
	new->str = NULL;
	return (new);
}

/**
 * @brief Retrieves the last token in a linked list of tokens.
 *
 * This function traverses the linked list of tokens until it finds the 
 * last token (the one that has a NULL next pointer). If the list is empty, 
 * it returns NULL.
 *
 * @param list Pointer to the head of the token list.
 * @return Pointer to the last token in the list, or NULL if the list 
 *         is empty.
 */
t_token	*last_token(t_token *list)
{
	t_token	*cur_token;

	if (!list)
		return (NULL);
	cur_token = list;
	while (cur_token->next)
		cur_token = cur_token->next;
	return (cur_token);
}

/**
 * @brief Creates a copy of a token.
 *
 * This function duplicates an existing token by allocating memory for a 
 * new token and copying the `id` and `str` fields from the provided 
 * token. The `next` pointer of the new token is initialized to NULL. 
 * If memory allocation fails for either the new token or its string, 
 * the function will free any allocated memory and return NULL.
 *
 * @param token Pointer to the token to be copied.
 * @return Pointer to the newly created token, or NULL if memory 
 *         allocation fails.
 */
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
	if (!dup_token->str)
		return (free_token(&dup_token), NULL);
	dup_token->next = NULL;
	return (dup_token);
}

/**
 * @brief Adds a token to the end of a linked list of tokens.
 *
 * This function adds a new token to the end of the linked list. If the 
 * list is empty, it sets the new token as the head of the list.
 *
 * @param list Pointer to a pointer to the head of the token list. 
 *              The head of the list will be updated if it's currently NULL.
 * @param new_token Pointer to the new token to be added.
 */
void	add_token_in_back(t_token **list, t_token *new_token)
{
	if (!new_token)
		return ;
	if (!(*list))
	{
		*list = new_token;
		return ;
	}
	last_token(*list)->next = new_token;
	return ;
}
