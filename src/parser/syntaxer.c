#include "../../include/minishell.h"

int		syntax(t_shell *shell);
int		process_syntax_checks(t_shell *shell, t_token **prev_token);
void	initialize_syntax_func(t_syntax_func func[14]);
int		check_parens(t_token *list);
int		check_quotes(t_token *list);

int	syntax(t_shell *shell)
{
	t_token	*prev_token;
	int		err_no;

	prev_token = NULL;
	err_no = process_syntax_checks(shell, &prev_token);
	if (err_no)
		return (err_no);
	err_no = check_parens(shell->token);
	if (err_no)
		return (err_no);
	err_no = check_quotes(shell->token);
	return (err_no);
}

int	process_syntax_checks(t_shell *shell, t_token **prev_token)
{
	t_token			*current_token;
	t_syntax_func	func[14];
	int				err_no;
	int				par_count;

	par_count = 0;
	initialize_syntax_func(func);
	current_token = shell->token;
	while (current_token)
	{
		err_no = func[current_token->id] \
		(*prev_token, current_token, &par_count);
		if (err_no)
			return (err_no);
		*prev_token = current_token;
		current_token = skip_whitespace_and_get_next_token(current_token);
	}
	return (PARSING_OK);
}

void	initialize_syntax_func(t_syntax_func func[14])
{
	func[0] = syntax_noop;
	func[1] = syntax_noop;
	func[2] = syntax_noop;
	func[3] = syntax_pipe;
	func[4] = syntax_and_opr;
	func[5] = syntax_open_paren;
	func[6] = syntax_close_paren;
	func[7] = syntax_redir;
	func[8] = syntax_redir;
	func[9] = syntax_quote;
	func[10] = syntax_quote;
	func[11] = syntax_env_var;
	func[12] = syntax_word;
	func[13] = syntax_or_opr;
}
//the case in which balace in negative i.e. parens closing before there is a
// matching open paren, is already handled in "syntax_close_paren".
int	check_parens(t_token *list)
{
	int		balance;
	t_token	*cur_token;

	balance = 0;
	cur_token = list;
	while (cur_token)
	{
		if (cur_token->id == PAR_OPEN)
			balance++;
		else if (cur_token->id == PAR_CLOSE)
			balance--;
		cur_token = cur_token->next;
	}
	if (balance > 0)
	{
		if (non_null_previous(list, cur_token)->id == PAR_OPEN)
			return (ERR_SYNTAX_NL);
		return (ERR_PARSING_ERROR);
	}
	return (PARSING_OK);
}

int	check_quotes(t_token *list)
{
	t_token	*cur_token;
	size_t	str_len;
	char	*str;


	cur_token = list;
	while (cur_token)
	{
		if (cur_token->id == DQUOTE || cur_token->id == SQUOTE)
		{
			str_len = ft_strlen(cur_token->str);
			str = cur_token->str;
			if (str_len == 1 || str[0] != str[str_len - 1])
				return (ERR_UNCLOSED_QUOTES);
		}
		cur_token = cur_token->next;
	}
	return (PARSING_OK);
}






