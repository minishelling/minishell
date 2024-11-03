/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfeuer <tfeuer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 13:26:51 by tfeuer            #+#    #+#             */
/*   Updated: 2024/10/31 13:26:52 by tfeuer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

t_ecode_p			tokenize(t_shell *shell, char *input);
static t_ecode_p	assign_token_id_and_string(char *str, size_t *pos, \
	t_token *token);
static void			initialize_lexing_funcs(t_lexer_func advance[13]);
t_token_id			get_token_id(char c);

/**
 * @brief Tokenizes the input string into a linked list of tokens.
 *
 * Parses the input string, creating tokens, assigning IDs, and appending 
 * each token to the shell's token list. After the initial tokenization, 
 * it distinguishes between `&` and `&&` as well as `|` and `||`, making 
 * AMPERSAND and OR_OPR tokens, ensuring correct token IDs are assigned 
 * for these operators.
 *
 * @param shell Pointer to the shell structure holding the token list.
 * @param input Input string to be tokenized.
 * 
 * @return t_ecode_p PARSING_OK if successful, or an error code (e.g., ERR_MEM,
 *         ERR_CMD_SUBSTIT) on failure.
 */
t_ecode_p	tokenize(t_shell *shell, char *input)
{
	size_t		current_pos;
	t_token		*token;
	t_ecode_p	err_no;

	current_pos = 0;
	while (input[current_pos])
	{
		token = new_token();
		if (!token)
			return (ERR_MEM);
		err_no = assign_token_id_and_string(input, &current_pos, token);
		if (err_no == ERR_CMD_SUBSTIT)
			return (free_token(&token), err_no);
		add_token_in_back(&shell->token, token);
	}
	token = shell->token;
	while (token)
	{
		if (token->str[0] == '&' && token->str[1] != '&')
			token->id = AMPERSAND;
		else if (!ft_strncmp(token->str, "||", 3))
			token->id = OR_OPR;
		token = token->next;
	}
	return (PARSING_OK);
}

/**
 * @brief Assigns a token ID and extracts the token string from the input.
 *
 * Determines the token's ID based on the current character in the input
 * string and advances the position accordingly. The identified substring
 * is stored in `token->str`.
 *
 * @param str Input string being tokenized.
 * @param pos Pointer to the current position in the input string.
 * @param token Pointer to the token structure where the ID and string are 
 *              stored.
 * 
 * @return t_ecode_p PARSING_OK if successful, or an error code (e.g., 
 *         ERR_MEM, ERR_CMD_SUBSTIT) on failure.
 */
static t_ecode_p	assign_token_id_and_string(char *str, size_t *pos, \
	t_token *token)
{
	int				start_pos;
	t_ecode_p		err_no;
	t_lexer_func	advance[13];
	char			*token_str;

	initialize_lexing_funcs(advance);
	start_pos = *pos;
	token->id = get_token_id(str[(*pos)]);
	err_no = advance[token->id](str, pos, &token->id);
	if (err_no == ERR_CMD_SUBSTIT)
		return (err_no);
	token_str = ft_substr(str, start_pos, (*pos - start_pos));
	if (!token_str)
		return (ERR_MEM);
	safe_assign_str(&(token->str), token_str);
	free(token_str);
	if (!token->str)
		return (ERR_MEM);
	return (PARSING_OK);
}

/**
 * @brief Initializes an array of function pointers for lexing.
 *
 * Sets up an array of function pointers to be used in token lexing based on
 * specific token types.
 *
 * @param advance Array to store function pointers for advancing position.
 */
static void	initialize_lexing_funcs(t_lexer_func advance[13])
{
	advance[0] = advance_pos_space_or_word;
	advance[1] = advance_pos_space_or_word;
	advance[2] = advance_pos_space_or_word;
	advance[3] = advance_pos_pipe;
	advance[4] = advance_pos_and_operator;
	advance[5] = advance_pos_parens;
	advance[6] = advance_pos_parens;
	advance[7] = advance_pos_redir;
	advance[8] = advance_pos_redir;
	advance[9] = advance_pos_quote;
	advance[10] = advance_pos_quote;
	advance[11] = advance_pos_env_var;
	advance[12] = advance_pos_space_or_word;
}

/**
 * @brief Gets the token ID for a given character.
 *
 * Determines the token ID associated with a specific character based on
 * predefined metacharacters.
 *
 * @param c The character to evaluate.
 *
 * @return t_token_id The token ID associated with the character.
 */
t_token_id	get_token_id(char c)
{
	t_token_id	token_id;

	token_id = SPACE_CHAR;
	while (token_id != WORD)
	{
		if (c == META_CHARS_PLUS_SET[token_id])
			break ;
		token_id++;
	}
	return (token_id);
}
