#include "../../include/minishell.h"

typedef bool	(*t_syntax_func) (t_token *t_prev, t_token *t_cur, \
		t_env *env_list);

t_token	*skip_whitespace(t_token *list)
{
	if (list == NULL)
		return (NULL);
	if (list->id == SPACE_CHAR || list->id == TAB_CHAR || list->id == NL)
		return (list->next);
	return (list);
}



bool is_par_closed(t_token *token_list_head) 
{
	t_token *current_token;
	int paren_count;

	paren_count = 0;
	current_token = token_list_head;
	while (current_token) 
	{
		//printf("current token is %s\n", current_token->str);
		if (current_token->id == PAR_OPEN) 
			paren_count++;

		else if (current_token->id == PAR_CLOSE) 
		{
			if (paren_count == 0) 
				return 0;
			paren_count--;
		}
		current_token = current_token->next;
	}
	return (paren_count == 0);
}

bool	is_quote_closed(t_token *token_list_head)
{
	t_token *current_token;
	char	*str;
	size_t	len;
	
	current_token = token_list_head;
	while (current_token) 
	{
		if (current_token->id == DQUOTE || current_token->id == SQUOTE)
		{
			len = ft_strlen(current_token->str);
			str = current_token->str;
			if (len == 1 || str[0] != str[len - 1])
			{
				//printf("str[0] is %c and quote is unclosed\n", str[0]);
				//error 258, "unclosed quotes";
				return (false);
			}
		}
		current_token = current_token->next;
	}	
	return (true);
}

void	tokenize_and_n_or_opr(t_token *token_list_head)
{
	t_token *current_token;

	current_token = token_list_head;
	while (current_token)
	{
		if (!ft_strncmp(current_token->str, "&", 1))
			current_token->id = WORD;
		else if (!ft_strncmp(current_token->str,"||", 2))
			current_token->id = OR_OPR;
		current_token = current_token->next;
	}
}


int	syntax(t_shell *shell)
{
	// t_token				*previous_token;
	// t_token				*current_token;
	bool 				quote_closed;
	bool 				par_closed;
	
	if (!shell->token) 
		return (0);
	
	quote_closed = is_quote_closed(shell->token);
	if (!quote_closed)
		return (ERR_SYNTAX_QUOTES);
	par_closed = is_par_closed(shell->token);
	if (!par_closed)
		return ( ERR_SYNTAX_PAREN);
	tokenize_and_n_or_opr(shell->token);
	print_token(shell->token);
	
	// t_syntax_func	func[15] = {
	// [0] = NULL,
	// [1] = NULL,
	// [2] = NULL,
	// [3] = syntax_id_pipe,
	// [4] = syntax_id_and_opr,
	// [5] = syntax_id_semicol,
	// [6] = syntax_id_parentheses_open,
	// [7] = syntax_id_parentheses_open,
	// [8] = syntax_id_redir,
	// [9] = syntax_id_redir,
	// [10] = syntax_id_misc,
	// [11] = syntax_id_misc,
	// [12] = syntax_id_misc,
	// [13] = syntax_id_misc,
	// [14] = syntax_id_misc // or_opr
	// };

	// previous_token = NULL;
	// current_token = skip_whitespace(shell->token);
	// while (current_token != NULL)
	// {
	// 	printf ("in syntax, current_token->id is %d\n", current_token->id);
	// 	if (func[current_token->id](previous_token, current_token, shell->env_list))
	// 		return (0);
	// 	previous_token = current_token;
	// 	current_token = get_after_space_token(current_token);
	// }
	return (0);
}

