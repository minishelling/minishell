#include "../../include/minishell.h"

bool	is_word(t_token *token)
{
	if (token == NULL)
		return (false);
	if (token->id == WORD)
		return (true);
	return (false);
}

bool	is_env_var(t_token *token)
{
	if (token == NULL)
		return (false);
	if (token->id == ENV_VAR)
		return (true);
	return (false);
}

bool	is_squote(t_token *token)
{
	if (token == NULL)
		return (false);
	if (token->id == SQUOTE)
		return (true);
	return (false);
}

bool	is_dquote(t_token *token)
{
	if (token == NULL)
		return (false);
	if (token->id == DQUOTE)
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

t_token *free_token(t_token *token)
{
    t_token *next_token;

    if (token == NULL)
        return (NULL);
    
    next_token = token->next;  // Save the pointer to the next token before freeing
    // if (token && token->str)
	// 	free(token->str);          // Free the token's string
    if (token)
		free(token);               // Free the token itself

    return (next_token);       // Return the next token in the list
}


bool join_quotes_tokens(t_shell *shell)
{
    t_token *current_token = shell->token;
    t_token *previous_token = NULL;

    shell->token = remove_empty_token(shell->token);  // Remove any empty tokens

    while (current_token != NULL)
    {
        // Case 1: Previous and current tokens are double quotes
        if (is_dquote(previous_token) && is_dquote(current_token))
        {
            // Remove the inner quotes: closing quote from previous and opening quote from current
            size_t prev_len = ft_strlen(previous_token->str);
            if (prev_len > 0 && previous_token->str[prev_len - 1] == '"')
                previous_token->str[prev_len - 1] = '\0'; // Null-terminate to remove

            // Skip the first quote of current_token
            if (current_token->str[0] == '"')
                current_token->str++; // Skip the first quote

            // Concatenate the two token strings
            char *new_str = ft_strjoin_fs1(&previous_token->str, current_token->str);
            if (!new_str)
                return (false);
            previous_token->str = new_str;

            // Free the current token
            t_token *temp = current_token;
            current_token = current_token->next; // Move to the next token
            free_token(temp); // Free the current token
            previous_token->next = current_token; // Link previous token to the next
            continue;
        }

        // Case 2: Previous token is single quote and current is single quote
        if (is_squote(previous_token) && is_squote(current_token))
        {
            // Remove the inner quotes: closing quote from previous and opening quote from current
            size_t prev_len = ft_strlen(previous_token->str);
            if (prev_len > 0 && previous_token->str[prev_len - 1] == '\'')
                previous_token->str[prev_len - 1] = '\0'; // Null-terminate to remove

            // Skip the first quote of current_token
            if (current_token->str[0] == '\'')
                current_token->str++; // Skip the first quote

            // Concatenate the two token strings
            char *new_str = ft_strjoin_fs1(&previous_token->str, current_token->str);
            if (!new_str)
                return (false);
            previous_token->str = new_str;

            // Free the current token
            t_token *temp = current_token;
            current_token = current_token->next; // Move to the next token
            free_token(temp); // Free the current token
            previous_token->next = current_token; // Link previous token to the next
            continue;
        }

        // Case 3: Previous is double quote and current is single quote (or vice versa)
        if ((is_dquote(previous_token) && is_squote(current_token)) || 
            (is_squote(previous_token) && is_dquote(current_token)))
        {
            // Concatenate without removing any quotes
            char *new_str = ft_strjoin_fs1(&previous_token->str, current_token->str);
            if (!new_str)
                return (false);
            previous_token->str = new_str;

            // Free the current token
            t_token *temp = current_token;
            current_token = current_token->next; // Move to the next token
            free_token(temp); // Free the current token
            previous_token->next = current_token; // Link previous token to the next
            continue;
        }

        // Update previous and current tokens
        previous_token = current_token;
        current_token = current_token->next;
    }
    return (true);
}









bool	join_word_and_env_var_tokens(t_shell *shell)
{
	t_token	*current_token;
	t_token	*previous_token;

	shell->token = remove_empty_token(shell->token);
	current_token = shell->token;
	previous_token = NULL;
	while (current_token != NULL)
	{
		if ((is_word(previous_token) || is_env_var(previous_token)) && (is_word(current_token) || is_env_var(current_token)))
		{
			previous_token->str = ft_strjoin_fs1(&previous_token->str, current_token->str);
			if (previous_token->str == NULL)
				return (false);
			if (is_env_var(previous_token) || is_env_var(current_token))
				previous_token->id = ENV_VAR;
			current_token = free_token_str(current_token);
			previous_token->next = current_token;
			continue ;
		}
		previous_token = current_token;
		current_token = current_token->next;
	}
	return (true);
}

