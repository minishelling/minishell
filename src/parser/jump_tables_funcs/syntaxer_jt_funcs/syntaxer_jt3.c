#include "../../../../include/minishell.h"

t_ecode_p	syntax_redir(t_token *prev_token, t_token *cur_token, \
	int *par_count);
t_ecode_p	handle_token_after_redir(t_token *after_redir_token);
t_ecode_p	remove_delimiter_quotes(t_token *delimiter_token);
t_ecode_p	syntax_quote(t_token *prev_token, t_token *cur_token, \
	int *par_count);
t_ecode_p	syntax_env_var(t_token *prev_token, t_token *cur_token, \
	int *par_count);

t_ecode_p	syntax_redir(t_token *prev_token, t_token *cur_token, \
	int *par_count)
{
	t_token		*next_token;
	t_ecode_p	err_no;

	(void)prev_token;
	(void)par_count;
	next_token = skip_whitespace_and_get_next_token(cur_token);
	if (!next_token)
		return (ERR_SYNTAX_NL);
	if (next_token->id == ENV_VAR && !ft_strncmp((cur_token->str), "<<", 3))
	{
		next_token->id = WORD;
		return (PARSING_OK);
	}
	err_no = handle_token_after_redir(next_token);
	if (err_no)
		return (err_no);
	return (PARSING_OK);
}

t_ecode_p	handle_token_after_redir(t_token *after_redir_token)
{
	if (after_redir_token->id == PIPE)
		return (ERR_SYNTAX_PIPE);
	if (after_redir_token->id == AND_OPR)
		return (ERR_SYNTAX_AND);
	if (after_redir_token->id == OR_OPR)
		return (ERR_SYNTAX_OR);
	if (after_redir_token->id == PAR_OPEN)
		return (ERR_SYNTAX_OPEN_PAR);
	if (after_redir_token->id == PAR_CLOSE)
		return (ERR_SYNTAX_CLOSE_PAR);
	if (after_redir_token->id == GT || after_redir_token->id == LT)
		return (ERR_SYNTAX_REDIR);
	if (after_redir_token->id == AMPERSAND)
		return (ERR_SYNTAX_AMPER);
	if (after_redir_token->id == SQUOTE || after_redir_token->id == DQUOTE)
		if (remove_delimiter_quotes(after_redir_token) != SUCCESS)
			return (ERR_MEM);
	return (PARSING_OK);
}

t_ecode_p	remove_delimiter_quotes(t_token *delimiter_token)
{
	delimiter_token->id = WORD;
	if (safe_assign_str(&(delimiter_token->str), \
		(ft_substr(delimiter_token->str, 1, \
			(ft_strlen(delimiter_token->str) - 2)))) != SUCCESS)
		return (ERR_MEM);
	return (PARSING_OK);
}

t_ecode_p	syntax_quote(t_token *prev_token, t_token *cur_token, \
	int *par_count)
{
	t_token	*next_token;

	(void)prev_token;
	(void)par_count;
	next_token = skip_whitespace_and_get_next_token(cur_token);
	if (next_token && next_token->id == PAR_OPEN)
		return (ERR_SYNTAX_OPEN_PAR);
	return (PARSING_OK);
}

// cmd substitution is already handled in tokenization.
t_ecode_p	syntax_env_var(t_token *prev_token, t_token *cur_token, \
	int *par_count)
{
	t_token	*next_token;

	(void)prev_token;
	(void)cur_token;
	(void)par_count;
	next_token = skip_whitespace_and_get_next_token(cur_token);
	if (next_token && next_token->id == PAR_OPEN)
		return (ERR_SYNTAX_ERROR);
	return (PARSING_OK);
}
