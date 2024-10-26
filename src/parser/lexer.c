#include "../../include/minishell.h"

void	classify_and_or_operators(t_token *token_list_head)
{
	t_token	*current_token;

	current_token = token_list_head;
	while (current_token)
	{
		if (current_token->str[0] == '&' && current_token->str[1] != '&')
			current_token->id = WORD;
		else if (!ft_strncmp(current_token->str, "||", 2))
			current_token->id = OR_OPR;
		current_token = current_token->next;
	}
}

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

int	assign_token_id_and_string(char *str, size_t *pos, t_token *token)
{
	int				start_pos;
	t_lexer_func	advance[13];
	char			*token_str;

	start_pos = *pos;
	advance[0] = &advance_pos_space_or_word;
	advance[1] = &advance_pos_space_or_word;
	advance[2] = &advance_pos_space_or_word;
	advance[3] = &advance_pos_pipe;
	advance[4] = &advance_pos_and_operator;
	advance[5] = &advance_pos_parens;
	advance[6] = &advance_pos_parens;
	advance[7] = &advance_pos_redir;
	advance[8] = &advance_pos_redir;
	advance[9] = &advance_pos_quote;
	advance[10] = &advance_pos_quote;
	advance[11] = &advance_pos_env_var;
	advance[12] = &advance_pos_space_or_word;
	token->id = get_token_id(str[(*pos)]);
	advance[token->id](str, pos, &token->id);
	token_str = ft_substr(str, start_pos, (*pos - start_pos));
	if (!token_str)
		return (ERR_MEM);
	safe_assign_str(&(token->str), token_str);
	free (token_str);
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
int	tokenize(t_shell *shell, char *input)
{
	size_t	current_pos;
	t_token	*token;

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
	classify_and_or_operators(shell->token);
	return (PARSING_OK);
}
