#include "../../include/minishell.h"

int			tokenize(t_shell *shell, char *input);
int			assign_token_id_and_string(char *str, size_t *pos, t_token *token);
void		initialize_lexing_funcs(t_lexer_func advance[13]);
t_token_id	get_token_id(char c);

int	tokenize(t_shell *shell, char *input)
{
	size_t	current_pos;
	t_token	*token;
	int		err_no;

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

int	assign_token_id_and_string(char *str, size_t *pos, t_token *token)
{
	int				start_pos;
	int				err_no;
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

void	initialize_lexing_funcs(t_lexer_func advance[13])
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
