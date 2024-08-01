#include "../../include/minishell.h"

typedef void	(*t_delimiter_func) \
			(const char *input, size_t *pos, const t_token_id value);

t_token_id	get_char_id(const char c)
{
	t_token_id	value;

    char *delimiters_set = (char *)malloc(strlen(SPECIAL_CHARS) + strlen(META_CHARS_SET) + 1); 
    strcpy(delimiters_set, META_CHARS_SET);
    strcat(delimiters_set, SPECIAL_CHARS);

    // remember to free the allocated memory
	//const char	delimiters_set[] = DELIMETER_SET;

	value = SPACE_CHAR;
	while (value != WORD)
	{
		if (delimiters_set[value] == c)
		{
			// printf ("value is %d\n", value);
			break ;
		}
		value++;
	}
	//printf ("in get_char_id, c is %c and returned value is %d\n", c, value);
	return (value);
}

void	initialize_token(const char *input, size_t *pos, t_token *node)
{
	const int				start_pos = *pos;
	const t_delimiter_func	func[14] = {
	[0] = &token_id_misc,
	[1] = &token_id_misc,
	[2] = &token_id_misc,
	[3] = &token_id_pipe,
	[4] = &token_id_ampersand,
	[5] = &token_id_semicol,
	[6] = &token_id_parentheses,
	[7] = &token_id_parentheses,
	[8] = &token_id_redir,
	[9] = &token_id_redir,
	[10] = &token_id_quote,
	[11] = &token_id_quote,
	[12] = &token_id_shell_var,
	[13] = &token_id_misc,
	};

	node->id = get_char_id(input[(*pos)]);
	//printf ("in get_token_info node->id = %d\n",node->id);
	//printf ("in get_token_info pos before is %zu\n", *pos);
	func[node->id](input, pos, node->id);
	//printf ("in get_token_info pos after is %zu\n", *pos);
	node->str = ft_substr(input, start_pos, (*pos - start_pos));
}

t_token	*lexer(const char *input)
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
		initialize_token(input, &current_pos, token);
		//printf ("in lexer, token->str is %s\n", token->str);
		// if (!token->str)
		// 	return (free_token_list(top, list_token_free_node_str), \
		// 			mini_error_test(error_print, 1, "lexer: token error"), \
		// 			NULL);
		
		if (token->id ==SHELL_VAR && ft_strlen(token->str) == 1)
			token->id = WORD;
		add_token_in_back(&token_list_head, token);
	}
	return (token_list_head);
}

/* considering variable expansion:
 *
 * 		export CD="c d ef"; echo ab$CD"g"
 * 		argv would show:
 * 		argv[1] = "abc"
 * 		argv[2] = "d"
 * 		argv[3] = "efg"
 * 		argv[4] = NULL
 *
 * implying some part of expansion happens before parsing or lexigng.
 * however further testing show that the tokens that are added,
 * show up as WORD tokens
 *
 * 		VAR="cat|"; <Makefile $VAR cat >>out
 * 		bash: cat|: command not found
 *
 * 		bash-3.2$ VAR="cat |"; <Makefile $VAR cat >>out
 * 		cat: |: No such file or directory
 * 		cat: cat: No such file or directory
 *
 * add a jumptable for token_id_specific functions.
 * 


1. if end of inputut is recognised current token shall be delimited.
	if there is no current token END_input shall be returned.
	
	example:
	c ='\0'
	=>  token delimited

2. if the previous char was an operator and current char (unqouted) can be used
	with the previous chars it shall be used as part of that operator.
	
	example:
	c_prev = '>'
	c = '>'
	=>  token = '>>'

3. if the previous char was used as part of an operator and the current char
	cannot be used with the previous chars to form an operator,
	the operator containing the pervious chars shall be delimited.
	
	example:
	c_prev = ">>"
	c = 'a'
	=> token delimited (+ new token started?)

4. if the current char is backslash '\', single-quote ''', double-quote '"' and
	it is unquited, it shall affect quoting for subsequent characters
	upto the end of the quoted
	
	example:
	echo ""'$PATH'""
	=> doesn't expland PATH since both double quotes close and single quotes
	won't expand

	echo "'$PATH'"
	=> expands PATH with single quote at both sides

	echo '"'$PATH'"'
	=> expands PATH with double quotes at both sides

5. if the currrent character is an unquoted '$', the shell will identify
	the start of any candidates for parameter-expansion.
	
	not quite sure what an example is of this.
	example:
	echo $P"AT"H
	ATH
	=> expands $P [EMPTY] then prints the rest of the inputut being "AT" and H

6. if the current char is not quoted and can be used as the first char
	of a new operator, the current token shall be delimeted.
	the current char shall be used as the beginning of the next (operator)
	token.

7. if the current char is an unquoted <newline> '\n', the current token shall
	be delimited.

8. if the current char is an unquoted <blank>, any token containing 
	the previous char is delimited and the current char shall be discarded.

9. if the previous char was part of a word, the current char shall be appended
	to the word.

10. if the current char is a '#', it and all subsequent chars up to, but 
	excluding, the next <newline> '\n' shall be discarded as a comment.
	the <newline> '\n' shall not be considered part of the comment.

11. ifthe current char is used as the start of a new word.
*/
