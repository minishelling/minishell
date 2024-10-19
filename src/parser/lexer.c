#include "../../include/minishell.h"

/**
 * @brief Identifies and assigns token IDs for logical operators (`&&` and `||`).
 * 
 * This function traverses the token list and assigns the appropriate token ID 
 * for logical operators `&&` (AND operator) and `||` (OR operator). If a single 
 * `&` is encountered, it is considered a WORD token.
 * 
 * @param token_list_head A pointer to the head of the token list to be processed.
 */
void classify_and_or_operators(t_token *token_list_head)
{
	t_token *current_token;

	current_token = token_list_head;
	while (current_token)
	{
		if (current_token->str[0] && current_token->str[1] && 
				current_token->str[0] == '&' && current_token->str[1] != '&')
					current_token->id = WORD;
		else if (!ft_strncmp(current_token->str, "||", 2))
			current_token->id = OR_OPR;
		
		current_token = current_token->next;
	}
}

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
 * @brief Assigns a token ID and extracts the token string from the input string.
 * 
 * This function identifies the token at the current position in the input string,
 * moves the position pointer to the appropriate location based on the token type,
 * and extracts the token's string from the input string. It uses an array of 
 * functions to handle different token types (e.g., spaces, pipes, redirections, etc.).
 * The extracted token string is allocated dynamically and returned to the caller.
 * 
 * @param str A pointer to the input string.
 * @param pos A pointer to the current position in the input string, which will
 *            be updated as the function advances through the string.
 * @param token A pointer to the token structure that will hold the token ID and
 *              the extracted token string.
 * 
 * @return An integer representing the success or failure of the operation:
 *         - PARSING_OK on success,
 *         - ERR_MEM if memory allocation fails.
 */
int assign_token_id_and_string(char *str, size_t *pos, t_token *token)
{
	int start_pos;
	
	start_pos = *pos;
	t_delimiter_func func[13] = {
		[0] = &advance_pos_space_or_word,
		[1] = &advance_pos_space_or_word,
		[2] = &advance_pos_space_or_word,
		[3] = &advance_pos_pipe,
		[4] = &advance_pos_and_operator,
		[5] = &advance_pos_parens,
		[6] = &advance_pos_parens,
		[7] = &advance_pos_redir,
		[8] = &advance_pos_redir,
		[9] = &advance_pos_quote,
		[10] = &advance_pos_quote,
		[11] = &advance_pos_env_var,
		[12] = &advance_pos_space_or_word,
	};
	token->id = get_token_id(str[(*pos)]);
	func[token->id](str, pos, &token->id);
	token->str = ft_substr(str, start_pos, (*pos - start_pos));  //!
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
	classify_and_or_operators(shell->token);
}
