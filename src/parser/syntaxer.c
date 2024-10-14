#include "../../include/minishell.h"

/**
 * @typedef t_syntax_func
 * @brief A function pointer type for syntax check functions.
 *
 * This type represents a pointer to a function that takes two tokens (previous and current)
 * and a pointer to an environment list, and returns an integer indicating success or an error code.
 *
 * @param t_prev A pointer to the previous token in the token list.
 * @param t_cur A pointer to the current token in the token list.
 * @param env_list A pointer to the environment list.
 * @return An integer status code indicating success or an error.
 */
typedef int (*t_syntax_func)(t_token *t_prev, t_token *t_cur, t_env *env_list);

/**
 * @brief Skips whitespace tokens in the linked list of tokens.
 *
 * @param list A pointer to the head of the token list.
 * @return A pointer to the next token that is not whitespace, or NULL if none exists.
 */
t_token *skip_whitespace(t_token *list)
{
    if (list == NULL)
        return (NULL);
    if (list->id == SPACE_CHAR || list->id == TAB_CHAR || list->id == NL)
        return (list->next);
    return (list);
}

/**
 * @brief Checks for balanced parentheses in the token list.
 *
 * @param head A pointer to the head of the token list.
 * @return Returns an error code if unbalanced parentheses are found,
 *         otherwise returns PARSING_OK.
 */
int check_parens(t_token *head)
{
    int balance = 0;
    t_token *current = head;

    while (current != NULL) 
    {
        if (current->id == PAR_OPEN)
            balance++;
        else if (current->id == PAR_CLOSE)
        {
            if (balance <= 0) 
                return ERR_SYNTAX_UNEXPECT_CLOSE;
            balance--;
        }
        current = current->next;
    }
    if (balance > 0)
    {
        if (non_null_previous(head, current)->id == PAR_OPEN)
            return ERR_SYNTAX_NL;
        return ERR_SYNTAX_UNEXPECT_OPEN;
    }
    return PARSING_OK;
}

/**
 * @brief Checks for unclosed quotes in the token list.
 *
 * @param token_list_head A pointer to the head of the token list.
 * @return Returns an error code if unclosed quotes are found,
 *         otherwise returns PARSING_OK.
 */
int check_quotes(t_token *token_list_head)
{
    t_token *current_token;
    char    *str;
    size_t  len;

    current_token = token_list_head;
    while (current_token) 
    {
        if (current_token->id == DQUOTE || current_token->id == SQUOTE)
        {
            len = ft_strlen(current_token->str);
            str = current_token->str;
            if (len == 1 || str[0] != str[len - 1])
                return (ERR_UNCLOSED_QUOTES);
        }
        current_token = current_token->next;
    }
    return (PARSING_OK);
}

/**
 * @brief Tokenizes the list of tokens to identify logical operators.
 *
 * @param token_list_head A pointer to the head of the token list.
 */
void tokenize_and_n_or_opr(t_token *token_list_head)
{
    t_token *current_token;

    current_token = token_list_head;
    while (current_token)
    {
        if (!current_token->str || current_token->str[0] == '\0')
            return ;
        if (current_token->str[0] == '&' && current_token->str[1] != '&')
            current_token->id = WORD;
        else if (!ft_strncmp(current_token->str, "||", 2))
            current_token->id = OR_OPR;
        current_token = current_token->next;
    }
}

/**
 * @brief Processes syntax checks for the token list.
 *
 * This function iterates through the token list and applies the appropriate syntax checks.
 * A new token for the OR operator (`||`) has been added to the jump table.
 *
 * @param shell A pointer to the shell structure containing token list and environment.
 * @param previous_token A pointer to the pointer of the previous token for reference.
 * @return Returns an error code if a syntax error is detected, otherwise returns PARSING_OK.
 */
int process_syntax_checks(t_shell *shell, t_token **previous_token)
{
    t_token *current_token;
    int err_no;

    current_token = skip_whitespace(shell->token);
    t_syntax_func func[14] = {
        [0] = syntax_misc,
        [1] = syntax_misc,
        [2] = syntax_misc,
        [3] = syntax_pipe,
        [4] = syntax_and_opr,
        [5] = syntax_parens,
        [6] = syntax_parens,
        [7] = syntax_redir,
        [8] = syntax_redir,
        [9] = syntax_misc,
        [10] = syntax_misc,
        [11] = syntax_misc,
        [12] = syntax_word,
        [13] = syntax_pipe
    };
    while (current_token != NULL)
    {
        err_no = func[current_token->id](*previous_token, current_token, shell->env_list);
        if (err_no)
            return (err_no);
        *previous_token = current_token;
        current_token = get_after_space_token(current_token);
    }
    return PARSING_OK;
}

/**
 * @brief Validates the syntax of the shell command.
 *
 * @param shell A pointer to the shell structure containing token list and environment.
 * @return Returns an error code if a syntax error is detected, 
 *         otherwise returns PARSING_OK.
 */
int syntax(t_shell *shell)
{
    t_token *previous_token = NULL;
    int err_no;

    if (!shell->token) 
        return (0);
    tokenize_and_n_or_opr(shell->token);
    err_no = process_syntax_checks(shell, &previous_token);
    if (err_no)
        return (err_no);
    err_no = check_parens(shell->token);
    if (err_no)
        return (err_no);
    err_no = check_quotes(shell->token);
    return (err_no);
}
