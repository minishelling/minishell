#include "../../../include/minishell.h"

int	syntax_noop(t_token *prev_token, t_token *cur_token, int *par_count)
{
	(void) prev_token;
	(void) cur_token;
	(void)par_count;
	return (PARSING_OK);
}

int	syntax_pipe(t_token *prev_token, t_token *cur_token, int *par_count)
{
	t_token	*next_token;

	(void)par_count;
	next_token = skip_whitespace_and_get_next_token(cur_token);
	if (!prev_token)
		return (ERR_SYNTAX_PIPE);
	if (!next_token)
		return (ERR_PARSING_ERROR);
	if (next_token->id == PIPE)
		return (ERR_SYNTAX_PIPE);
	if (next_token->id == AND_OPR)
		return (ERR_SYNTAX_AND);
	if (next_token->id == OR_OPR)
		return (ERR_SYNTAX_OR);
	if (next_token->id == PAR_CLOSE)
		return (ERR_SYNTAX_CLOSE_PAR);
	if (next_token->id == PAR_CLOSE)
		return (ERR_SYNTAX_CLOSE_PAR);
	return (PARSING_OK);
}

int	syntax_or_opr(t_token *prev_token, t_token *cur_token, int *par_count)
{
	t_token	*next_token;

	(void)par_count;
	next_token = skip_whitespace_and_get_next_token(cur_token);
	if (!prev_token)
			return (ERR_SYNTAX_OR);
	 if (!next_token)
		return (ERR_PARSING_ERROR);
	if (next_token->id == PIPE)
		return (ERR_SYNTAX_PIPE);
	if (next_token->id == OR_OPR)
		return (ERR_SYNTAX_OR);
	if (next_token->id == AND_OPR)
		return (ERR_SYNTAX_AND);
	if (next_token->id == PAR_CLOSE)
		return (ERR_SYNTAX_CLOSE_PAR);
	return (PARSING_OK);
}

int	syntax_and_opr(t_token *prev_token, t_token *cur_token, int *par_count)
{
	t_token	*next_token;

	(void)par_count;
	next_token = skip_whitespace_and_get_next_token(cur_token);
	if (!prev_token)
		return (ERR_SYNTAX_AND);
	if (!next_token)
		return (ERR_PARSING_ERROR);
	if (next_token->id == PIPE)
		return (ERR_SYNTAX_PIPE);
	if (next_token->id == OR_OPR)
		return (ERR_SYNTAX_OR);
	if (next_token->id == AND_OPR)
		return (ERR_SYNTAX_AND);
	if (next_token->id == PAR_CLOSE)
		return (ERR_SYNTAX_CLOSE_PAR);
	return (PARSING_OK);
}

int	parens_count_check(t_token *cur_token, int *par_count)
{
	if (cur_token->id == PAR_OPEN)
		(*par_count)++;
	else if (cur_token->id == PAR_CLOSE)
		(*par_count)--;
	if (*par_count < 0)
		return (ERR_SYNTAX_CLOSE_PAR);
	return (PARSING_OK);	
}


int	syntax_open_paren(t_token *prev_token, t_token *cur_token, int *par_count)
{
	t_token	*next_token;
	int		err_no;

	(void)prev_token;
	(void) cur_token;
	err_no = parens_count_check(cur_token, par_count);
	if (err_no)
		return (err_no);
	next_token = skip_whitespace_and_get_next_token(cur_token);
	if (!next_token)
		return (ERR_PARSING_ERROR);
	if (next_token->id == PIPE)
		return (ERR_SYNTAX_PIPE);
	if (next_token->id == AND_OPR)
		return (ERR_SYNTAX_AND);
	if (next_token->id == OR_OPR)
		return (ERR_SYNTAX_OR);
	if (next_token->id == PAR_CLOSE)
		return (ERR_SYNTAX_CLOSE_PAR);
	return (PARSING_OK);
}

int	syntax_close_paren(t_token *prev_token, t_token *cur_token, int *par_count)
{
	t_token	*next_token;
	int		err_no;

	(void) cur_token;
	err_no = parens_count_check(cur_token, par_count);
	if (err_no)
		return (err_no);
	if (!prev_token)
		return (ERR_SYNTAX_CLOSE_PAR);
	next_token = skip_whitespace_and_get_next_token(cur_token);
	if (next_token)
	{
		if (next_token->id == PAR_OPEN)
			return (ERR_SYNTAX_OPEN_PAR);
		if ((next_token->id == SQUOTE || next_token->id == DQUOTE || next_token->id == WORD))
			return (ERR_SYNTAX_ERROR);
	}
	return (PARSING_OK);
}

int	remove_delimiter_quotes(t_token *delimiter_token)
{
	delimiter_token->id = WORD;
	if (safe_assign_str(&(delimiter_token->str), \
		(ft_substr(delimiter_token->str, 1, \
			(ft_strlen(delimiter_token->str) - 2)))) != SUCCESS)
		return (ERR_MEM);
	return (PARSING_OK);
}

int	syntax_redir(t_token *prev_token, t_token *cur_token, int *par_count)
{
	t_token	*next_token;

	(void) prev_token;
	(void)par_count;
	next_token = skip_whitespace_and_get_next_token(cur_token);
	if (!next_token)
		return (ERR_SYNTAX_NL);
	if (next_token->id == PIPE)
		return (ERR_SYNTAX_PIPE);
	if (next_token->id == AND_OPR)
		return (ERR_SYNTAX_AND);
	if (next_token->id == OR_OPR)
		return (ERR_SYNTAX_OR);
	if (next_token->id == PAR_OPEN)
		return (ERR_SYNTAX_OPEN_PAR);
	if (next_token->id == PAR_CLOSE)
		return (ERR_SYNTAX_CLOSE_PAR);
	if (next_token->id == GT || next_token->id == LT)
		return (ERR_SYNTAX_REDIR);
	if (next_token->id == SQUOTE || next_token->id == DQUOTE)
		if (remove_delimiter_quotes(next_token) != SUCCESS)
			return (ERR_MEM);
	if (next_token->id == ENV_VAR && !ft_strncmp((cur_token->str), "<<", 2))
		next_token->id = WORD;	
	return (PARSING_OK);
}
// cmd substitution is already handled in tokenization.
int	syntax_env_var(t_token *prev_token, t_token *cur_token, int *par_count)
{
	t_token	*next_token;

	(void)par_count;
	(void)prev_token;
	(void)cur_token;
	next_token = skip_whitespace_and_get_next_token(cur_token);
	if (next_token && next_token->id == PAR_OPEN)
		return (ERR_SYNTAX_ERROR);
	return (PARSING_OK);
}


int	syntax_word(t_token *prev_token, t_token *cur_token, int *par_count)
{
	t_token	*next_token;

	(void)par_count;
	(void)prev_token;
	(void)cur_token;
	next_token = skip_whitespace_and_get_next_token(cur_token);
	if (next_token)
	{
		if (cur_token->id == WORD && next_token->id == PAR_OPEN)
		{
			if (next_token->next == NULL)
				return (ERR_SYNTAX_NL);
			else
				return (ERR_SYNTAX_ERROR);
		}
	}
	return (PARSING_OK);
}


int	syntax_quote(t_token *prev_token, t_token *cur_token, int *par_count)
{
	t_token *next_token;

	(void)prev_token;
	(void)par_count;
	next_token = skip_whitespace_and_get_next_token(cur_token);
	
	if (next_token && next_token->id == PAR_OPEN)
		return (ERR_SYNTAX_OPEN_PAR);

	return (PARSING_OK);
}