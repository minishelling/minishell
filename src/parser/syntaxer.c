#include "../../include/minishell.h"

/**
 * @brief Checks for balanced parentheses in the token list.
 *
 * @param head A pointer to the head of the token list.
 * @return Returns an error code if unbalanced parentheses are found,
 *         otherwise returns PARSING_OK.
 */
int	check_parens(t_token *head)
{
	int		balance;
	t_token	*current;

	balance = 0;
	current = head;
	while (current != NULL)
	{
		if (current->id == PAR_OPEN)
			balance++;
		else if (current->id == PAR_CLOSE)
		{
			if (balance <= 0)
				return (ERR_SYNTAX_UNEXPECT_CLOSE);
			balance--;
		}
		current = current->next;
	}
	if (balance > 0)
	{
		if (non_null_previous(head, current)->id == PAR_OPEN)
			return (ERR_SYNTAX_NL);
		return (ERR_SYNTAX_UNEXPECT_OPEN);
	}
	return (PARSING_OK);
}

/**
 * @brief Checks for unclosed quotes in the token list.
 *
 * @param token_list_head A pointer to the head of the token list.
 * @return Returns an error code if unclosed quotes are found,
 *         otherwise returns PARSING_OK.
 */
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

void	initialize_syntax_func(t_syntax_func func[14])
{
	func[0] = syntax_noop;
	func[1] = syntax_noop;
	func[2] = syntax_noop;
	func[3] = syntax_pipe;
	func[4] = syntax_and_opr;
	func[5] = syntax_parens;
	func[6] = syntax_parens;
	func[7] = syntax_redir;
	func[8] = syntax_redir;
	func[9] = syntax_noop;
	func[10] = syntax_noop;
	func[11] = syntax_noop;
	func[12] = syntax_word;
	func[13] = syntax_pipe;
}

int	process_syntax_checks(t_shell *shell, t_token **previous_token)
{
	t_token			*current_token;
	t_syntax_func	func[14];
	int				err_no;

	initialize_syntax_func(func);
	current_token = shell->token;
	while (current_token)
	{
		err_no = func[current_token->id] \
		(*previous_token, current_token, shell->env_list);
		if (err_no)
			return (err_no);
		*previous_token = current_token;
		current_token = skip_whitespace_and_get_next_token(current_token);
	}
	return (PARSING_OK);
}

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
