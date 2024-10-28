#include "../../include/minishell.h"

int		syntax(t_shell *shell);
int		process_syntax_checks(t_shell *shell, t_token **previous_token);
void	initialize_syntax_func(t_syntax_func func[14]);
int		check_parens(t_token *head);
int		check_quotes(t_token *token_list_head);

int	syntax(t_shell *shell)
{
	t_token	*previous_token;
	int		err_no;

	previous_token = NULL;
	err_no = process_syntax_checks(shell, &previous_token);
	if (err_no)
		return (err_no);
	err_no = check_parens(shell->token);
	if (err_no)
		return (err_no);
	err_no = check_quotes(shell->token);
	return (err_no);
}

int	process_syntax_checks(t_shell *shell, t_token **previous_token)
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
		(*previous_token, current_token, &par_count);
		if (err_no)
			return (err_no);
		*previous_token = current_token;
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
int	check_parens(t_token *head)
{
	int		balance;
	t_token	*current;

	balance = 0;
	current = head;
	while (current)
	{
		if (current->id == PAR_OPEN)
			balance++;
		else if (current->id == PAR_CLOSE)
			balance--;
		current = current->next;
	}
	if (balance > 0)
	{
		if (non_null_previous(head, current)->id == PAR_OPEN)
			return (ERR_SYNTAX_NL);
		return (ERR_PARSING_ERROR);
	}
	return (PARSING_OK);
}

int	check_quotes(t_token *token_list_head)
{
	t_token	*current_token;
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
				return (ERR_UNCLOSED_QUOTES);
		}
		current_token = current_token->next;
	}
	return (PARSING_OK);
}






