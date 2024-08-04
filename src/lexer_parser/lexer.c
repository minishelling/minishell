#include "../../include/minishell.h"

typedef void	(*t_delimiter_func) \
			(char *input, size_t *pos, t_token_id value);

t_token_id	get_token_id(char c)
{
	t_token_id	token_id;

    char *delimiters_set = (char *)malloc(strlen(SPECIAL_CHARS) + strlen(META_CHARS_SET) + 1); 
    strcpy(delimiters_set, META_CHARS_SET);
    strcat(delimiters_set, SPECIAL_CHARS);

    // remember to free the allocated memory

	token_id = SPACE_CHAR;
	while (token_id != WORD)
	{
		if (delimiters_set[token_id] == c)
			break ;
		token_id++;
	}
	//printf ("in get_char_id, c is %c and returned value is %d\n", c, value);
	return (token_id);
}

void	set_token_id_n_str(char *str, size_t *pos, t_token *token)
{
	int	start_pos;

	start_pos = *pos;
	t_delimiter_func	func[14] = {
	[0] = &set_pos_end_space_or_word,
	[1] = &set_pos_end_space_or_word,
	[2] = &set_pos_end_space_or_word,
	[3] = &set_pos_end_pipe,
	[4] = &set_pos_end_ampersand,
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
	//printf ("in get_token_info node->id = %d\n",node->id);
	//printf ("in get_token_info pos before is %zu\n", *pos);
	func[token->id](str, pos, token->id);
	//printf ("in get_token_info pos after is %zu\n", *pos);
	token->str = ft_substr(str, start_pos, (*pos - start_pos));
}

t_token	*lexer(char *input)
{
	size_t	current_pos;
	t_token	*token_list_head;
	t_token	*token;

	token_list_head = NULL;
	current_pos = 0;
	while (input[current_pos])
	{
		token = new_token();
		// if (!node)
		// 	mini_error_test(error_print, ERROR, "parser/lexer.c: lexer @ malloc");
		set_token_id_n_str(input, &current_pos, token);
		//printf ("in lexer, token->str is %s\n", token->str);
		// if (!token->str)
		// 	return (free_token_list(top, list_token_free_node_str), \
		// 			mini_error_test(error_print, 1, "lexer: token error"), \
		// 			NULL);
		
		if (token->id ==ENV_VAR && ft_strlen(token->str) == 1)
			token->id = WORD;
		add_token_in_back(&token_list_head, token);
	}
	return (token_list_head);
}


