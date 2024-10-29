#include "../../include/minishell.h"

t_ecode_p	syntax(t_shell *shell);
t_ecode_p	process_syntax_checks(t_shell *shell, t_token **prev_token);
void		initialize_syntaxing_funcs(t_syntax_func check_syntax[15]);
t_ecode_p	check_parens(t_token *list);
t_ecode_p	check_quotes(t_token *list);

t_ecode_p	syntax(t_shell *shell)
{
	t_token		*prev_token;
	t_token		*cur_token;
	t_ecode_p	err_no;

	prev_token = NULL;
	err_no = process_syntax_checks(shell, &prev_token);
	if (err_no)
		return (err_no);
	err_no = check_parens(shell->token);
	if (err_no)
		return (err_no);
	err_no = check_quotes(shell->token);
	if (err_no)
		return (err_no);
	cur_token = shell->token;
	while (cur_token)
	{
		if (cur_token->id == AMPERSAND)
			return (ERR_BG_PROCESS);
		cur_token = cur_token->next;
	}
	return (PARSING_OK);
}

t_ecode_p	process_syntax_checks(t_shell *shell, t_token **prev_token)
{
	t_token			*current_token;
	t_syntax_func	check_syntax[15];
	t_ecode_p		err_no;
	int				par_count;

	par_count = 0;
	initialize_syntaxing_funcs(check_syntax);
	current_token = shell->token;
	while (current_token)
	{
		err_no = check_syntax[current_token->id] \
		(*prev_token, current_token, &par_count);
		if (err_no)
			return (err_no);
		*prev_token = current_token;
		current_token = skip_whitespace_and_get_next_token(current_token);
	}
	return (PARSING_OK);
}

void	initialize_syntaxing_funcs(t_syntax_func check_syntax[15])
{
	check_syntax[0] = syntax_noop;
	check_syntax[1] = syntax_noop;
	check_syntax[2] = syntax_noop;
	check_syntax[3] = syntax_pipe;
	check_syntax[4] = syntax_and_opr;
	check_syntax[5] = syntax_open_paren;
	check_syntax[6] = syntax_close_paren;
	check_syntax[7] = syntax_redir;
	check_syntax[8] = syntax_redir;
	check_syntax[9] = syntax_quote;
	check_syntax[10] = syntax_quote;
	check_syntax[11] = syntax_env_var;
	check_syntax[12] = syntax_word;
	check_syntax[13] = syntax_or_opr;
	check_syntax[14] = syntax_ampersand;
}

//the case in which balace in negative i.e. parens closing before there is a
// matching open paren, is already handled in "syntax_close_paren".
t_ecode_p	check_parens(t_token *list)
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

t_ecode_p	check_quotes(t_token *list)
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
