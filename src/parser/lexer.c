#include "../../include/minishell.h"

/** 
 * @brief Function pointer type for delimiter handling functions.
 *
 * This function type represents a pointer to a function that processes
 * a string, modifies the current position in the string, and updates
 * the token ID based on the delimiter found.
 *
 * @param str Pointer to the input string.
 * @param pos Pointer to the current position in the string.
 * @param token_id Pointer to the token ID to be updated.
 */
typedef void (*t_delimiter_func)(char *str, size_t *pos, t_token_id *token_id);

/** 
 * @brief Gets the token ID based on the character.
 *
 * @param c The character to analyze.
 * @return The corresponding token ID.
 */
t_token_id get_token_id(char c)
{
    t_token_id token_id;

    token_id = SPACE_CHAR;
    while (token_id != WORD)
    {
        if (c == META_CHARS_PLUS_SET[token_id])
            break;
        token_id++;
    }
    return token_id;
}

/** 
 * @brief Assigns the token ID and extracts the token string from the input.
 *
 * @param shell Pointer to the shell structure.
 * @param str The input string to parse.
 * @param pos Pointer to the current position in the string.
 * @param token Pointer to the token structure to fill.
 * @return PARSING_OK on success, ERR_MEM if memory allocation fails.
 */
int assign_token_id_and_string(char *str, size_t *pos, t_token *token)
{
    int start_pos;

    start_pos = *pos;
    t_delimiter_func func[14] = {
        [0] = &set_pos_end_space_or_word,
        [1] = &set_pos_end_space_or_word,
        [2] = &set_pos_end_space_or_word,
        [3] = &set_pos_end_pipe,
        [4] = &set_pos_end_and_opr,
        [5] = &set_pos_end_semicol,
        [6] = &set_pos_end_parentheses,
        [7] = &set_pos_end_parentheses,
        [8] = &set_pos_end_redir,
        [9] = &set_pos_end_redir,
        [10] = &set_pos_end_quote,
        [11] = &set_pos_end_quote,
        [12] = &set_pos_end_env_var,
        [13] = &set_pos_end_space_or_word,
    };
    token->id = get_token_id(str[(*pos)]);
    func[token->id](str, pos, &token->id);
    token->str = ft_substr(str, start_pos, (*pos - start_pos));  //malloc
    if (!token->str)
        return (ERR_MEM);
    return (PARSING_OK);
}

/** 
 * @brief Tokenizes the input string into a linked list of tokens.
 *
 * @param shell Pointer to the shell structure.
 * @param input The input string to tokenize.
 * @return PARSING_OK on success, ERR_MEM if memory allocation fails.
 */
int tokenize(t_shell *shell, char *input)
{
    size_t current_pos;
    t_token *token;

    shell->token = NULL;
    current_pos = 0;

    while (input[current_pos])
    {
        token = new_token();
        if (!token)
            return (ERR_MEM);
        assign_token_id_and_string(input, &current_pos, token);
        if (!token->str)
            return (ERR_MEM);
        add_token_in_back(&shell->token, token);
    }
    return (PARSING_OK);
}
