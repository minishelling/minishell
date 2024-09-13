#include "../../include/minishell.h"

bool	is_word(t_token *token)
{
	if (token == NULL)
		return (false);
	if (token->id == WORD)
		return (true);
	return (false);
}

t_token	*remove_empty_token(t_token *token)
{
	t_token	*current_token;
	t_token	*previous_token;

	if (token == NULL)
		return (NULL);
	current_token = token;
	previous_token = NULL;
	while (current_token != NULL)
	{
		if (current_token->str != NULL)
		{
			previous_token = current_token;
			current_token = current_token->next;
			continue ;
		}
		current_token = free_token_node(current_token);
		if (previous_token == NULL)
			token = current_token;
		else
			previous_token->next = current_token;
	}
	return (token);
}

bool	join_word_tokens(t_shell *shell)
{
	t_token	*current_token;
	t_token	*previous_token;

	shell->token = remove_empty_token(shell->token);
	current_token = shell->token;
	previous_token = NULL;
	while (current_token != NULL)
	{
		if (is_word(previous_token) && is_word(current_token))
		{
			previous_token->str = ft_strjoin_fs1(&previous_token->str, current_token->str);
			if (previous_token->str == NULL)
				return (false);
			current_token = free_token_str(current_token);
			previous_token->next = current_token;
			continue ;
		}
		previous_token = current_token;
		current_token = current_token->next;
	}
	return (true);
}

