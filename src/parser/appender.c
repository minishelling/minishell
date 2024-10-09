#include "../../include/minishell.h"


// t_token *free_token(t_token **token)
// {
//     t_token *next_token;

//     if (*token == NULL)
//         return (NULL);

//     next_token = (*token)->next;
//     free(*token);
// 	*token = NULL;
//     return (next_token);
// }


/**
 * @brief Joins two tokens based on their quote types (single or double).
 *
 * This function allocates a new string that combines the strings of the
 * previous token and the current token. It frees the current token after
 * joining.
 *
 * @param previous_token Pointer to a pointer of the previous token.
 * @param current_token Pointer to a pointer of the current token.
 * @return Pointer to the newly allocated string if successful; NULL otherwise.
 */
static char *join_quotes(t_token **previous_token, t_token **current_token)
{
    size_t len_prev;
    size_t len_curr;

    len_prev = strlen((*previous_token)->str);
    len_curr = strlen((*current_token)->str);

    char *new_str = malloc(len_prev + len_curr + 1);
    if (!new_str)
        return NULL; // Handle allocation failure
    ft_strlcpy(new_str, (*previous_token)->str, len_prev + 1);  //check
    ft_strlcat(new_str, (*current_token)->str, len_prev + len_curr + 1);  //check
    free(*current_token);
    return new_str;
}

/**
 * @brief Joins tokens that are enclosed in quotes.
 *
 * This function iterates through the token list in the shell structure,
 * checking if consecutive tokens are quoted. If they are, it joins them
 * using the join_quotes function.
 *
 * @param shell Pointer to the shell structure containing the token list.
 * @return true if all operations succeeded; false otherwise.
 */
bool join_quotes_tokens(t_shell *shell)
{
    t_token *current_token;
    t_token *previous_token;
	char *new_str;

    previous_token = NULL;
    current_token = shell->token;

    while (current_token != NULL)
    {
       	new_str = NULL;
        if (is_dquote(previous_token) && is_dquote(current_token))
            new_str = join_quotes(&previous_token, &current_token);
        else if (is_squote(previous_token) && is_squote(current_token))
            new_str = join_quotes(&previous_token, &current_token);
        else if ((is_dquote(previous_token) && is_squote(current_token)) ||
                 (is_squote(previous_token) && is_dquote(current_token)))
            		new_str = join_quotes(&previous_token, &current_token);
        if (new_str)
            previous_token->str = new_str;
        else
            return false;
        previous_token = current_token;
        current_token = current_token->next;
    }
    return true;
}

void process_and_remove_parens(t_shell *shell, t_token *current, t_token *word_token)
{
    t_token *token_after_par = word_token->next;
    t_token *token_before_par = previous_token_if_exists(shell->token, current);

    // Remove both opening and closing parentheses
    shell->token = remove_token(shell->token, current);   // Remove PAR_OPEN
    shell->token = remove_token(shell->token, word_token);  // Remove PAR_CLOSE

    // Handle arithmetic expansion if both parentheses are balanced
    if (token_before_par && token_before_par->id == PAR_OPEN &&
        token_after_par && token_after_par->id == PAR_CLOSE)
    {
        handle_arith_expan(&shell->token, &token_before_par, &token_after_par);
    }
}

void check_and_process_parens(t_shell *shell, t_token **current_token)
{
    t_token *next_token = (*current_token)->next;

    while (next_token && next_token->id == WORD && next_token->next)
        next_token = next_token->next;

    if (next_token && next_token->id == PAR_CLOSE)
    {
        process_and_remove_parens(shell, *current_token, next_token);
        t_token *token_after_par = next_token->next;
        if (token_after_par)
            *current_token = token_after_par;
        else
            return; // End processing if there's no more tokens
    }
    else
    {
        *current_token = (*current_token)->next;
    }
}

void remove_subshell_parens(t_shell *shell)
{
    t_token *current_token = shell->token;

    while (current_token && current_token->next)
    {
        if (current_token->id == PAR_OPEN)
        {
            check_and_process_parens(shell, &current_token);
            if (!current_token) // Check if current_token was set to NULL
                break; // End loop if there's no more tokens
        }
        else
        {
            current_token = current_token->next;
        }
    }
}


/**
 * @brief Joins consecutive word or environment variable tokens.
 *
 * This function iterates through the token list in the shell structure,
 * joining consecutive tokens of type word or environment variable. It frees
 * the current token after joining.
 *
 * @param shell Pointer to the shell structure containing the token list.
 * @return true if all operations succeeded; false otherwise.
 */
bool join_word_and_env_var_tokens(t_shell *shell)
{
    t_token *current_token;
    t_token *previous_token;
	char *joined_str;

    current_token = shell->token;
    previous_token = NULL;
    while (current_token != NULL)
    {
        if ((is_word(previous_token) || is_env_var(previous_token) ||
             is_squote(previous_token) || is_dquote(previous_token)) &&
            (is_word(current_token) || is_env_var(current_token) ||
             is_squote(current_token) || is_dquote(current_token)))
        {
            joined_str = ft_strjoin_fs1(&previous_token->str, current_token->str); // Join strings
            if (joined_str == NULL)
                return (ERR_MEM);
            previous_token->str = joined_str;
            current_token = free_token_str(current_token);
            previous_token->next = current_token;
            continue;
        }
        previous_token = current_token;
        current_token = current_token->next;
    }
    return (PARSING_OK);
}
